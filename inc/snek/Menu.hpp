/**
 * @file Menu.hpp
 * 
 * @brief Menu class representing a simple menu system.
 * 
 * @authors Jacek Zub
 */
#pragma once

#include <functional>

#include "snek/ILayer.hpp"

namespace snek {

class Menu : public ILayer {
public:
    auto update(InputAction action) -> void override {
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
                    m_items[m_selectedIndex].onSelect();
                }
                break;
            default:
                break;
        }
    }

    auto render(Renderer& renderer) const -> void override {
        for (size_t i = 0; i < m_items.size(); i++) {
            const auto& item = m_items[i];

            Renderer::DrawTextProps props{
                .text = item.text,
                .position = {50.f, 50.f + static_cast<float>(i) * 30.f},
                .characterSize = 24u,
                .fillColor = (i == m_selectedIndex) ? sf::Color::Yellow : sf::Color::White
            };

            renderer.drawText(props);
        }
    }

    auto addItem(const std::string& text, std::function<void()> onSelect = [](){}) -> void {
        m_items.push_back({text, onSelect});
    }
private:
    struct MenuItem {
        std::string text;
        std::function<void()> onSelect;
    };

    std::vector<MenuItem> m_items;
    size_t m_selectedIndex{0u};
}; // class Menu

auto createMainMenu(
    ILayer** current_layer,
    ILayer* game_layer,
    ILayer* options_layer,
    sf::Window& window
) -> Menu {
    Menu menu;

    menu.addItem("Start Game", [current_layer, game_layer](){
        *current_layer = game_layer;
    });

    menu.addItem("Options", [current_layer, options_layer](){
        *current_layer = options_layer;
    });

    menu.addItem("Exit", [&window](){
        window.close();
    });

    return menu;
}

auto createOptionsMenu(
    ILayer** current_layer,
    ILayer* main_menu_layer
) -> Menu {
    Menu menu;

    menu.addItem("Back", [current_layer, main_menu_layer](){
        *current_layer = main_menu_layer;
    });

    return menu;
}

} // namespace snek
