/**
 * @file TextureManager.hpp
 * 
 * @brief Simple singleton texture manager to load and cache textures.
 * 
 * @authors Jacek Zub
 */
#pragma once

#include <SFML/Graphics/Texture.hpp>

#include <string>
#include <string_view>
#include <unordered_map>
#include <memory>
#include <print>

namespace snek {

class TextureManager {
public:
    static auto getTexture(std::string_view path) -> const sf::Texture* {
        auto& textures = instance().m_textures;

        const auto it = textures.find(std::string{path});

        if (it != textures.end()) {
            return it->second.get();
        }

        auto texture = std::make_unique<sf::Texture>();

        std::string_view key{path};

        if (!texture->loadFromFile(key)) {
            //std::println(stderr, "Failed to load texture from path: {}", path);

            return nullptr;
        }

        const auto texture_ptr = texture.get();

        textures.emplace(key, std::move(texture));

        return texture_ptr;
    }

    static auto unloadAll() -> void {
        instance().m_textures.clear();
    }

private:
    TextureManager() = default;

    static auto instance() -> TextureManager& {
        static TextureManager instance;
        return instance;
    }

    std::unordered_map<
        std::string,
        std::unique_ptr<sf::Texture>
    > m_textures;
}; // class TextureManager

} // namespace snek
