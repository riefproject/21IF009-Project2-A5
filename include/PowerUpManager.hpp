#pragma once

/**
 * @file PowerUpManager.hpp
 * @brief Power-up generation, sinusoidal falling physics, and active effects queue.
 * @author Naira
 *
 * @details Implements the dynamic power-up item system, randomized spawning at screen top,
 * trigonometric wavy floating descent, collision detection with the player, immediate
 * or timed gameplay modifiers, and maintaining a FIFO queue of up to 3 concurrent active effects.
 */

#include "Defines.hpp"
#include <deque>

class AssetManager;
class Player;
class Game;

/**
 * @class PowerUpManager
 * @brief Manages falling power-up items and active player buffs/debuffs.
 */
class PowerUpManager {
private:
    bool m_powerupActive{false};
    PowerUp m_currentPowerup;
    Vector2 m_powerupPosition{0.0f, 0.0f};
    float m_powerupTimer{3.0f};
    std::deque<ActivePowerup> m_activeEffects; // Maximum 3 concurrent active effects

public:
    PowerUpManager();

    /**
     * @brief Spawns a new randomized power-up entity at the top of the gameplay window.
     */
    void spawn();

    /**
     * @brief Updates falling trajectory and checks collection collision with player.
     * @param dt Frame delta time.
     * @param player Const reference to player entity.
     * @param game Reference to active game session.
     * @param assets Reference to loaded audio/visual assets.
     * @param sfxEnabled Whether sound effects are enabled.
     */
    void update(float dt, const Player& player, Game& game, const AssetManager& assets, bool sfxEnabled);

    /**
     * @brief Activates the effect of the collected power-up item.
     * @param game Reference to active game session.
     * @param assets Reference to loaded audio assets.
     * @param sfxEnabled Whether sound effects are enabled.
     */
    void activate(Game& game, const AssetManager& assets, bool sfxEnabled);

    /**
     * @brief Updates countdown timers for active effects and reverts buffs upon expiry.
     * @param dt Frame delta time in seconds.
     * @param game Reference to active game session.
     */
    void updateActiveEffects(float dt, Game& game);

    /**
     * @brief Renders the currently descending power-up sprite on screen.
     * @param assets Reference to loaded textures.
     */
    void draw(const AssetManager& assets) const;

    /**
     * @brief Checks if a specific power-up effect is currently active in the queue.
     * @param type PowerUpType enum to query.
     * @return True if currently active.
     */
    bool isTypeActive(PowerUpType type) const;

    /**
     * @brief Retrieves texture and timer UI color for a given power-up type.
     * @param assets Reference to loaded textures.
     * @param type PowerUpType enum.
     * @return PowerUpVisuals containing texture and HUD color.
     */
    static PowerUpVisuals getVisuals(const AssetManager& assets, PowerUpType type);

    /**
     * @brief Resets power-up system state, clearing falling items and active effects.
     */
    void reset();

    /**
     * @brief Checks if a power-up item is currently descending on screen.
     */
    bool isPowerupFalling() const { return m_powerupActive; }

    /**
     * @brief Gets current timer until next potential spawn.
     */
    float getSpawnTimer() const { return m_powerupTimer; }

    /**
     * @brief Sets timer until next spawn.
     * @param t Time in seconds.
     */
    void setSpawnTimer(float t) { m_powerupTimer = t; }

    /**
     * @brief Accesses active power-up effects queue (const).
     */
    const std::deque<ActivePowerup>& getActiveEffects() const { return m_activeEffects; }

    /**
     * @brief Accesses active power-up effects queue (mutable).
     */
    std::deque<ActivePowerup>& getActiveEffects() { return m_activeEffects; }
};
