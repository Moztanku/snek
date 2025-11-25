/**
 * @file Board.hpp
 * 
 * @brief Board class managing the game board and its entities.
 */
#pragma once

#include <vector>
#include <random>

#include "snek/ILayer.hpp"
#include "snek/utils.hpp"
#include "snek/Snake.hpp"
#include "snek/Input.hpp"

namespace snek {

class Board : public ILayer {
public:
    Board() {
        // Initial fruit spawn
        spawnFruit();
    }

    enum class State {
        Playing,
        Paused,
        GameOver
    };

    auto update(InputAction action) -> void override {
        if (m_state != State::Playing) {
            return;
        }

        switch (action) {
            case InputAction::TurnLeft:
                m_snake.turnLeft();
                break;
            case InputAction::TurnRight:
                m_snake.turnRight();
                break;
            default:
                break;
        }

        m_snake.move();

        handle_collision();
    }

    auto render(Renderer& renderer) const -> void override {
        const sf::Vector2f BOARD_SIZE = {
            m_width * snek::TILE_SIZE,
            m_height * snek::TILE_SIZE
        };

        sf::View view(BOARD_SIZE / 2.f, BOARD_SIZE);

        renderer.setView(view);

        for (const auto* entity : getEntities()) {
            renderer.draw(entity);
        }
    }

    auto getState() const -> State {
        return m_state;
    }

    auto getEntities() const -> std::vector<const Entity*> {
        std::vector<const Entity*> entities;

        const auto snake_entities = m_snake.getEntities();

        entities.reserve(snake_entities.size() + m_fruits.size());

        entities.insert(
            entities.end(),
            snake_entities.begin(),
            snake_entities.end());

        for (const auto& fruit : m_fruits) {
            entities.push_back(&fruit);
        }

        return entities;
    }
private:
    // Board properties
    State m_state{State::Playing};

    uint32_t m_width{40u};
    uint32_t m_height{30u};

    // Entities
    Snake m_snake;
    std::vector<Entity> m_fruits;
    std::vector<Entity> m_rocks;

    auto spawnFruit() -> void {
        static std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<uint32_t> dist(0u, m_width * m_height - 1u);

        uint32_t idx;
        Entity fruit;

        const auto collidesWithEntity = [&]() -> bool {
            for (const auto* entity : getEntities()) {
                if (checkCollision({fruit.position, fruit.size}, {entity->position, entity->size})) {
                    return true;
                }
            }

            return false;
        };

        do {
            idx = dist(rng);

            fruit.position = {
                (idx % m_width) * snek::TILE_SIZE + snek::TILE_SIZE / 2.f,
                (idx / m_width) * snek::TILE_SIZE + snek::TILE_SIZE / 2.f
            };
            fruit.size = {snek::TILE_SIZE, snek::TILE_SIZE};
        } while (collidesWithEntity());


        fruit.direction = Direction::Up; // fruits don't have direction, but set to Up by default
        fruit.texture = TextureManager::getTexture(snek::RESPATH_TEST_BMP);

        m_fruits.push_back(std::move(fruit));
    }

    auto handle_collision() -> void {
        const auto& head = *m_snake.getEntities().front();

        for (auto it = m_fruits.begin(); it != m_fruits.end(); ) {
            const sf::Rect head_collision = {head.position, head.size};

            if (checkCollision(head_collision, {it->position, it->size})) {
                m_snake.grow();
                it = m_fruits.erase(it);
                spawnFruit();
            } else {
                ++it;
            }
        }

        for (const auto* entity : m_snake.getEntities() | std::views::drop(2)) {
            // collision is shrunken a bit
            constexpr float COLLISION_SHRINK_FACTOR = 0.2f;
            const sf::Rect head_collision = {
                head.position + head.size * COLLISION_SHRINK_FACTOR / 2.f,
                head.size * (1.f - COLLISION_SHRINK_FACTOR)
            };

            if (checkCollision(head_collision, {entity->position, entity->size})) {
                m_state = State::GameOver;

                return;
            }
        }
    }
}; // class Board

} // namespace snek
