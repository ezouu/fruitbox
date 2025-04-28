#pragma once
#include <SFML/Graphics.hpp>

class Button {
    sf::RectangleShape box_;
    sf::Text           label_;
public:
    Button(const sf::Font& f,
           const std::string& txt,
           sf::Vector2f pos,
           sf::Vector2f size)
    {
        box_.setPosition(pos);
        box_.setSize(size);
        box_.setFillColor(sf::Color(70,70,70));
        box_.setOutlineColor(sf::Color::White);
        box_.setOutlineThickness(2.f);

        label_.setFont(f);
        label_.setString(txt);
        label_.setCharacterSize(static_cast<unsigned>(size.y*0.55f));
        sf::FloatRect b = label_.getLocalBounds();
        label_.setOrigin(b.left+b.width/2.f, b.top+b.height/2.f);
        label_.setPosition(pos + size/2.f);
    }
    bool contains(sf::Vector2f p) const { return box_.getGlobalBounds().contains(p); }
    void draw(sf::RenderWindow& w)      { w.draw(box_); w.draw(label_); }
};
