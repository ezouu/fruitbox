#pragma once
#include "Scene.hpp"
#include "Button.hpp"

class MenuScene : public Scene {
    sf::Font& font_;
    Button    play_, config_;
    SceneID   next_ = SceneID::None;

public:
    MenuScene(sf::Font& f)
    : font_(f),
      play_  (f, "Play",   {60.f, 60.f}, {220.f, 70.f}),
      config_(f, "Config", {60.f,160.f}, {220.f, 70.f}) {}

    void handleEvent(const sf::Event& e) override {
        if (e.type==sf::Event::MouseButtonPressed && e.mouseButton.button==sf::Mouse::Left) {
            sf::Vector2f p{float(e.mouseButton.x), float(e.mouseButton.y)};
            if      (play_.contains(p))   next_ = SceneID::Game;
            else if (config_.contains(p)) next_ = SceneID::Config;
        }
    }
    void update(float) override {}
    void draw(sf::RenderWindow& w) override { play_.draw(w); config_.draw(w); }

    SceneID next() const override { return next_; }
    void    resetNext()    override { next_ = SceneID::None; }
};
