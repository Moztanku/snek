/**
 * @file ILayer.hpp
 * 
 * @brief Interface for a layer in the game's rendering and input system.
 */
#pragma once

#include "snek/Input.hpp"
#include "snek/Renderer.hpp"

namespace snek {

struct ILayer {
    virtual ~ILayer() = default;

    virtual auto update(InputAction action) -> void = 0;
    virtual auto render(Renderer& renderer) const -> void = 0;
};

} // namespace snek
