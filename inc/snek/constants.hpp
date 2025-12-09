/**
 * @file snek/constants.hpp
 * 
 * @brief Constants used throughout the project.
 * 
 * @authors Jacek Zub
 */
#pragma once

#include <cstdint>
#include <string_view>

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

// Path prefix
#define PATH_PREFIX "res/"

// Texture paths
constexpr std::string_view RESPATH_TEST_BMP = PATH_PREFIX "test.bmp";
constexpr std::string_view RESPATH_ARIAL_TTF = PATH_PREFIX "arial.ttf";

// Sound paths
constexpr std::string_view RESPATH_OPTION_WAV = PATH_PREFIX "menu_opcje.wav";
constexpr std::string_view RESPATH_CONFIRM_WAV = PATH_PREFIX "menu_potwierdzenie.wav";
constexpr std::string_view RESPATH_TURN_WAV = PATH_PREFIX "efekt_skret.wav";
constexpr std::string_view RESPATH_EAT_WAV = PATH_PREFIX "efekt_jedzenia.wav";
constexpr std::string_view RESPATH_DEATH_WAV = PATH_PREFIX "efekt_smierc.wav";

#undef PATH_PREFIX

// Enum resolutions
enum class Resolution {
    FULLSCREEN,
    SMALL,
    MEDIUM,
    LARGE,
    DEFAULT = SMALL,
};


} // namespace snek
