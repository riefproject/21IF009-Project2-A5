/**
 * @file Game.cpp
 * @brief Implementation of active game session updates and mode speeds.
 * @author Arief
 */

#include "Game.hpp"
#include "AssetManager.hpp"
#include "ScoreManager.hpp"

void Game::init(int level, const AssetManager& /*assets*/) {
    gameLevel = level;
    score = 0;
    lives = 3;
    frameCounter = 0;
    blockFallTimer = 0.0f;
    gameOver = false;

    player.initializePosition();
    bullets.clear();
    powerups.reset();

    int minB = LEVEL_BLOCK_RANGES[level].minBlocks;
    int maxB = LEVEL_BLOCK_RANGES[level].maxBlocks;
    grid.init(minB, maxB);
}

float Game::getSpeedForMode(int mode) const {
    if (mode >= 0 && mode < 10) {
        return FIXED_LEVEL_SPEEDS[mode];
    }
    if (mode == 10) { // Progressive
        return DFT_SPEED * (1.0f + static_cast<float>(frameCounter) / 3600.0f);
    }
    return DFT_SPEED;
}

void Game::update(float dt, const AssetManager& assets, bool sfxEnabled) {
    if (gameOver) return;

    blockFallTimer += dt;

    // PowerUp spawning
    powerups.setSpawnTimer(powerups.getSpawnTimer() - dt);
    if (powerups.getSpawnTimer() <= 0.0f && !powerups.isPowerupFalling()) {
        powerups.spawn();
        powerups.setSpawnTimer(3.0f);
    }

    powerups.update(dt, player, *this, assets, sfxEnabled);
    powerups.updateActiveEffects(dt, *this);

    // Block speed & fall movement
    int minB = LEVEL_BLOCK_RANGES[gameLevel].minBlocks;
    int maxB = LEVEL_BLOCK_RANGES[gameLevel].maxBlocks;
    float currentSpeed = getSpeedForMode(gameLevel);

    rowAddDelay = static_cast<int>(60.0f / currentSpeed);
    frameCounter++;

    if (frameCounter >= rowAddDelay) {
        grid.moveBlocksDown(minB, maxB);
        frameCounter = 0;
    }

    // Update bullets & collision
    bullets.update();
    bullets.handleCollisions(grid, *this, powerups.isTypeActive(PowerUpType::SpecialBullet));

    // Update laser
    player.updateLaser(dt);

    if (grid.isGameOverCheck() || lives <= 0) {
        gameOver = true;
    }
}
