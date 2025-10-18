//
// Created by adame on 10/18/2025.
//

#ifndef TILE_H
#define TILE_H

enum class content_type {
    EMPTY,
    WALL,
    FOOD,
    SNAKE,
};


class Tile {
    content_type content_type;
    content_ref content_ref;
};



#endif //TILE_H
