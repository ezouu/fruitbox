#pragma once
#include <SFML/Graphics.hpp>

enum class SceneID { None, Menu, Game, Config, Exit };

class Scene {
public:
    virtual ~Scene() = default;
    virtual void handleEvent(const sf::Event& e) = 0;
    virtual void update(float dt)                = 0;
    virtual void draw(sf::RenderWindow& w)       = 0;

    virtual SceneID next()  const = 0;   // request a transition
    virtual void    resetNext()    = 0;   // clear that request
};

