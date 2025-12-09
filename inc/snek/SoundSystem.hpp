/**
 * @file SoundSystem.hpp
 * 
 * @brief Sound system for the snek game.
 */
#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Audio/PlaybackDevice.hpp>

#include <algorithm>
#include <cstdlib>
#include <print>
#include <stdexcept>
#include <string_view>
#include <vector>
#include <string>
#include <unordered_map>

namespace snek {

class SoundSystem {
public:
    static auto Play(std::string_view sound_path) -> void {
        auto& instance = get_instance();

        if (!instance.ensurePlaybackDevice()) {
            return;
        }

        const sf::SoundBuffer* buffer{nullptr};
        try {
            buffer = &instance.loadOrGetBuffer(sound_path);
        } catch (const std::exception& ex) {
            std::println(stderr, "Failed to load sound {}: {}", sound_path, ex.what());
            return;
        }

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

        m_sounds.emplace_back(*buffer);
        auto& sound = m_sounds.back();
        sound.setVolume(instance.m_volume);
        sound.play();
    }

    static auto SetVolume(float volume) -> void {
        auto& instance = get_instance();
        instance.m_volume = volume;

        if (instance.m_audioStatus != AudioStatus::Available) {
            return;
        }

        for (auto& sound : instance.m_sounds) {
            sound.setVolume(volume);
        }
    }
private:
    enum class AudioStatus {
        Unknown,
        Available,
        Unavailable
    };

    float m_volume{60.f};
    std::vector<sf::Sound> m_sounds;
    std::unordered_map<std::string, sf::SoundBuffer> m_sound_buffers;
    AudioStatus m_audioStatus{AudioStatus::Unknown};

    auto loadOrGetBuffer(std::string_view sound_path) -> const sf::SoundBuffer& {
        const auto key = std::string(sound_path);
        const auto it = m_sound_buffers.find(key);

        if (it != m_sound_buffers.end()) {
            return it->second;
        }

        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile(key)) {
            throw std::runtime_error("Failed to load sound: " + key);
        }

        auto [emplaced_it, _] = m_sound_buffers.emplace(key, std::move(buffer));

        return emplaced_it->second;
    }

    auto ensurePlaybackDevice() -> bool {
        if (m_audioStatus == AudioStatus::Unavailable) {
            return false;
        }
        if (m_audioStatus == AudioStatus::Available) {
            return true;
        }

        if (std::getenv("SNEK_DISABLE_AUDIO") != nullptr) {
            std::println(stderr, "Audio disabled via SNEK_DISABLE_AUDIO.");
            m_audioStatus = AudioStatus::Unavailable;
            return false;
        }

        const auto devices = sf::PlaybackDevice::getAvailableDevices();
        if (devices.empty()) {
            std::println(stderr, "Audio disabled: no playback device available.");
            m_audioStatus = AudioStatus::Unavailable;
            return false;
        }

        m_audioStatus = AudioStatus::Available;
        return true;
    }

    static auto get_instance() -> SoundSystem& {
        static SoundSystem instance;
        return instance;
    }
}; // class SoundSystem

} // namespace snek
