/**
 * @file Renderer.hpp
 * 
 * @brief Renderer class responsible for drawing entities on the screen. 2D now.
 * 
 * @authors Jacek Zub
 */
#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include <print>

#include "snek/constants.hpp"
#include "snek/Entity.hpp"

namespace snek {

class Renderer {
public:
    Renderer(sf::RenderWindow& window)
        : m_window(window)
    {}

    auto setView(const sf::View& view) -> void {
        m_window.setView(view);
    }

    auto draw(const Entity* entity) -> void {
        if (entity->texture == nullptr) {
            std::println(stderr, "Entity has no texture, cannot draw.");

            return;
        }

        sf::Sprite sprite(*entity->texture);

        sprite.setTextureRect(getTextureRect(entity->direction));
        sprite.setOrigin({
            entity->size.x / 2.f,
            entity->size.y / 2.f
        });
        sprite.setPosition(entity->position);

        m_window.draw(sprite);
    }

    struct DrawTextProps {
        const std::string& text;
        sf::Vector2f position{5.f, 5.0f};
        uint32_t characterSize{20u};
        sf::Color fillColor{sf::Color::White};
    };
    auto drawText(const DrawTextProps& props) -> void {
        static std::unique_ptr<sf::Font> font{nullptr};

        if (!font) {
            font = std::make_unique<sf::Font>("res/arial.ttf");
        }

        sf::Text text(*font, props.text);

        text.setCharacterSize(props.characterSize);
        text.setFillColor(props.fillColor);
        text.setPosition(props.position);

        m_window.draw(text);
    }

    auto debugText(const std::vector<std::string>& lines) -> void {
        static std::unique_ptr<sf::Font> font{nullptr};

        if (!font) {
            font = std::make_unique<sf::Font>("res/arial.ttf");
        }

        float y_offset = 5.f;
        const float x_offset = 5.f;

        for (const auto& line : lines) {
            sf::Text text(*font, line);

            text.setCharacterSize(20);
            text.setFillColor(sf::Color::White);
            text.setPosition({x_offset, y_offset});

            m_window.draw(text);

            y_offset += 16.f;
        }
    }

    auto beginFrame() -> void {
        m_window.clear(sf::Color::Black);
    }

    auto endFrame() -> void {
        m_window.display();
    }
private:
    sf::RenderWindow& m_window;
}; // class Renderer

} // namespace snek
