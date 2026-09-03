#pragma once

/**
 * @file BulletManager.hpp
 * @brief Projectile physics, bullet lifecycle, and collision handling system.
 * @author Goklas
 *
 * @details Manages spawning, movement, boundary checking, and collision detection
 * for player-fired bullets against the 2D block grid.
 */

#include "Defines.hpp"
#include <vector>

class AssetManager;
class Grid;
class Game;

/**
 * @class BulletManager
 * @brief Manages active player projectiles and hit detections.
 */
class BulletManager {
private:
    std::vector<Bullets> m_bullets;
    int m_bulletCount{0};
    bool m_canShoot{true};

public:
    BulletManager() = default;

    /**
     * @brief Spawns a projectile originating from the player's cannon position.
     * @param playerPos Current player coordinates.
     * @param assets Reference to loaded audio/visual assets.
     * @param sfxEnabled Whether sound effects are unmuted.
     */
    void shoot(Vector2 playerPos, const AssetManager& assets, bool sfxEnabled);

    /**
     * @brief Updates projectile positions upward and purges out-of-bounds bullets.
     */
    void update();

    /**
     * @brief Renders all active projectiles on screen.
     * @param assets Reference to loaded sprite textures.
     */
    void draw(const AssetManager& assets) const;

    /**
     * @brief Checks and resolves collisions between active bullets and active grid blocks.
     * @param grid Reference to 2D block grid.
     * @param game Reference to active game session.
     * @param hasSpecialBullet Whether the player currently has the row-clearing bomb active.
     */
    void handleCollisions(Grid& grid, Game& game, bool hasSpecialBullet);

    /**
     * @brief Removes all active projectiles.
     */
    void clear();

    /**
     * @brief Checks if player is currently allowed to fire another bullet.
     */
    bool canShoot() const { return m_canShoot; }

    /**
     * @brief Sets shooting permission status.
     * @param value True to permit shooting.
     */
    void setCanShoot(bool value) { m_canShoot = value; }

    /**
     * @brief Returns the lifetime counter of fired bullets.
     */
    int getBulletCount() const { return m_bulletCount; }

    /**
     * @brief Resets bullet counter to zero.
     */
    void resetBulletCount() { m_bulletCount = 0; }

    /**
     * @brief Accesses the list of currently flying bullets.
     */
    const std::vector<Bullets>& getBullets() const { return m_bullets; }
};
