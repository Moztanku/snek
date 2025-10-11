/**
 * @file main.cpp
 * 
 * @brief Starting point of the snek game, contains the main loop.
 * 
 * @authors Jacek Zub
 */
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <print>
#include <cstdint>

#include "snek/constants.hpp"

auto pool_events(sf::Window& window) -> void;
auto draw_something(sf::RenderWindow& window) -> void;

auto main() -> int32_t {
    auto window = sf::RenderWindow(
        sf::VideoMode({snek::WINDOW_WIDTH, snek::WINDOW_HEIGHT}),
        snek::WINDOW_TITLE);

    window.setFramerateLimit(snek::FRAMERATE_LIMIT);

    while (window.isOpen()) {
        pool_events(window);

        window.clear(sf::Color::Black);

        draw_something(window);

        window.display();
    }

    return 0;
}

auto pool_events(sf::Window& window) -> void {
    while (const auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        else if (event->is<sf::Event::KeyPressed>()) {
            const auto& key_code = event->getIf<sf::Event::KeyPressed>()->code;

            if (key_code == sf::Keyboard::Key::Escape) {
                window.close();
            }
            else {
                std::println("Key pressed: {}", static_cast<int>(key_code));
            }
        }
    }
}

auto draw_something(sf::RenderWindow& window) -> void {
    sf::RectangleShape rect;

    rect.setSize({400.f, 300.f});
    rect.setOrigin({
        200.f,
        150.f
    });
    rect.setPosition({
        snek::WINDOW_WIDTH / 2.f,
        snek::WINDOW_HEIGHT / 2.f
    });
    rect.setFillColor(sf::Color::Green);

    window.draw(rect);
}
