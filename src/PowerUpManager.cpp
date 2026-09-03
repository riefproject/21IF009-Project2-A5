/**
 * @file PowerUpManager.cpp
 * @brief Implementation of PowerUpManager physics, activation, and effects queue.
 * @author Naira
 */

#include "PowerUpManager.hpp"
#include "AssetManager.hpp"
#include "Player.hpp"
#include "Game.hpp"
#include <cmath>
#include <algorithm>

PowerUpManager::PowerUpManager() {
    reset();
}

void PowerUpManager::reset() {
    m_powerupActive = false;
    m_powerupPosition = { 0.0f, 0.0f };
    m_powerupTimer = 3.0f;
    m_activeEffects.clear();
}

void PowerUpManager::spawn() {
    m_powerupActive = true;
    int typeInt = rand() % static_cast<int>(PowerUpType::Count);
    if (typeInt == 0) typeInt = 1; // Hindari PowerUpType::None

    m_currentPowerup.type = static_cast<PowerUpType>(typeInt);
    m_powerupPosition = {
        static_cast<float>(rand() % (GAME_SCREEN_WIDTH - 32)),
        -32.0f
    };
}

void PowerUpManager::update(float /*dt*/, const Player& player, Game& game, const AssetManager& assets, bool sfxEnabled) {
    if (!m_powerupActive) return;

    // Gerakan jatuh ke bawah
    m_powerupPosition.y += 2.0f;

    // Gerakan meliuk (wavy) lembut
    m_powerupPosition.x += std::sin(static_cast<float>(GetTime()) * 2.0f) * 1.0f;

    // Batasi dalam game screen width
    if (m_powerupPosition.x < 0.0f) m_powerupPosition.x = 0.0f;
    if (m_powerupPosition.x > static_cast<float>(GAME_SCREEN_WIDTH - 32)) {
        m_powerupPosition.x = static_cast<float>(GAME_SCREEN_WIDTH - 32);
    }

    // Cek jika powerup keluar batas bawah
    if (m_powerupPosition.y > static_cast<float>(GAME_SCREEN_HEIGHT)) {
        m_powerupActive = false;
        m_powerupTimer = 5.0f + static_cast<float>(rand() % 3);
        return;
    }

    // Deteksi tabrakan dengan shooter
    Rectangle powerupRect = { m_powerupPosition.x, m_powerupPosition.y, 40.0f, 40.0f };
    Rectangle shooterRect = {
        static_cast<float>(player.x - 32),
        static_cast<float>(player.y - 32),
        96.0f,
        64.0f
    };

    if (CheckCollisionRecs(powerupRect, shooterRect)) {
        activate(game, assets, sfxEnabled);
    }
}

void PowerUpManager::activate(Game& game, const AssetManager& assets, bool sfxEnabled) {
    if (m_activeEffects.size() >= 3) return;

    PowerUpType type = m_currentPowerup.type;
    if (type == PowerUpType::Random) {
        do {
            type = static_cast<PowerUpType>(rand() % static_cast<int>(PowerUpType::Count));
        } while (type == PowerUpType::Random || type == PowerUpType::None);
    }

    // Jika efek yang sama sudah aktif, perbarui durasinya kembali ke 10 detik
    for (auto& effect : m_activeEffects) {
        if (effect.type == type) {
            effect.duration = 10.0f;
            m_powerupActive = false;
            m_powerupTimer = 7.0f + static_cast<float>(rand() % 8);
            return;
        }
    }

    float duration = 10.0f;

    switch (type) {
    case PowerUpType::SpeedUp:
        game.rowAddDelay = static_cast<int>(game.rowAddDelay * 0.25f);
        if (game.rowAddDelay < 10) game.rowAddDelay = 10;
        if (sfxEnabled) PlaySound(assets.getSound(SoundAsset::Speedup));
        break;

    case PowerUpType::SlowDown:
        game.rowAddDelay = static_cast<int>(game.rowAddDelay * 2.5f);
        if (game.rowAddDelay > 240) game.rowAddDelay = 240;
        if (sfxEnabled) PlaySound(assets.getSound(SoundAsset::Slowdown));
        break;

    case PowerUpType::SpecialBullet:
        game.bullets.resetBulletCount();
        if (sfxEnabled) PlaySound(assets.getSound(SoundAsset::SpecialBullet));
        break;

    case PowerUpType::ExtraLife:
        if (game.lives < 3) {
            game.lives++;
            if (sfxEnabled) PlaySound(assets.getSound(SoundAsset::Heal));
        }
        duration = 0.0f; // Efek instan
        break;

    case PowerUpType::Bomb:
        game.lives--;
        if (sfxEnabled) PlaySound(assets.getSound(SoundAsset::Poison));
        duration = 0.0f; // Efek instan
        break;

    default:
        break;
    }

    if (duration > 0.0f) {
        m_activeEffects.push_back({ type, duration, true });
    }

    m_powerupActive = false;
    m_powerupTimer = 7.0f + static_cast<float>(rand() % 8);
}

void PowerUpManager::updateActiveEffects(float dt, Game& game) {
    for (auto& effect : m_activeEffects) {
        if (effect.active) {
            effect.duration -= dt;
            if (effect.duration <= 0.0f) {
                // Revert efek statistik
                switch (effect.type) {
                case PowerUpType::SpeedUp:
                    game.rowAddDelay *= 4;
                    break;
                case PowerUpType::SlowDown:
                    game.rowAddDelay = static_cast<int>(game.rowAddDelay / 2.5f);
                    break;
                default:
                    break;
                }
            }
        }
    }

    // Hapus efek yang sudah expired
    m_activeEffects.erase(
        std::remove_if(m_activeEffects.begin(), m_activeEffects.end(),
            [](const ActivePowerup& e) { return e.duration <= 0.0f; }),
        m_activeEffects.end()
    );
}

void PowerUpManager::draw(const AssetManager& assets) const {
    if (!m_powerupActive) return;

    Texture2D powerupTexture{};
    float localScale = 1.0f;

    switch (m_currentPowerup.type) {
    case PowerUpType::SpeedUp:
        powerupTexture = assets.getTexture(TextureAsset::Speedup);
        localScale = 40.0f / 640.0f;
        break;
    case PowerUpType::SlowDown:
        powerupTexture = assets.getTexture(TextureAsset::Slowdown);
        localScale = 40.0f / 1024.0f;
        break;
    case PowerUpType::SpecialBullet:
        powerupTexture = assets.getTexture(TextureAsset::SpecialBullet);
        if (powerupTexture.width > 0) {
            localScale = 40.0f / static_cast<float>(powerupTexture.width);
        }
        break;
    case PowerUpType::ExtraLife:
        powerupTexture = assets.getTexture(TextureAsset::Pls1Hp);
        localScale = 40.0f / 672.0f;
        break;
    case PowerUpType::Bomb:
        powerupTexture = assets.getTexture(TextureAsset::Min1Hp);
        localScale = 40.0f / 762.0f;
        break;
    case PowerUpType::Random:
        powerupTexture = assets.getTexture(TextureAsset::Random);
        localScale = 40.0f / 1024.0f;
        break;
    default:
        powerupTexture = assets.getTexture(TextureAsset::Random);
        localScale = 40.0f / 1024.0f;
        break;
    }

    if (powerupTexture.width > 0) {
        DrawTextureEx(powerupTexture, m_powerupPosition, 0.0f, localScale, WHITE);
    }
}

bool PowerUpManager::isTypeActive(PowerUpType type) const {
    for (const auto& effect : m_activeEffects) {
        if (effect.type == type && effect.active) {
            return true;
        }
    }
    return false;
}

PowerUpVisuals PowerUpManager::getVisuals(const AssetManager& assets, PowerUpType type) {
    PowerUpVisuals visuals;

    switch (type) {
    case PowerUpType::SpeedUp:
        visuals.texture = assets.getTexture(TextureAsset::Speedup);
        visuals.timerColor = GREEN;
        break;
    case PowerUpType::SlowDown:
        visuals.texture = assets.getTexture(TextureAsset::Slowdown);
        visuals.timerColor = RED;
        break;
    case PowerUpType::SpecialBullet:
        visuals.texture = assets.getTexture(TextureAsset::SpecialBullet);
        visuals.timerColor = BLUE;
        break;
    default:
        visuals.texture = assets.getTexture(TextureAsset::Random);
        visuals.timerColor = WHITE;
        break;
    }

    return visuals;
}
