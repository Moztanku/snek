/**
 * @file Input.hpp
 * 
 * @brief Input handling related definitions.
 * 
 * @authors Jacek Zub
 */
#pragma once

#include <SFML/Window/Window.hpp>

#include "snek/constants.hpp"

namespace snek {

enum class InputAction {
    Forward,
    Backward,
    TurnLeft,
    TurnRight,
    Exit,
    None
};

auto poll_events(sf::Window& window) -> InputAction {
    using Closed = sf::Event::Closed;
    using KeyPressed = sf::Event::KeyPressed;
    using sf::Keyboard::Key;

    while (const auto event = window.pollEvent()) {
        if (event->is<Closed>()) {
            window.close();

            return InputAction::Exit;
        }
        else if (event->is<KeyPressed>()) {
            const auto& key_code = event->getIf<KeyPressed>()->code;

            switch (key_code) {
                case Key::W:
                case Key::Up:
                    return InputAction::Forward;
                case Key::S:
                case Key::Down:
                    return InputAction::Backward;
                case Key::A:
                case Key::Left:
                    return InputAction::TurnLeft;
                case Key::D:
                case Key::Right:
                    return InputAction::TurnRight;
                case Key::Escape:
                    window.close();
                    return InputAction::Exit;
                default:
                    break;
            }
        }
    }

    return InputAction::None;
}

} // namespace snek
