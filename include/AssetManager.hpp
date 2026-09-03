#pragma once

/**
 * @file AssetManager.hpp
 * @brief Centralized RAII asset management for textures, sound effects, music, and fonts.
 * @author Arief
 *
 * @details Implements a Resource Acquisition Is Initialization (RAII) pattern
 * that handles loading, caching, type-safe accessing, and automatic destruction
 * of Raylib multimedia assets, preventing GPU/audio memory leaks and double-free errors.
 */

#include "AssetTypes.hpp"
#include "Constants.hpp"
#include "raylib.h"
#include <unordered_map>
#include <array>

/**
 * @class AssetManager
 * @brief Manages the lifecycle of all loaded visual and audio assets.
 */
class AssetManager {
private:
    std::unordered_map<SoundAsset, Sound> m_sounds;
    std::unordered_map<FontAsset, Font> m_fonts;
    std::unordered_map<TextureAsset, Texture2D> m_textures;
    std::unordered_map<BgTexture, Texture2D> m_bgTextures;
    std::unordered_map<BgModeTexture, Texture2D> m_bgModeTextures;
    std::unordered_map<BgModeTexture, Texture2D> m_txModeTextures;
    std::array<std::array<Texture2D, 4>, 2> m_shooterSkins{};
    bool m_loaded{false};

public:
    AssetManager() = default;
    ~AssetManager();

    // Prevent copy to avoid double-free of Raylib texture IDs and sound buffers
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;
    AssetManager(AssetManager&&) noexcept = default;
    AssetManager& operator=(AssetManager&&) noexcept = default;

    /**
     * @brief Loads all sounds, fonts, textures, backgrounds, mode art, and skins into memory.
     */
    void loadAll();

    /**
     * @brief Unloads and releases all cached multimedia assets from GPU and audio memory.
     */
    void unloadAll();

    /**
     * @brief Retrieves a const reference to a loaded sound effect.
     * @param id SoundAsset enum identifier.
     * @return Const reference to Raylib Sound.
     */
    const Sound& getSound(SoundAsset id) const;

    /**
     * @brief Retrieves a const reference to a loaded font.
     * @param id FontAsset enum identifier.
     * @return Const reference to Raylib Font.
     */
    const Font& getFont(FontAsset id) const;

    /**
     * @brief Retrieves a const reference to a gameplay sprite texture.
     * @param id TextureAsset enum identifier.
     * @return Const reference to Raylib Texture2D.
     */
    const Texture2D& getTexture(TextureAsset id) const;

    /**
     * @brief Retrieves a const reference to a full-screen background texture.
     * @param id BgTexture enum identifier.
     * @return Const reference to Raylib Texture2D.
     */
    const Texture2D& getBg(BgTexture id) const;

    /**
     * @brief Retrieves a const reference to a mode-specific background illustration.
     * @param id BgModeTexture enum identifier.
     * @return Const reference to Raylib Texture2D.
     */
    const Texture2D& getBgMode(BgModeTexture id) const;

    /**
     * @brief Retrieves a const reference to a mode-specific text overlay texture.
     * @param id BgModeTexture enum identifier.
     * @return Const reference to Raylib Texture2D.
     */
    const Texture2D& getTxMode(BgModeTexture id) const;

    /**
     * @brief Retrieves a specific multi-directional fragment of a player shooter skin.
     * @param skin Skin index (0 or 1).
     * @param part Directional segment (Left, Mid, Right, Top).
     * @return Const reference to Raylib Texture2D.
     */
    const Texture2D& getShooterPart(uint skin, ShooterSkinPart part) const;
};
