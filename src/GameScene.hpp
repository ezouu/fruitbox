#pragma once
/* --------------------------------------------------------------------
 *  Fruit-Box “Game” scene
 *  – 10×17 grid of apples
 *  – 2-minute timer
 *  – selectable rectangles that disappear if they sum to 10
 *  – optional FPS display
 *  – writes the score to the persistent leaderboard when time is up
 * ------------------------------------------------------------------ */
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <iomanip>
#include <sstream>
#include <algorithm>

#include "Scene.hpp"
#include "InputBindings.hpp"
#include "Settings.hpp"
#include "ScoreStore.hpp"

class GameScene : public Scene {
    /* ------------ injected from the outside ----------------------- */
    sf::Font&        font_;
    InputBindings&   binds_;
    Settings&        set_;

    /* ------------ scene-to-scene plumbing ------------------------- */
    SceneID          next_     = SceneID::None;

    /* ------------ board data -------------------------------------- */
    static constexpr int ROWS = 10, COLS = 17;
    std::vector<std::vector<int>>  value_;   // 1-9 per cell
    std::vector<std::vector<bool>> alive_;   // true = still visible

    /* ------------ geometry cache ---------------------------------- */
    float        cell_   = 0.f;              // side of a square cell in px
    sf::Vector2f origin_;                    // top-left corner of grid

    /* ------------ selection state --------------------------------- */
    bool         dragging_ = false;
    sf::Vector2i a_, b_;                     // drag endpoints (cell coords)

    /* ------------ game state -------------------------------------- */
    int          score_      = 0;
    float        timeLeft_   = 120.f;        // seconds
    float        fpsSmooth_  = 0.f;          // exponential-moving FPS
    bool         recorded_   = false;        // prevents double logging
    std::mt19937 rng_{std::random_device{}()};

public:
    GameScene(sf::Font& f,
              InputBindings& b,
              Settings& s,
              sf::Vector2u  winSize)
        : font_(f), binds_(b), set_(s)
    {
        resetBoard();
        recalcGeometry(winSize);
    }

    /* ------------- helpers ---------------------------------------- */
    void recalcGeometry(sf::Vector2u w)
    {
        cell_   = std::min(w.x / float(COLS), w.y / float(ROWS));
        origin_ = { (w.x - cell_ * COLS) / 2.f,
                    (w.y - cell_ * ROWS) / 2.f };
    }

    void resetBoard()
    {
        value_.assign(ROWS, std::vector<int>(COLS));
        alive_.assign(ROWS, std::vector<bool>(COLS, true));
        std::uniform_int_distribution<int> d(1, 9);
        for (auto& row : value_)
            for (int& v : row) v = d(rng_);
        score_     = 0;
        timeLeft_  = 120.f;
        recorded_  = false;
    }

    bool valid(sf::Vector2i c) const
    { return c.x >= 0 && c.x < COLS && c.y >= 0 && c.y < ROWS; }

    sf::Vector2i winToCell(sf::Vector2f p) const
    {
        p -= origin_;
        return { int(p.x / cell_), int(p.y / cell_) };
    }

    void applySelection()
    {
        int r1 = std::min(a_.y, b_.y), r2 = std::max(a_.y, b_.y);
        int c1 = std::min(a_.x, b_.x), c2 = std::max(a_.x, b_.x);

        int sum = 0;
        for (int r = r1; r <= r2; ++r)
            for (int c = c1; c <= c2; ++c)
                if (alive_[r][c]) sum += value_[r][c];

        if (sum == 10) {
            ++score_;
            for (int r = r1; r <= r2; ++r)
                for (int c = c1; c <= c2; ++c)
                    alive_[r][c] = false;
        }
    }

    /* ------------- Scene interface -------------------------------- */
    void handleEvent(const sf::Event& e) override
    {
        if (e.type == sf::Event::Resized)
            recalcGeometry({ e.size.width, e.size.height });

        /* ---- keyboard shortcuts ---- */
        if (e.type == sf::Event::KeyPressed) {
            if (e.key.code == binds_.restart)            resetBoard();
            else if (e.key.code == sf::Keyboard::Escape) next_ = SceneID::Menu;
        }

        /* ---- mouse selection ---- */
        if (e.type == sf::Event::MouseButtonPressed &&
            e.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2i c = winToCell({ float(e.mouseButton.x),
                                         float(e.mouseButton.y) });
            if (valid(c)) { dragging_ = true; a_ = b_ = c; }
        }
        else if (e.type == sf::Event::MouseMoved && dragging_) {
            sf::Vector2i c = winToCell({ float(e.mouseMove.x),
                                         float(e.mouseMove.y) });
            if (valid(c)) b_ = c;
        }
        else if (e.type == sf::Event::MouseButtonReleased &&
                 e.mouseButton.button == sf::Mouse::Left && dragging_)
        {
            dragging_ = false;
            applySelection();
        }
    }

    void update(float dt) override
    {
        /* countdown */
        timeLeft_ -= dt;

        /* log score & exit round once */
        if (timeLeft_ <= 0.f && !recorded_) {
            ScoreStore::append(score_, set_.scores);   // persistent write
            recorded_ = true;
            next_ = SceneID::Menu;
        }

        /* smooth FPS for HUD */
        fpsSmooth_ = 0.9f * fpsSmooth_ + 0.1f * (1.f / dt);
    }

    void draw(sf::RenderWindow& w) override
    {
        /* ---- draw grid ---- */
        sf::RectangleShape tile({ cell_ - 1, cell_ - 1 });
        sf::Text t; t.setFont(font_);
        t.setCharacterSize(static_cast<unsigned>(cell_ * 0.5f));

        for (int r = 0; r < ROWS; ++r)
            for (int c = 0; c < COLS; ++c)
            {
                tile.setPosition(origin_.x + c * cell_,
                                 origin_.y + r * cell_);

                if (alive_[r][c]) {
                    tile.setFillColor(sf::Color(200, 80, 80));
                    w.draw(tile);

                    t.setString(std::to_string(value_[r][c]));
                    sf::FloatRect b = t.getLocalBounds();
                    t.setOrigin(b.left + b.width  / 2.f,
                                b.top  + b.height / 2.f);
                    t.setPosition(tile.getPosition() +
                                  sf::Vector2f(cell_ / 2, cell_ / 2));
                    w.draw(t);
                }
                else {                        // empty / removed
                    tile.setFillColor(sf::Color(40, 40, 40));
                    w.draw(tile);
                }
            }

        /* ---- selection rectangle ---- */
        if (dragging_) {
            sf::Vector2f p0 = origin_ + sf::Vector2f(a_.x * cell_,
                                                     a_.y * cell_);
            sf::Vector2f p1 = origin_ + sf::Vector2f(b_.x * cell_,
                                                     b_.y * cell_);
            sf::Vector2f tl(std::min(p0.x, p1.x), std::min(p0.y, p1.y));
            sf::Vector2f br(std::max(p0.x, p1.x) + cell_,
                            std::max(p0.y, p1.y) + cell_);

            sf::RectangleShape outline(br - tl);
            outline.setPosition(tl);
            outline.setFillColor(sf::Color::Transparent);
            outline.setOutlineThickness(3.f);
            outline.setOutlineColor(sf::Color::Yellow);
            w.draw(outline);
        }

        /* ---- HUD: score + timer ---- */
        sf::Text hud; hud.setFont(font_); hud.setCharacterSize(26);
        int secs = int(timeLeft_ < 0 ? 0 : timeLeft_);
        int mm = secs / 60, ss = secs % 60;
        std::ostringstream oss;
        oss << "Score " << score_ << "   "
            << std::setw(2) << std::setfill('0') << mm << ':'
            << std::setw(2) << ss;
        hud.setString(oss.str());
        hud.setOrigin(hud.getLocalBounds().width, 0);
        hud.setPosition(w.getSize().x - 10.f, 10.f);
        w.draw(hud);

        /* ---- optional FPS ---- */
        if (set_.showFPS) {
            sf::Text ft; ft.setFont(font_); ft.setCharacterSize(18);
            ft.setString(std::to_string(
                              static_cast<int>(fpsSmooth_ + 0.5f)) + " FPS");
            ft.setOrigin(ft.getLocalBounds().width, 0);
            ft.setPosition(w.getSize().x - 10.f, 42.f);
            w.draw(ft);
        }
    }

    /* ------------- scene transition helpers ----------------------- */
    SceneID next()   const override { return next_; }
    void    resetNext()  override  { next_ = SceneID::None; }
};
