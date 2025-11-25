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

// Window related
constexpr uint32_t WINDOW_WIDTH = 800u;
constexpr uint32_t WINDOW_HEIGHT = 600u;
constexpr char WINDOW_TITLE[] = "Snek Game";
constexpr uint32_t FRAMERATE_LIMIT = 60u;

// Game related
constexpr float TILE_SIZE = 32.f;
constexpr float SNAKE_INITIAL_SPEED = 4 * (TILE_SIZE / FRAMERATE_LIMIT); // 4 tiles per second
constexpr float SNAKE_SPEED_INCREMENT = 0.5f * (TILE_SIZE / FRAMERATE_LIMIT); // increase speed by 0.5 tiles per second
constexpr uint32_t SNAKE_INITIAL_LENGTH = 5u;

// Rendering related
constexpr int32_t TEXTURE_TILE_SIZE = 64;



// Texture paths
constexpr std::string PATH_PREFIX = "";
constexpr std::string RESPATH_TEST_BMP = PATH_PREFIX + "res/test.bmp";
constexpr std::string RESPATH_ARIAL_TTF = PATH_PREFIX + "res/arial.ttf";


// Enum resolutions
enum class Resolution {
    FULLSCREEN,
    DEFAULT = FULLSCREEN,
    SMALL,
    MEDIUM,
    LARGE,
};


} // namespace snek
