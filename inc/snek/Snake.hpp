/**
 * @file Snake.hpp
 * 
 * @brief Snake class representing the player-controlled snake.
 * 
 * @authors Jacek Zub
 */
#pragma once

#include <SFML/System/Vector2.hpp>

#include <vector>
#include <memory>
#include <ranges>

#include "snek/constants.hpp"
#include "snek/Entity.hpp"
#include "snek/TextureManager.hpp"

namespace snek {

class Snake {
    struct Pivot {
        sf::Vector2f position;
        Direction direction;

        std::shared_ptr<Pivot> next{nullptr};
    };
    
    struct Segment {
        Entity entity;

        std::shared_ptr<Pivot> next_pivot{nullptr};
    };
public:
    Snake(
        uint32_t initial_len = SNAKE_INITIAL_LENGTH,
        sf::Vector2f start_pos = {WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f}
    ) {
        for (uint32_t i = 0u; i < initial_len; i++) {
            Segment segment;
            auto& entity = segment.entity;

            entity.position = {
                start_pos.x,
                start_pos.y + i * snek::TILE_SIZE};
            entity.size = {snek::TILE_SIZE, snek::TILE_SIZE};
            entity.direction = Direction::Up;
            entity.texture = TextureManager::getTexture("res/assets/snake_sprites.png");
            entity.textureIndex = (i == 0) ? 0u : 1u; // head uses first tile, body second
            if (i == 0) {
                entity.rotationOffsetDegrees = 90.f;
            }

            m_segments.push_back(std::move(segment));
        }
    }

    auto turnLeft() -> bool {
        if (m_segments.empty()) {
            return false;
        }

        const auto head_dir = m_segments.front().entity.direction;

        return turn(static_cast<Direction>(
            (static_cast<int32_t>(head_dir) + 3) % 4
        ));
    }

    auto turnRight() -> bool {
        if (m_segments.empty()) {
            return false;
        }

        const auto head_dir = m_segments.front().entity.direction;

        return turn(static_cast<Direction>(
            (static_cast<int32_t>(head_dir) + 1) % 4));
    }

    auto grow() -> void {
        const auto& tail = m_segments.back();

        Segment new_segment;

        new_segment.next_pivot = tail.next_pivot;

        auto& entity = new_segment.entity;

        entity.size = {snek::TILE_SIZE, snek::TILE_SIZE};
        entity.direction = tail.entity.direction;
        entity.texture = TextureManager::getTexture("res/assets/snake_sprites.png");
        entity.textureIndex = 1u;

        switch (tail.entity.direction) {
            case Direction::Up:
                entity.position = {
                    tail.entity.position.x,
                    tail.entity.position.y + snek::TILE_SIZE};
                break;
            case Direction::Right:
                entity.position = {
                    tail.entity.position.x - snek::TILE_SIZE,
                    tail.entity.position.y};
                break;
            case Direction::Down:
                entity.position = {
                    tail.entity.position.x,
                    tail.entity.position.y - snek::TILE_SIZE};
                break;
            case Direction::Left:
                entity.position = {
                    tail.entity.position.x + snek::TILE_SIZE,
                    tail.entity.position.y};
                break;
        }

        m_segments.push_back(std::move(new_segment));

        m_speed += SNAKE_SPEED_INCREMENT;
    }

    auto getEntities() const -> std::vector<const Entity*> {
        std::vector<const Entity*> entities;

        entities.reserve(m_segments.size());

        for (const auto& segment : m_segments) {
            entities.push_back(&segment.entity);
        }

        return entities;
    }

    auto turn(Direction new_direction) -> bool {
        if (m_distance_since_last_turn < snek::TILE_SIZE) {
            return false;
        }

        auto& head = m_segments.front().entity;

        if (head.direction == new_direction) {
            return false;
        }

        m_distance_since_last_turn = 0.f;
        head.direction = new_direction;

        auto pivot = std::make_shared<Pivot>();
        pivot->position = head.position;
        pivot->direction = head.direction;

        for (auto& segment : m_segments | std::views::drop(1)) {
            if (segment.next_pivot == nullptr) {
                segment.next_pivot = pivot;
            } else {
                segment.next_pivot->next = pivot;
                break;
            }
        }

        return true;
    }

    auto move() -> void {
        m_distance_since_last_turn += m_speed;

        for (auto& segment : m_segments) {
            if (segment.next_pivot == nullptr) {
                move(segment, m_speed);
            } else {
                const auto dist = distanceToNextPivot(segment);

                if (dist > m_speed) {
                    move(segment, m_speed);
                } else {
                    segment.entity.position = segment.next_pivot->position;
                    segment.entity.direction = segment.next_pivot->direction;

                    move(segment, m_speed - dist);

                    segment.next_pivot = segment.next_pivot->next;
                }
            }
        }
    }

    auto distanceToNextPivot(const Segment& segment) const -> float {
        const auto& pivot_pos = segment.next_pivot->position;
        const auto& seg_pos = segment.entity.position;

        switch (segment.entity.direction) {
            case Direction::Up:
                return seg_pos.y - pivot_pos.y;
            case Direction::Right:
                return pivot_pos.x - seg_pos.x;
            case Direction::Down:
                return pivot_pos.y - seg_pos.y;
            case Direction::Left:
                return seg_pos.x - pivot_pos.x;
        }

        return 0.f;
    }

    auto move(Segment& segment, float amount) -> void {
        switch (segment.entity.direction) {
            case Direction::Up:
                segment.entity.position.y -= amount;
                break;
            case Direction::Right:
                segment.entity.position.x += amount;
                break;
            case Direction::Down:
                segment.entity.position.y += amount;
                break;
            case Direction::Left:
                segment.entity.position.x -= amount;
                break;
        }
    }

    std::vector<Segment> m_segments;
    float m_speed{SNAKE_INITIAL_SPEED};

    float m_distance_since_last_turn{0.f};
}; // class Snake

} // namespace snek
