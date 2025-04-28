#pragma once
#include "Scene.hpp"
#include "InputBindings.hpp"
#include <random>
#include <iomanip>

class GameScene : public Scene {
    sf::Font&      font_;
    InputBindings& binds_;
    SceneID        next_ = SceneID::None;

    // board
    static constexpr int ROWS = 10, COLS = 17;
    std::vector<std::vector<int>>  value_;
    std::vector<std::vector<bool>> alive_;

    // geometry
    float           cell_;          // px
    sf::Vector2f    origin_;        // upper-left corner of the grid

    // selection
    bool dragging_ = false;
    sf::Vector2i a_, b_;

    // game state
    int   score_    = 0;
    float timeLeft_ = 120.f;            // seconds
    std::mt19937 rng_{std::random_device{}()};

public:
    GameScene(sf::Font& f, InputBindings& b, sf::Vector2u win)
    : font_(f), binds_(b)
    {
        resetBoard();
        recalcGeometry(win);
    }

    /* ---------- helpers ---------- */
    void recalcGeometry(sf::Vector2u win) {
        cell_   = std::min(win.x / float(COLS), win.y / float(ROWS));
        origin_ = { (win.x - cell_*COLS)/2.f, (win.y - cell_*ROWS)/2.f };
    }
    void resetBoard() {
        value_.assign(ROWS, std::vector<int>(COLS));
        alive_.assign(ROWS, std::vector<bool>(COLS,true));
        std::uniform_int_distribution<int> d(1,9);
        for (auto& row : value_)
            for (int& v : row) v = d(rng_);
        score_ = 0; timeLeft_ = 120.f;
    }
    bool valid(sf::Vector2i c) const { return c.x>=0&&c.x<COLS && c.y>=0&&c.y<ROWS; }
    sf::Vector2i winToCell(sf::Vector2f p) const {
        p -= origin_;
        return { int(p.x / cell_), int(p.y / cell_) };
    }
    void applySelection() {
        int r1 = std::min(a_.y,b_.y), r2 = std::max(a_.y,b_.y);
        int c1 = std::min(a_.x,b_.x), c2 = std::max(a_.x,b_.x);

        int sum=0;
        for(int r=r1;r<=r2;++r)
            for(int c=c1;c<=c2;++c)
                if (alive_[r][c]) sum += value_[r][c];

        if (sum==10) {
            ++score_;
            for(int r=r1;r<=r2;++r)
                for(int c=c1;c<=c2;++c)
                    alive_[r][c]=false;
        }
    }

    /* ---------- Scene interface ---------- */
    void handleEvent(const sf::Event& e) override {
        if (e.type==sf::Event::Resized)
            recalcGeometry({e.size.width,e.size.height});

        // keyboard
        if (e.type==sf::Event::KeyPressed) {
            if      (e.key.code == binds_.restart)   resetBoard();
            else if (e.key.code == sf::Keyboard::Escape) next_ = SceneID::Menu;
        }

        // mouse-drag selection
        if (e.type==sf::Event::MouseButtonPressed && e.mouseButton.button==sf::Mouse::Left) {
            sf::Vector2i c = winToCell({float(e.mouseButton.x),float(e.mouseButton.y)});
            if (valid(c)) { dragging_=true; a_=b_=c; }
        }
        else if (e.type==sf::Event::MouseMoved && dragging_) {
            sf::Vector2i c = winToCell({float(e.mouseMove.x),float(e.mouseMove.y)});
            if (valid(c)) b_=c;
        }
        else if (e.type==sf::Event::MouseButtonReleased && e.mouseButton.button==sf::Mouse::Left && dragging_) {
            dragging_=false; applySelection();
        }
    }

    void update(float dt) override {
        timeLeft_ -= dt;
        if (timeLeft_ <= 0.f) next_ = SceneID::Menu;
    }

    void draw(sf::RenderWindow& w) override {
        // --- grid ---
        sf::RectangleShape tile({cell_-1, cell_-1});
        sf::Text t; t.setFont(font_);
        t.setCharacterSize(static_cast<unsigned>(cell_*0.5f));

        for(int r=0;r<ROWS;++r)
            for(int c=0;c<COLS;++c)
            {
                tile.setPosition(origin_.x+c*cell_, origin_.y+r*cell_);
                if (alive_[r][c]) {
                    tile.setFillColor(sf::Color(200,80,80));
                    w.draw(tile);
                    t.setString(std::to_string(value_[r][c]));
                    sf::FloatRect b = t.getLocalBounds();
                    t.setOrigin(b.left+b.width/2.f, b.top+b.height/2.f);
                    t.setPosition(tile.getPosition()+sf::Vector2f(cell_/2,cell_/2));
                    w.draw(t);
                } else {
                    tile.setFillColor(sf::Color(40,40,40));
                    w.draw(tile);
                }
            }

        // selection outline
        if (dragging_) {
            sf::Vector2f p0 = origin_ + sf::Vector2f(a_.x*cell_, a_.y*cell_);
            sf::Vector2f p1 = origin_ + sf::Vector2f(b_.x*cell_, b_.y*cell_);
            sf::Vector2f tl(std::min(p0.x,p1.x), std::min(p0.y,p1.y));
            sf::Vector2f br(std::max(p0.x,p1.x)+cell_, std::max(p0.y,p1.y)+cell_);
            sf::RectangleShape outline(br-tl);
            outline.setPosition(tl);
            outline.setFillColor(sf::Color::Transparent);
            outline.setOutlineThickness(3.f);
            outline.setOutlineColor(sf::Color::Yellow);
            w.draw(outline);
        }

        // --- HUD (score & timer, top-right) ---
        sf::Text hud; hud.setFont(font_); hud.setCharacterSize(26);
        int s = int(timeLeft_); int m=s/60; s%=60;
        std::ostringstream ss; ss<<"Score "<<score_<<"   "
           << std::setw(2)<<std::setfill('0')<<m<<':'<<std::setw(2)<<s;
        hud.setString(ss.str());
        hud.setOrigin(hud.getLocalBounds().width,0);
        hud.setPosition(w.getSize().x - 10.f, 10.f);
        w.draw(hud);
    }

    SceneID next() const override { return next_; }
    void    resetNext()    override { next_ = SceneID::None; }
};
