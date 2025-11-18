/**
 * @file Entity.hpp
 * 
 * @brief Entity struct representing a game object, renderable with a sprite.
 * 
 * @authors Jacek Zub
 */
#pragma once

#include <SFML/Graphics/Texture.hpp>

#include <memory>

#include "snek/constants.hpp"

namespace snek {

enum class Direction : int32_t {
    Up = 0,
    Right = 1,
    Down = 2,
    Left = 3
};

auto getTextureRect(Direction direction) -> sf::IntRect {
    const auto x = static_cast<int32_t>(direction) * snek::TEXTURE_TILE_SIZE;
    const auto y = 0;

    const auto w = snek::TEXTURE_TILE_SIZE;
    const auto h = snek::TEXTURE_TILE_SIZE;

    return sf::IntRect{{x, y}, {w, h}};
}

struct Entity {
    sf::Vector2f position;
    sf::Vector2f size;
    Direction direction;

    const sf::Texture* texture;
};

} // namespace snek
