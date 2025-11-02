/**
 * @file main.cpp
 * 
 * @brief Starting point of the snek game, contains the main loop.
 * 
 * @authors Jacek Zub
 */
#include <SFML/Window.hpp>

#include "snek/constants.hpp"
#include "snek/Entity.hpp"
#include "snek/TextureManager.hpp"
#include "snek/Renderer.hpp"
#include "snek/Snake.hpp"
#include "snek/Board.hpp"
#include "snek/Input.hpp"
#include "snek/Menu.hpp"

auto main() -> int32_t {
    auto window = sf::RenderWindow(
        sf::VideoMode({snek::WINDOW_WIDTH, snek::WINDOW_HEIGHT}),
        snek::WINDOW_TITLE);

    window.setFramerateLimit(snek::FRAMERATE_LIMIT);

    snek::Renderer renderer{window};

    snek::Board board;
    snek::Menu main_menu;
    snek::Menu options_menu;
    snek::ILayer* current_layer = &main_menu;

    main_menu = snek::createMainMenu(
        &current_layer,
        &board,
        &options_menu,
        window
    );
    options_menu = snek::createOptionsMenu(
        &current_layer,
        &main_menu
    );

    while (window.isOpen()) {
        const auto action = snek::poll_events(window);

        current_layer->update(action);

        renderer.beginFrame();

        current_layer->render(renderer);

        if (current_layer == &board) {
            std::vector<std::string> debug_lines;
            for (const auto* entity : board.getEntities()) {
                std::string line = 
                    "Pos: (" + std::to_string(static_cast<int32_t>(entity->position.x)) + ", " +
                    std::to_string(static_cast<int32_t>(entity->position.y)) + ") Dir: " +
                    std::to_string(static_cast<int32_t>(entity->direction));

                debug_lines.push_back(std::move(line));
            }
            renderer.debugText(debug_lines);
        }

        renderer.endFrame();
    }

    return 0;
}
