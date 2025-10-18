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

constexpr uint32_t WINDOW_WIDTH = 800u;
constexpr uint32_t WINDOW_HEIGHT = 600u;
constexpr char WINDOW_TITLE[] = "Snek Game";

constexpr uint32_t FRAMERATE_LIMIT = 60u;

} // namespace snek