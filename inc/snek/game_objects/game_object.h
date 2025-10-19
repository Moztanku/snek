//
// Created by adame on 10/18/2025.
//

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <SFML/Graphics.hpp>

/** everything that can lock up space of a surface's tile */
enum class object_type {
    EMPTY,
    WALL,
    FOOD,
    SNAKE,
};

/** everything that has a texture and position in the game world */
class game_object {
public:
    const object_type type;
    sf::Texture& texture; // TODO rethink how to manage texture memory

    game_object(const object_type type, sf::Texture& texture)
        : type(type), texture(texture) {};
    ~game_object();
};

#endif //GAME_OBJECT_H
