#pragma once
#include <sstream>
#include <SFML/Graphics.hpp>
#include "Scene.hpp"
#include "Button.hpp"
#include "InputBindings.hpp"
#include "Settings.hpp"

class ConfigScene : public Scene {
    sf::Font&        font_;
    InputBindings&   binds_;
    Settings&        set_;
    Button           back_;
    SceneID          next_ = SceneID::None;

    enum class WaitFor { None, Restart, Select };
    WaitFor waiting_ = WaitFor::None;
    sf::Text text_;

    void refreshText() {
        std::ostringstream ss;
        ss << "Key bindings / options\n\n"
           << "[1] Restart:       " << keyName(binds_.restart)     << '\n'
           << "[2] Select / hold: " << keyName(binds_.selectHold)  << '\n'
           << "[3] Show FPS:      " << (set_.showFPS ? "On" : "Off") << '\n';
        if (waiting_ != WaitFor::None) ss << "\nPress a key...";
        text_.setString(ss.str());
    }

public:
    ConfigScene(sf::Font& f, InputBindings& b, Settings& s)
        : font_(f), binds_(b), set_(s),
          back_(f, "Back", {60.f, 60.f}, {180.f, 60.f})
    {
        text_.setFont(font_);
        text_.setCharacterSize(22);
        text_.setPosition(60.f, 150.f);
        refreshText();
    }

    void handleEvent(const sf::Event& e) override {
        if (e.type == sf::Event::MouseButtonPressed &&
            e.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f p{float(e.mouseButton.x), float(e.mouseButton.y)};
            if (back_.contains(p)) { next_ = SceneID::Menu; return; }

            float y = p.y - text_.getPosition().y;
            if      (y >= 56  && y < 84)  { waiting_ = WaitFor::Restart; refreshText(); }
            else if (y >= 84  && y < 112) { waiting_ = WaitFor::Select;  refreshText(); }
            else if (y >= 112 && y < 140) { set_.showFPS = !set_.showFPS; refreshText(); }
        }
        else if (e.type == sf::Event::KeyPressed && waiting_ != WaitFor::None) {
            if (waiting_ == WaitFor::Restart)     binds_.restart    = e.key.code;
            else                                  binds_.selectHold = e.key.code;
            waiting_ = WaitFor::None;
            refreshText();
        }
    }

    void update(float) override {}
    void draw(sf::RenderWindow& w) override { back_.draw(w); w.draw(text_); }

    SceneID next() const override { return next_; }
    void    resetNext()  override { next_ = SceneID::None; }
};

