/**
 * @file BulletManager.cpp
 * @brief Implementation of BulletManager physics and grid collisions.
 * @author Goklas
 */

#include "BulletManager.hpp"
#include "AssetManager.hpp"
#include "Grid.hpp"
#include "Game.hpp"
#include <algorithm>
#include <cmath>

void BulletManager::shoot(Vector2 playerPos, const AssetManager& assets, bool sfxEnabled) {
    float blockSize = static_cast<float>(auto_x(32));
    if (blockSize <= 0.0f) return;

    playerPos.x = std::round(playerPos.x / blockSize) * blockSize;
    playerPos.y = std::round(playerPos.y / blockSize) * blockSize;

    if (m_canShoot) {
        Bullets newBullet;
        newBullet.position = { playerPos.x, static_cast<float>(GetScreenHeight() - 20) };
        newBullet.active = true;

        m_bullets.push_back(newBullet);
        m_bulletCount++;

        if (sfxEnabled) {
            PlaySound(assets.getSound(SoundAsset::Shoot));
        }
        m_canShoot = false;
    }
}

void BulletManager::update() {
    float blockSize = static_cast<float>(auto_x(32));

    for (auto& bullet : m_bullets) {
        if (bullet.active) {
            bullet.position.y -= blockSize / 3.0f;
            if (bullet.position.y < 0.0f) {
                bullet.active = false;
            }
        }
    }

    // Membersihkan peluru yang tidak aktif (Erase-Remove Idiom)
    m_bullets.erase(
        std::remove_if(m_bullets.begin(), m_bullets.end(),
            [](const Bullets& b) { return !b.active; }),
        m_bullets.end()
    );
}

void BulletManager::draw(const AssetManager& assets) const {
    float blockSize = static_cast<float>(auto_x(32));
    const Texture2D& tex = assets.getTexture(TextureAsset::Bullet);
    if (tex.width == 0) return;

    float scale = blockSize / static_cast<float>(tex.width);

    for (const auto& bullet : m_bullets) {
        if (bullet.active) {
            DrawTextureEx(tex, bullet.position, 0.0f, scale, WHITE);
        }
    }
}

void BulletManager::handleCollisions(Grid& grid, Game& game, bool hasSpecialBullet) {
    float blockSize = static_cast<float>(auto_x(32));
    if (blockSize <= 0.0f) return;

    for (auto& bullet : m_bullets) {
        if (!bullet.active) continue;

        int gridX = static_cast<int>(bullet.position.x / blockSize);
        int gridY = static_cast<int>(bullet.position.y / blockSize);

        if (gridX >= 0 && gridX < MAX_COLUMNS && gridY >= 0 && gridY < MAX_ROWS) {
            Block* block = grid.getBlockAt(gridY, gridX);
            if (block && block->active) {
                grid.processBulletHit(gridX, gridY, bullet, game, hasSpecialBullet);
            }
        }
    }
}

void BulletManager::clear() {
    m_bullets.clear();
    m_bulletCount = 0;
    m_canShoot = true;
}
