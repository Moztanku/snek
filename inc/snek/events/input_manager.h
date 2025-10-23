//
// Created by adame on 10/19/2025.
//

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <SFML/Window.hpp>
#include "snek/events/event_bus.h"
#include "snek/events/events.h"

/** maps user input to semantic events */
class InputManager {
public:
    explicit InputManager(EventBus& bus)
        : event_bus(bus) {
    }
    ~InputManager() = default;

    void handle_event(const sf::Event& event) const {
        if (event.is<sf::Event::Closed>()) {
            event_bus.publish("PAUSE_GAME", PauseGameEvent{});
            event_bus.publish("WINDOW_CLOSED", WindowClosedEvent{});
        }
        else if (const auto& mouse_event = event.getIf<sf::Event::MouseButtonPressed>()) {
            const auto& button = mouse_event->button;
            const auto& position = mouse_event->position;
            if (button == sf::Mouse::Button::Left) {
                event_bus.publish("LEFT_CLICK", GridClickEvent{position});
            }
        }
        else if (const auto& key_press_event = event.getIf<sf::Event::KeyPressed>()) {
            if (key_press_event->code == sf::Keyboard::Key::Escape) {
                event_bus.publish("PAUSE_GAME", PauseGameEvent{});
            }
            else if (key_press_event->code == sf::Keyboard::Key::W) {
                event_bus.publish("MOVE_KEY_PRESSED", MoveGameEvent({0, 1 }));
            }
            else if (key_press_event->code == sf::Keyboard::Key::S) {
                event_bus.publish("MOVE_KEY_PRESSED", MoveGameEvent({0, -1}));
            }
            else if (key_press_event->code == sf::Keyboard::Key::A) {
                event_bus.publish("MOVE_KEY_PRESSED", MoveGameEvent({-1, 0}));
            }
            else if (key_press_event->code == sf::Keyboard::Key::D) {
                event_bus.publish("MOVE_KEY_PRESSED", MoveGameEvent({1, 0}));
            }
            else {
                std::println("Key pressed: {}", static_cast<int>(key_press_event->code));
            }
        }
    }

private:
    EventBus& event_bus;
};

#endif //INPUT_MANAGER_H
