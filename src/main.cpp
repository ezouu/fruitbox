#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "Scene.hpp"
#include "MenuScene.hpp"
#include "ConfigScene.hpp"
#include "GameScene.hpp"
#include "InputBindings.hpp"
#include "Settings.hpp"

/* simple font loader ---------------------------------------------------- */
static sf::Font loadFont() {
    sf::Font f;
    const char* paths[] = {
        "resources/arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/mnt/c/Windows/Fonts/arial.ttf"
    };
    for (auto p : paths) if (f.loadFromFile(p)) return f;
    throw std::runtime_error("Could not load any TTF font.");
}

/* ---------------------------------------------------------------------- */
int main() {
    sf::RenderWindow app(sf::VideoMode(1024, 768),
                         "Fruit Box Local", sf::Style::Default);
    app.setVerticalSyncEnabled(true);

    sf::Font        font = loadFont();
    InputBindings   binds;
    Settings        set;

    std::unique_ptr<Scene> scene = std::make_unique<MenuScene>(font);
    sf::Clock dtClock;

    while (app.isOpen()) {
        float dt = dtClock.restart().asSeconds();

        sf::Event e;
        while (app.pollEvent(e)) {
            if (e.type == sf::Event::Closed) app.close();
            scene->handleEvent(e);
        }
        scene->update(dt);

        SceneID jump = scene->next();
        if (jump != SceneID::None) {
            switch (jump) {
                case SceneID::Menu:
                    scene = std::make_unique<MenuScene>(font); break;
                case SceneID::Config:
                    scene = std::make_unique<ConfigScene>(font, binds, set); break;
                case SceneID::Game:
                    scene = std::make_unique<GameScene>(font, binds, set, app.getSize()); break;
                case SceneID::Exit:
                    app.close(); break;
                default: break;
            }
        }

        app.clear(sf::Color::Black);
        scene->draw(app);
        app.display();
    }
}
