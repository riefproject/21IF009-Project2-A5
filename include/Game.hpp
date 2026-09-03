#pragma once

/**
 * @file Game.hpp
 * @brief Active game session state and core mechanics orchestration.
 * @author Arief
 *
 * @details Manages live game session state: player lives, score, level difficulty,
 * speed calculations across 11 modes, frame countdowns, block dropping timers,
 * power-up updates, and coordinating the Grid, Player, BulletManager, and PowerUpManager.
 */

#include "Constants.hpp"
#include "Grid.hpp"
#include "Player.hpp"
#include "BulletManager.hpp"
#include "PowerUpManager.hpp"

class AssetManager;
class ScoreManager;

/**
 * @class Game
 * @brief Encapsulates an active playing session and its subsystem entities.
 */
class Game {
public:
    Grid grid;
    Player player;
    BulletManager bullets;
    PowerUpManager powerups;

    ll score{0};
    int lives{3};
    int frameCounter{0};
    int rowAddDelay{60};
    float blockFallTimer{0.0f};
    bool gameOver{false};
    int gameLevel{0};

    Game() = default;

    /**
     * @brief Initializes a fresh game round for a specific difficulty level.
     * @param level Mode index (0 to 10).
     * @param assets Reference to loaded AssetManager.
     */
    void init(int level, const AssetManager& assets);

    /**
     * @brief Calculates base block falling interval based on chosen difficulty mode.
     * @param mode Mode index.
     * @return Interval multiplier in seconds.
     */
    float getSpeedForMode(int mode) const;

    /**
     * @brief Main gameplay update tick: processes input, physics, timers, and game over checks.
     * @param dt Frame delta time in seconds.
     * @param assets Reference to loaded audio/visual assets.
     * @param sfxEnabled Whether sound effects are enabled.
     */
    void update(float dt, const AssetManager& assets, bool sfxEnabled);

    /**
     * @brief Renders the game board, grid, bullets, power-ups, player, laser, and HUD.
     * @param assets Reference to loaded assets.
     * @param scoreMgr Reference to ScoreManager for leaderboard stats.
     */
    void draw(const AssetManager& assets, const ScoreManager& scoreMgr) const;
};
