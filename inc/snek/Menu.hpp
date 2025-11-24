/**
 * @file Menu.hpp
 * 
 * @brief Menu class representing a simple menu system.
 * 
 * @authors Jacek Zub
 */
#pragma once

#include <functional>

#include "snek/utils.hpp"
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
                    m_items[m_selectedIndex]->select();
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
                .text = item->getText(),
                .position = {50.f, 50.f + static_cast<float>(i) * 30.f},
                .characterSize = 24u,
                .fillColor = (i == m_selectedIndex) ? sf::Color::Yellow : sf::Color::White
            };

            renderer.drawText(props);
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

    struct Button : IItem {
        Button(
            const std::string& text,
            std::function<void()> onSelect
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

    struct Toggle : IItem {
        Toggle(
            const std::string& text,
            const std::vector<std::string>& options,
            std::function<void(const std::string&)> onSelect
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

auto createMainMenu(
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

auto createOptionsMenu(
    ILayer** current_layer,
    ILayer* main_menu_layer,
    sf::Window& window
) -> Menu {
    Menu menu;

    menu.addToggle(
        "Resolution",
        {"800x600", "1024x768", "1280x960", "1440x1080", "1600x1200"},
        [&window](const std::string& option) {
            const sf::VideoMode mode =
                option == "800x600" ? sf::VideoMode({800, 600}) :
                option == "1024x768" ? sf::VideoMode({1024, 768}) :
                option == "1280x960" ? sf::VideoMode({1280, 960}) :
                option == "1440x1080" ? sf::VideoMode({1440, 1080}) :
                sf::VideoMode({1600, 1200});

                createWindow(window, mode);
        }
    );

    menu.addButton("Back", [current_layer, main_menu_layer](){
        *current_layer = main_menu_layer;
    });

    return menu;
}

} // namespace snek
