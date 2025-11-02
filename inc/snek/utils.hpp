/**
 * @file utils.hpp
 * 
 * @brief Utility functions for the snek game.
 * 
 * @authors Jacek Zub
 */
#pragma once

#include <SFML/Graphics/Rect.hpp>

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

} // namespace snek
