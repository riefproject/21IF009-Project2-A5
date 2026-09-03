/**
 * @file AssetManager.cpp
 * @brief Implementation of RAII AssetManager loading and unloading.
 * @author Arief
 */

#include "AssetManager.hpp"
#include <iostream>

AssetManager::~AssetManager() {
    unloadAll();
}

void AssetManager::loadAll() {
    if (m_loaded) return;

    // Load sounds
    m_sounds[SoundAsset::Move]          = LoadSound("assets/sounds/click.wav");
    m_sounds[SoundAsset::Select]        = LoadSound("assets/sounds/select.wav");
    m_sounds[SoundAsset::Shoot]         = LoadSound("assets/sounds/gunshot.mp3");
    m_sounds[SoundAsset::Death]         = LoadSound("assets/sounds/death.mp3");
    m_sounds[SoundAsset::SpecialBullet] = LoadSound("assets/sounds/special_bullet.mp3");
    m_sounds[SoundAsset::Heal]          = LoadSound("assets/sounds/heal.mp3");
    m_sounds[SoundAsset::Poison]        = LoadSound("assets/sounds/poison.mp3");
    m_sounds[SoundAsset::Slowdown]      = LoadSound("assets/sounds/slowdown.mp3");
    m_sounds[SoundAsset::Speedup]       = LoadSound("assets/sounds/speedup.mp3");

    // Load fonts
    m_fonts[FontAsset::Body]   = LoadFont("assets/fonts/Square.ttf");
    m_fonts[FontAsset::Header] = LoadFont("assets/fonts/Square.ttf");
    m_fonts[FontAsset::Ingame] = LoadFont("assets/fonts/Square.ttf");

    // Load textures
    m_textures[TextureAsset::Block]         = LoadTexture("assets/sprites/block.png");
    m_textures[TextureAsset::Bullet]        = LoadTexture("assets/sprites/bullet_brick.png");
    m_textures[TextureAsset::Heart]         = LoadTexture("assets/sprites/heart.png");
    m_textures[TextureAsset::LaserButton]   = LoadTexture("assets/sprites/laser_button.png");
    m_textures[TextureAsset::Random]        = LoadTexture("assets/sprites/random.png");
    m_textures[TextureAsset::Speedup]       = LoadTexture("assets/sprites/speed_up.png");
    m_textures[TextureAsset::Slowdown]      = LoadTexture("assets/sprites/slow_down.png");
    m_textures[TextureAsset::Min1Hp]        = LoadTexture("assets/sprites/minushp.png");
    m_textures[TextureAsset::Pls1Hp]        = LoadTexture("assets/sprites/heal.png");
    m_textures[TextureAsset::SpecialBullet] = LoadTexture("assets/sprites/bomb.png");
    m_textures[TextureAsset::WhiteIcon]     = LoadTexture("assets/icon/icon.png");
    m_textures[TextureAsset::Skin1]         = LoadTexture("assets/sprites/skin_1.png");
    m_textures[TextureAsset::Skin2]         = LoadTexture("assets/sprites/skin_2.png");

    // Load background textures
    m_bgTextures[BgTexture::Play]        = LoadTexture("assets/bg/BG_Play.png");
    m_bgTextures[BgTexture::MainMenu]    = LoadTexture("assets/bg/MainMenu.png");
    m_bgTextures[BgTexture::Settings]    = LoadTexture("assets/bg/BG_Settings.png");
    m_bgTextures[BgTexture::HighScores]  = LoadTexture("assets/bg/BG_HighScores.png");
    m_bgTextures[BgTexture::Paused]      = LoadTexture("assets/bg/BG_Paused.png");
    m_bgTextures[BgTexture::Controls]    = LoadTexture("assets/bg/BG_Controls.png");
    m_bgTextures[BgTexture::Confirm]     = LoadTexture("assets/bg/BG_Confirm.png");
    m_bgTextures[BgTexture::Plain]       = LoadTexture("assets/bg/BG_Plain.png");
    m_bgTextures[BgTexture::Loading]     = LoadTexture("assets/bg/BG_Loading.png");
    m_bgTextures[BgTexture::HowToPlay]   = LoadTexture("assets/bg/BG_HowToPlay.png");
    m_bgTextures[BgTexture::GameArea]    = LoadTexture("assets/bg/GameArea.png");
    m_bgTextures[BgTexture::UiGame]      = LoadTexture("assets/bg/UIArea.png");
    m_bgTextures[BgTexture::CreditScene] = LoadTexture("assets/bg/CreditScene.png");
    m_bgTextures[BgTexture::IconLoading] = LoadTexture("assets/icon/ICON_Loading.png");

    // Load mode backgrounds
    m_bgModeTextures[BgModeTexture::SuperEz]     = LoadTexture("assets/bg/mode/SuperEZ.png");
    m_bgModeTextures[BgModeTexture::Ez]          = LoadTexture("assets/bg/mode/EZ.png");
    m_bgModeTextures[BgModeTexture::Beginner]    = LoadTexture("assets/bg/mode/Beginner.png");
    m_bgModeTextures[BgModeTexture::Medium]      = LoadTexture("assets/bg/mode/Medium.png");
    m_bgModeTextures[BgModeTexture::Hard]        = LoadTexture("assets/bg/mode/Hard.png");
    m_bgModeTextures[BgModeTexture::SuperHard]   = LoadTexture("assets/bg/mode/SuperHard.png");
    m_bgModeTextures[BgModeTexture::Expert]      = LoadTexture("assets/bg/mode/Expert.png");
    m_bgModeTextures[BgModeTexture::Master]      = LoadTexture("assets/bg/mode/Master.png");
    m_bgModeTextures[BgModeTexture::Legend]      = LoadTexture("assets/bg/mode/Legend.png");
    m_bgModeTextures[BgModeTexture::God]         = LoadTexture("assets/bg/mode/God.png");
    m_bgModeTextures[BgModeTexture::Progressive] = LoadTexture("assets/bg/mode/Progressive.png");

    // Load mode text overlays
    m_txModeTextures[BgModeTexture::SuperEz]     = LoadTexture("assets/bg/mode/tx/SuperEZ.png");
    m_txModeTextures[BgModeTexture::Ez]          = LoadTexture("assets/bg/mode/tx/EZ.png");
    m_txModeTextures[BgModeTexture::Beginner]    = LoadTexture("assets/bg/mode/tx/Beginner.png");
    m_txModeTextures[BgModeTexture::Medium]      = LoadTexture("assets/bg/mode/tx/Medium.png");
    m_txModeTextures[BgModeTexture::Hard]        = LoadTexture("assets/bg/mode/tx/Hard.png");
    m_txModeTextures[BgModeTexture::SuperHard]   = LoadTexture("assets/bg/mode/tx/SuperHard.png");
    m_txModeTextures[BgModeTexture::Expert]      = LoadTexture("assets/bg/mode/tx/Expert.png");
    m_txModeTextures[BgModeTexture::Master]      = LoadTexture("assets/bg/mode/tx/Master.png");
    m_txModeTextures[BgModeTexture::Legend]      = LoadTexture("assets/bg/mode/tx/Legend.png");
    m_txModeTextures[BgModeTexture::God]         = LoadTexture("assets/bg/mode/tx/God.png");
    m_txModeTextures[BgModeTexture::Progressive] = LoadTexture("assets/bg/mode/tx/Progressive.png");

    // Load shooter skin sprites (Skin 1 & Skin 2)
    m_shooterSkins[0][static_cast<int>(ShooterSkinPart::Left)]  = LoadTexture("assets/sprites/shooter1l.png");
    m_shooterSkins[0][static_cast<int>(ShooterSkinPart::Mid)]   = LoadTexture("assets/sprites/shooter1m.png");
    m_shooterSkins[0][static_cast<int>(ShooterSkinPart::Right)] = LoadTexture("assets/sprites/shooter1r.png");
    m_shooterSkins[0][static_cast<int>(ShooterSkinPart::Top)]   = LoadTexture("assets/sprites/shooter1t.png");

    m_shooterSkins[1][static_cast<int>(ShooterSkinPart::Left)]  = LoadTexture("assets/sprites/shooter2l.png");
    m_shooterSkins[1][static_cast<int>(ShooterSkinPart::Mid)]   = LoadTexture("assets/sprites/shooter2m.png");
    m_shooterSkins[1][static_cast<int>(ShooterSkinPart::Right)] = LoadTexture("assets/sprites/shooter2r.png");
    m_shooterSkins[1][static_cast<int>(ShooterSkinPart::Top)]   = LoadTexture("assets/sprites/shooter2t.png");

    m_loaded = true;
    std::cout << "[LOG] All assets loaded successfully (C++ RAII AssetManager).\n";
}

void AssetManager::unloadAll() {
    if (!m_loaded) return;

    for (auto& [id, sound] : m_sounds) {
        UnloadSound(sound);
    }
    m_sounds.clear();

    for (auto& [id, font] : m_fonts) {
        UnloadFont(font);
    }
    m_fonts.clear();

    for (auto& [id, tex] : m_textures) {
        UnloadTexture(tex);
    }
    m_textures.clear();

    for (auto& [id, tex] : m_bgTextures) {
        UnloadTexture(tex);
    }
    m_bgTextures.clear();

    for (auto& [id, tex] : m_bgModeTextures) {
        UnloadTexture(tex);
    }
    m_bgModeTextures.clear();

    for (auto& [id, tex] : m_txModeTextures) {
        UnloadTexture(tex);
    }
    m_txModeTextures.clear();

    for (int s = 0; s < 2; ++s) {
        for (int p = 0; p < 4; ++p) {
            UnloadTexture(m_shooterSkins[s][p]);
        }
    }

    m_loaded = false;
    std::cout << "[LOG] All assets unloaded cleanly.\n";
}

const Sound& AssetManager::getSound(SoundAsset id) const {
    static Sound emptySound{};
    auto it = m_sounds.find(id);
    return (it != m_sounds.end()) ? it->second : emptySound;
}

const Font& AssetManager::getFont(FontAsset id) const {
    static Font emptyFont{};
    auto it = m_fonts.find(id);
    return (it != m_fonts.end()) ? it->second : emptyFont;
}

const Texture2D& AssetManager::getTexture(TextureAsset id) const {
    static Texture2D emptyTex{};
    auto it = m_textures.find(id);
    return (it != m_textures.end()) ? it->second : emptyTex;
}

const Texture2D& AssetManager::getBg(BgTexture id) const {
    static Texture2D emptyTex{};
    auto it = m_bgTextures.find(id);
    return (it != m_bgTextures.end()) ? it->second : emptyTex;
}

const Texture2D& AssetManager::getBgMode(BgModeTexture id) const {
    static Texture2D emptyTex{};
    auto it = m_bgModeTextures.find(id);
    return (it != m_bgModeTextures.end()) ? it->second : emptyTex;
}

const Texture2D& AssetManager::getTxMode(BgModeTexture id) const {
    static Texture2D emptyTex{};
    auto it = m_txModeTextures.find(id);
    return (it != m_txModeTextures.end()) ? it->second : emptyTex;
}

const Texture2D& AssetManager::getShooterPart(uint skin, ShooterSkinPart part) const {
    static Texture2D emptyTex{};
    uint s = (skin >= 2) ? 0 : skin;
    int p = static_cast<int>(part);
    if (p < 0 || p >= 4) return emptyTex;
    return m_shooterSkins[s][p];
}
