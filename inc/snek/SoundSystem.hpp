/**
 * @file SoundSystem.hpp
 * 
 * @brief Sound system for the snek game.
 */
#pragma once

#include <SFML/Audio.hpp>

#include <vector>
#include <string>
#include <unordered_map>

namespace snek {

class SoundSystem {
public:
    static auto Play(std::string_view sound_path) -> void {
        auto& instance = get_instance();

        const auto& buffer = instance.loadOrGetBuffer(sound_path);

        auto& m_sounds = instance.m_sounds;
        m_sounds.erase(
            std::remove_if(
                m_sounds.begin(),
                m_sounds.end(),
                [](const sf::Sound& sound) {
                    return sound.getStatus() == sf::Sound::Status::Stopped;
                }
            ),
            m_sounds.end()
        );

        m_sounds.emplace_back(buffer);
        m_sounds.back().setVolume(instance.m_volume);
        m_sounds.back().play();
    }

    static auto SetVolume(float volume) -> void {
        auto& instance = get_instance();
        instance.m_volume = volume;

        for (auto& sound : instance.m_sounds) {
            sound.setVolume(volume);
        }
    }
private:
    float m_volume{60.f};
    std::vector<sf::Sound> m_sounds;
    std::unordered_map<std::string_view, sf::SoundBuffer> m_sound_buffers;

    auto loadOrGetBuffer(std::string_view sound_path) -> const sf::SoundBuffer& {
        const auto it = m_sound_buffers.find(sound_path);

        if (it != m_sound_buffers.end()) {
            return it->second;
        }

        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile(sound_path)) {
            throw std::runtime_error("Failed to load sound: " + std::string(sound_path));
        }

        m_sound_buffers.emplace(sound_path, std::move(buffer));

        return m_sound_buffers.at(sound_path);
    }

    static auto get_instance() -> SoundSystem& {
        static SoundSystem instance;
        return instance;
    }
}; // class SoundSystem

} // namespace snek
