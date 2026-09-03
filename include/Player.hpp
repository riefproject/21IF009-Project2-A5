#pragma once

/**
 * @file Player.hpp
 * @brief Player character entity, input handling, laser aiming, and opening animations.
 * @author Faliq
 *
 * @details Implements the spaceship shooter entity controlled by the user,
 * horizontal movement boundaries, laser beam projection, multi-part shooter skin
 * rendering, and the opening intro sequence.
 */

#include "Defines.hpp"
#include "Scale.hpp"

class AssetManager;
class Grid;

/**
 * @class Player
 * @brief Represents the player entity, handling movement and rendering.
 */
class Player {
public:
    int x{0};
    int y{0};
    bool laserActive{false};
    float laserDuration{0.0f};
    float laserCooldown{0.0f};

    Vector2 getPosition() const { return { static_cast<float>(x), static_cast<float>(y) }; }

    Player();

    /**
     * @brief Initializes shooter position at bottom-center of gameplay area.
     */
    void initializePosition();

    /**
     * @brief Readjusts position when the game window is resized.
     */
    void updatePositionOnResize();

    /**
     * @brief Processes horizontal left/right movement input with screen boundary checks.
     * @param left True if left move input is active.
     * @param right True if right move input is active.
     * @param assets Reference to loaded audio assets.
     * @param sfxEnabled Whether sound effects are enabled.
     */
    void move(bool left, bool right, const AssetManager& assets, bool sfxEnabled);

    /**
     * @brief Triggers the laser aiming ability if cooldown has expired.
     */
    void triggerLaser();

    /**
     * @brief Updates laser duration and cooldown timers.
     * @param dt Frame delta time in seconds.
     */
    void updateLaser(float dt);

    /**
     * @brief Renders the player shooter spaceship using the selected skin.
     * @param assets Reference to loaded AssetManager.
     * @param skin Current selected skin index (0 or 1).
     */
    void draw(const AssetManager& assets, uint skin) const;

    /**
     * @brief Renders the vertical laser aiming beam and impact dot on closest block.
     * @param grid Reference to 2D block grid for raycast intersection.
     */
    void drawLaser(const Grid& grid) const;
};

// =============================================================================
// OPENING ANIMATION VISUAL EFFECTS
// =============================================================================

/**
 * @brief Calculates background fade-in color progression.
 * @param trans Pointer to transition progress value.
 * @return Fade color.
 */
Color fadeInOpeningAnimation(float* trans);

/**
 * @brief Calculates background fade-out color progression.
 * @param trans Pointer to transition progress value.
 * @return Fade color.
 */
Color fadeOutOpeningAnimation(float* trans);

/**
 * @brief Executes full opening animation sequence displaying the game icon.
 * @param trans Pointer to transition progress value.
 * @param assets Reference to loaded AssetManager.
 */
void openingAnimation(float* trans, const AssetManager& assets);
