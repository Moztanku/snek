//
// Created by adame on 10/19/2025.
//

#ifndef EVENTS_H
#define EVENTS_H

#include <SFML/System/Vector2.hpp>

struct MoveGameEvent {
    sf::Vector2i move_vector;
};
struct PauseGameEvent {};
struct WindowClosedEvent {};
struct GridClickEvent {
    sf::Vector2i pixelPosition;
};

#endif //EVENTS_H
