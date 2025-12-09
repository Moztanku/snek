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
#include "snek/SoundSystem.hpp"

namespace snek {

class Board : public ILayer {
public:
    Board() {
        // Initial fruit spawn
        spawnFruit();

        // Create some rocks
        createRocks(10u);
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
                if (m_snake.turnLeft()) {
                    SoundSystem::Play(RESPATH_TURN_WAV);
                }
                break;
            case InputAction::TurnRight:
                if (m_snake.turnRight()) {
                    SoundSystem::Play(RESPATH_TURN_WAV);
                }
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

        for (const auto& rock : m_rocks) {
            entities.push_back(&rock);
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
        fruit.texture = TextureManager::getTexture("res/assets/snake_sprites.png");
        fruit.textureIndex = 2u;
        fruit.rotationOffsetDegrees = 90.f;

        m_fruits.push_back(std::move(fruit));
    }

    auto createRocks(const uint32_t count) -> void {
        static std::mt19937 rng(std::random_device{}());

        std::vector<bool> occupied(m_width * m_height, false);

        for (const auto* entity : getEntities()) {
            const uint32_t x = static_cast<uint32_t>(entity->position.x) / static_cast<uint32_t>(snek::TILE_SIZE);
            const uint32_t y = static_cast<uint32_t>(entity->position.y) / static_cast<uint32_t>(snek::TILE_SIZE);

            occupied[y * m_width + x] = true;
        }

        // Check for 9-cell availability
        const auto isAvailable = [&](uint32_t x, uint32_t y) -> bool {
            for (int32_t dy = -1; dy <= 1; dy++) {
                for (int32_t dx = -1; dx <= 1; dx++) {
                    const int32_t nx = static_cast<int32_t>(x) + dx;
                    const int32_t ny = static_cast<int32_t>(y) + dy;

                    if (nx >= 0 && nx < static_cast<int32_t>(m_width) &&
                        ny >= 0 && ny < static_cast<int32_t>(m_height)) {
                        if (occupied[ny * m_width + nx]) {
                            return false;
                        }
                    }
                }
            }

            return true;
        };

        uint32_t placed = 0;
        while (placed < count) {
            std::uniform_int_distribution<uint32_t> dist(0u, m_width * m_height - 1u);
            const uint32_t idx = dist(rng);
            const uint32_t x = idx % m_width;
            const uint32_t y = idx / m_width;

            if (isAvailable(x, y)) {
                Entity rock;
                rock.position = {
                    x * snek::TILE_SIZE + snek::TILE_SIZE / 2.f,
                    y * snek::TILE_SIZE + snek::TILE_SIZE / 2.f
                };
                rock.size = {snek::TILE_SIZE, snek::TILE_SIZE};
                rock.direction = Direction::Up; // rocks don't have direction, but set to Up by default
                rock.texture = TextureManager::getTexture("res/assets/snake_sprites.png");
                rock.textureIndex = 3u;

                std::uniform_real_distribution<float> rotation_dist(0.f, 360.f);
                rock.rotationOffsetDegrees = rotation_dist(rng);

                m_rocks.push_back(std::move(rock));

                // Mark 9 cells as occupied
                for (int32_t dy = -1; dy <= 1; dy++) {
                    for (int32_t dx = -1; dx <= 1; dx++) {
                        const int32_t nx = static_cast<int32_t>(x) + dx;
                        const int32_t ny = static_cast<int32_t>(y) + dy;

                        if (nx >= 0 && nx < static_cast<int32_t>(m_width) &&
                            ny >= 0 && ny < static_cast<int32_t>(m_height)) {
                            occupied[ny * m_width + nx] = true;
                        }
                    }
                }

                placed++;
            }
        }
    }

    auto handle_collision() -> void {
        const auto& head = *m_snake.getEntities().front();

        for (auto it = m_fruits.begin(); it != m_fruits.end(); ) {
            const sf::Rect head_collision = {head.position, head.size};

            if (checkCollision(head_collision, {it->position, it->size})) {
                m_snake.grow();
                it = m_fruits.erase(it);
                spawnFruit();
                SoundSystem::Play(RESPATH_EAT_WAV);
            } else {
                ++it;
            }
        }
        
        // collision is shrunken a bit
        constexpr float COLLISION_SHRINK_FACTOR = 0.4f;
        const sf::Rect head_collision = {
            head.position + head.size * COLLISION_SHRINK_FACTOR / 2.f,
            head.size * (1.f - COLLISION_SHRINK_FACTOR)
        };

        for (const auto* entity : m_snake.getEntities() | std::views::drop(2)) {
            if (checkCollision(head_collision, {entity->position, entity->size})) {
                SoundSystem::Play(RESPATH_DEATH_WAV);
                m_state = State::GameOver;

                return;
            }
        }

        for (const auto& rock : m_rocks) {
            const auto [dx, dy] = rock.position - head.position;

            if (std::abs(dx) > TILE_SIZE || std::abs(dy) > TILE_SIZE) {
                continue; // skip distant rocks
            }

            if (checkCollision(head_collision, {rock.position, rock.size})) {
                SoundSystem::Play(RESPATH_DEATH_WAV);
                m_state = State::GameOver;

                return;
            }
        }

        // Check collision with borders
        if (head.position.x < 0.f ||
            head.position.x > m_width * snek::TILE_SIZE ||
            head.position.y < 0.f ||
            head.position.y > m_height * snek::TILE_SIZE) {
            SoundSystem::Play(RESPATH_DEATH_WAV);
            m_state = State::GameOver;
        }
    }
}; // class Board

} // namespace snek
