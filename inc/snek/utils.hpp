/**
 * @file utils.hpp
 * 
 * @brief Utility functions for the snek game.
 * 
 * @authors Jacek Zub
 */
#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "snek/constants.hpp"

namespace snek {

constexpr auto checkCollision(
    sf::FloatRect a,
    sf::FloatRect b
) -> bool {
    const auto aleft = a.position.x ; const auto bleft = b.position.x;
    const auto atop  = a.position.y ; const auto btop  = b.position.y;
    const auto awidth  = a.size.x   ; const auto bwidth  = b.size.x;
    const auto aheight = a.size.y   ; const auto bheight = b.size.y;

    return !(aleft + awidth <= bleft ||
             aleft >= bleft + bwidth ||
             atop + aheight <= btop ||
             atop >= btop + bheight);
}

inline auto createWindow(
    sf::Window& window,
    const sf::VideoMode& mode = sf::VideoMode{{WINDOW_WIDTH, WINDOW_HEIGHT}},
    const sf::State state = sf::State::Fullscreen
) -> void {
    window.create(
        mode,
        WINDOW_TITLE,
        sf::Style::Titlebar | sf::Style::Close,
        state
    );
    window.setFramerateLimit(snek::FRAMERATE_LIMIT);

    auto dm = sf::VideoMode::getDesktopMode().size;
    auto ip = window.getPosition();
    auto ws = window.getSize();

    int posX = (ip.x + ws.x) - dm.x / 2 - ws.x / 2;
    posX = dm.x / 2 - ws.x / 2;
    int posY = (ip.y + ws.y) - dm.y / 2 - ws.y / 2;
    posY = dm.y / 2 - ws.y / 2;

    window.setPosition({
        posX,
        posY
    });
}

} // namespace snek
