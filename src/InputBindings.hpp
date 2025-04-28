#pragma once
#include <SFML/Window/Keyboard.hpp>

struct InputBindings {
    sf::Keyboard::Key restart     = sf::Keyboard::Tab;
    sf::Keyboard::Key selectHold  = sf::Keyboard::Space;
};
inline std::string keyName(sf::Keyboard::Key k) {
    using K = sf::Keyboard;
    if (k>=K::A && k<=K::Z)   return std::string(1, char('A'+k-K::A));
    if (k>=K::Num0 && k<=K::Num9) return std::string(1, char('0'+k-K::Num0));
    switch (k) {
        case K::Space: return "Space";
        case K::Enter: return "Enter";
        case K::Tab:   return "Tab";
        case K::Escape:return "Esc";
        default:       return "Key" + std::to_string(static_cast<int>(k));
    }
}