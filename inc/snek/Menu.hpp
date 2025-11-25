/**
 * @file Menu.hpp
 * 
 * @brief Menu class representing a simple menu system.
 * 
 * @authors Jacek Zub
 */
#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <ranges>

#include "snek/utils.hpp"
#include "snek/ILayer.hpp"

namespace snek {

class Menu final : public ILayer {
public:
    auto update(const InputAction action) -> void override {
        switch (action) {
            case InputAction::Forward:
                if (m_selectedIndex > 0) {
                    m_selectedIndex--;
                }
                break;
            case InputAction::Backward:
                if (m_selectedIndex + 1 < m_items.size()) {
                    m_selectedIndex++;
                }
                break;
            case InputAction::TurnRight:
                if (m_selectedIndex < m_items.size()) {
                    m_items[m_selectedIndex]->select();
                }
                break;
            default:
                break;
        }
    }

    auto render(Renderer& renderer) const -> void override {
        renderer.resetView();

        const auto windowSize = renderer.getWindowSize();
        const sf::Vector2f winSizef{
            static_cast<float>(windowSize.x),
            static_cast<float>(windowSize.y)
        };

        sf::VertexArray background(sf::PrimitiveType::TriangleStrip, 4);
        const sf::Color topColor(6, 18, 28);
        const sf::Color bottomColor(20, 54, 30);

        background[0].position = {0.f, 0.f};               background[0].color = topColor;
        background[1].position = {winSizef.x, 0.f};        background[1].color = topColor;
        background[2].position = {winSizef.x, winSizef.y}; background[2].color = bottomColor;
        background[3].position = {0.f, winSizef.y};        background[3].color = bottomColor;

        renderer.drawDrawable(background);

        const uint32_t characterSize = 44u;
        const float spacing = static_cast<float>(characterSize) + 18.f;
        const float totalHeight = spacing * static_cast<float>(m_items.size());
        const float startY = (winSizef.y - totalHeight) / 2.f + static_cast<float>(characterSize) / 2.f;

        const float panelWidth = std::min(winSizef.x - 80.f, 520.f);
        const float panelHeight = totalHeight + 80.f;
        sf::RectangleShape panel({panelWidth, panelHeight});
        panel.setOrigin({panelWidth / 2.f, panelHeight / 2.f});
        panel.setPosition({winSizef.x / 2.f, winSizef.y / 2.f});
        panel.setFillColor(sf::Color(8, 18, 10, 190));
        panel.setOutlineThickness(3.f);
        panel.setOutlineColor(sf::Color(94, 232, 169, 180));
        renderer.drawDrawable(panel);

        auto& font = renderer.getFont();
        for (size_t i = 0; i < m_items.size(); i++) {
            const auto& item = m_items[i];
            const bool selected = i == m_selectedIndex;
            const float y = startY + static_cast<float>(i) * spacing;

            sf::Text text(font, item->getText(), characterSize);
            text.setStyle(sf::Text::Bold);
            text.setLetterSpacing(1.08f);

            const auto bounds = text.getLocalBounds();
            const float baseWidth = panelWidth * 0.7f;
            const float highlightWidth = std::max(baseWidth, bounds.size.x + 60.f);
            const float highlightHeight = static_cast<float>(characterSize) + 22.f;

            text.setOrigin({
                bounds.position.x + bounds.size.x / 2.f,
                bounds.position.y + bounds.size.y / 2.f});
            text.setPosition({winSizef.x / 2.f, y});
            text.setFillColor(selected ? sf::Color(94, 232, 169) : sf::Color(230, 230, 230));
            text.setOutlineThickness(selected ? 2.f : 1.f);
            text.setOutlineColor(sf::Color(0, 0, 0, 200));

            sf::RectangleShape highlight({highlightWidth, highlightHeight});
            highlight.setOrigin({highlightWidth / 2.f, highlightHeight / 2.f});
            highlight.setPosition({winSizef.x / 2.f, y});
            highlight.setFillColor(selected ? sf::Color(20, 60, 36, 220) : sf::Color(0, 0, 0, 120));
            highlight.setOutlineThickness(selected ? 2.5f : 1.5f);
            highlight.setOutlineColor(selected ? sf::Color(94, 232, 169, 200) : sf::Color(255, 255, 255, 40));

            renderer.drawDrawable(highlight);
            renderer.drawDrawable(text);
        }
    }

    auto addButton(const std::string& text, std::function<void()> onSelect = [](){}) -> void {
        m_items.push_back(
            std::make_unique<Button>(text, onSelect)
        );
    }

    auto addToggle(
        const std::string& text,
        const std::vector<std::string>& options,
        std::function<void(const std::string&)> onSelect
    ) -> void {
        m_items.push_back(
            std::make_unique<Toggle>(text, options, onSelect)
        );
    }
private:
    struct IItem {
        virtual ~IItem() = default;
    
        virtual auto select() -> void = 0;
        virtual auto getText() const -> std::string = 0;
    };

    struct Button final : IItem {
        Button(
            const std::string& text,
            const std::function<void()>& onSelect
        )
            : m_text(text)
            , m_onSelect(onSelect)
        {}

        auto select() -> void override {
            m_onSelect();
        }

        auto getText() const -> std::string override {
            return m_text;
        }

        std::string m_text;
        std::function<void()> m_onSelect;
    };

    struct Toggle final : IItem {
        Toggle(
            const std::string& text,
            const std::vector<std::string>& options,
            const std::function<void(const std::string&)>& onSelect
        )
            : text(text)
            , options(options)
            , onSelect(onSelect)
        {}

        auto select() -> void override {
            selectedIndex = (selectedIndex + 1) % options.size();
            onSelect(options[selectedIndex]);
        }

        auto getText() const -> std::string override {
            return text + ": " + options[selectedIndex];
        }

        std::string text;
        std::vector<std::string> options;
        size_t selectedIndex{0u};
        std::function<void(const std::string&)> onSelect;
    };

    std::vector<std::unique_ptr<IItem>> m_items;
    size_t m_selectedIndex{0u};
}; // class Menu

inline auto createMainMenu(
    ILayer** current_layer,
    ILayer* game_layer,
    ILayer* options_layer,
    sf::Window& window
) -> Menu {
    Menu menu;

    menu.addButton("Start Game", [current_layer, game_layer](){
        *current_layer = game_layer;
    });

    menu.addButton("Options", [current_layer, options_layer](){
        *current_layer = options_layer;
    });

    menu.addButton("Exit", [&window](){
        window.close();
    });

    return menu;
}

inline auto createOptionsMenu(
    ILayer** current_layer,
    ILayer* main_menu_layer,
    sf::Window& window
) -> Menu {
    Menu menu;

    const std::vector<std::pair<std::string, Resolution>> resolution_option_list = {
    {"Fullscreen", Resolution::FULLSCREEN},
    {"800x600", Resolution::SMALL},
    {"1280x960", Resolution ::MEDIUM},
    {"1600x1200", Resolution::LARGE}
    };
    const std::map resolution_options(resolution_option_list.begin(), resolution_option_list.end());
    const std::vector<std::string> resolution_option_names = resolution_option_list
        | std::views::transform([](const auto& p) { return p.first; })
        | std::ranges::to<std::vector>();

    const auto update_resolution = [resolution_options, &window](const std::string& option) {
        sf::VideoMode mode;
        auto state = sf::State::Windowed;

        switch (resolution_options.at(option)) {
            case Resolution::SMALL:
                mode = sf::VideoMode({800, 600}); break;
            case Resolution::MEDIUM:
                mode = sf::VideoMode({1280, 960}); break;
            case Resolution::LARGE:
                mode =  sf::VideoMode({1600, 1200}); break;
            case Resolution::FULLSCREEN:
            default:
                mode = sf::VideoMode::getDesktopMode();
                state = sf::State::Fullscreen;
        }

        createWindow(window, mode, state);
    };

    menu.addToggle(
        "Resolution",
        resolution_option_names,
        update_resolution
    );

    update_resolution(resolution_option_names[0]);

    menu.addButton("Back", [current_layer, main_menu_layer](){
        *current_layer = main_menu_layer;
    });

    return menu;
}

} // namespace snek
