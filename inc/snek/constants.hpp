/**
 * @file snek/constants.hpp
 * 
 * @brief Constants used throughout the project.
 * 
 * @authors Jacek Zub
 */
#pragma once

#include <cstdint>

namespace snek {

struct Vector3d { int x, y, z; };
struct Vector2d { int x, y; };
struct Vector3f { float x, y, z; };
struct Vector2f { float x, y; };

constexpr uint32_t WINDOW_WIDTH = 1200u;
constexpr uint32_t WINDOW_HEIGHT = 900u;
constexpr char WINDOW_TITLE[] = "Snek Game";

constexpr uint32_t FRAMERATE_LIMIT = 60u;
constexpr Vector3d tile_size{ 50, 50, 50 }; // TODO maybe 2d vector is enough?
} // namespace snek