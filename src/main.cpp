#include <SFML/Graphics.hpp>
#include <iostream>
#include "Scene.hpp"
#include "MenuScene.hpp"
#include <memory>   
#include "ConfigScene.hpp"
#include "GameScene.hpp"
#include "InputBindings.hpp"

/* Graceful font loader --------------------------------------------------- */
static sf::Font loadFont()
{
    sf::Font f;
    const char* paths[] = {
        "resources/arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/mnt/c/Windows/Fonts/arial.ttf"
    };
    for (auto p: paths) if (f.loadFromFile(p)) return f;
    throw std::runtime_error("Could not load any TTF font.");
}

/* ------------------------------------------------------------------------ */
int main()
{
    sf::RenderWindow app(sf::VideoMode(1024,768), "Fruit Box Local",
                         sf::Style::Default);
    app.setVerticalSyncEnabled(true);

    sf::Font        font   = loadFont();
    InputBindings   binds;
    std::unique_ptr<Scene> scene = std::make_unique<MenuScene>(font);

    sf::Clock clk;
    while (app.isOpen())
    {
        float dt = clk.restart().asSeconds();

        /* events */
        sf::Event e;
        while (app.pollEvent(e))
        {
            if (e.type==sf::Event::Closed) app.close();
            scene->handleEvent(e);
        }
        scene->update(dt);

        /* scene change? */
        SceneID jump = scene->next();
        if (jump != SceneID::None)
        {
            switch (jump)
            {
                case SceneID::Menu:   scene.reset(new MenuScene(font));                     break;
                case SceneID::Config: scene.reset(new ConfigScene(font, binds));           break;
                case SceneID::Game:   scene.reset(new GameScene(font, binds, app.getSize())); break;
                case SceneID::Exit:   app.close();                                         break;
                default: break;
            }
        }

        app.clear(sf::Color::Black);
        scene->draw(app);
        app.display();
    }
}
