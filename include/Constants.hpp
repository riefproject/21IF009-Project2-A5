#pragma once

/**
 * @file Constants.hpp
 * @brief Global engine dimensions, difficulty configuration, and mathematical constants.
 * @author Arief
 *
 * @details Centralizes immutable compile-time constants for screen resolution,
 * gameplay grid boundaries, difficulty level settings, and primitive type aliases.
 */

#include <array>
#include <cstdint>
#include <cstdlib>
#include "raylib.h"

// =============================================================================
// PRIMITIVE TYPE ALIASES
// =============================================================================
using uint = unsigned int;
using ll = long long int;
using ull = unsigned long long int;

// =============================================================================
// SCREEN & DISPLAY CONSTANTS
// =============================================================================
constexpr float LOADING_TIME = 2.0f;

constexpr int MIN_SCREEN_WIDTH = 480;
constexpr int MIN_SCREEN_HEIGHT = 640;
constexpr int ASPECT_RATIO_WIDTH = 3;
constexpr int ASPECT_RATIO_HEIGHT = 4;

constexpr int BASE_WIDTH = MIN_SCREEN_WIDTH;
constexpr int BASE_HEIGHT = MIN_SCREEN_HEIGHT;
constexpr int GAME_SCREEN_WIDTH = 320;
constexpr int GAME_SCREEN_HEIGHT = 640;

constexpr float DFT_SPEED = 0.3f;
constexpr Color PRIMARY_COLOR = { 0, 65, 57, 255 };

// =============================================================================
// GRID DIMENSION CONSTANTS
// =============================================================================
constexpr int MAX_ROWS = 17;
constexpr int MAX_COLUMNS = 10;
constexpr int MAX_WIDTH_BLOCKS = 11;
constexpr int MAX_LEVELS = 11;

// Bitmask for a fully filled row of 10 columns: (1 << 10) - 1 = 0x03FF
constexpr uint16_t FULL_ROW_BITMASK = 0x03FF;

// =============================================================================
// LEVEL & DIFFICULTY CONFIGURATION
// =============================================================================

/**
 * @brief Human-readable names for all 11 difficulty modes.
 */
inline const std::array<const char*, MAX_LEVELS> LEVEL_NAMES = {
    "Super EZ",
    "Easy",
    "Beginner",
    "Medium",
    "Hard",
    "Super Hard",
    "Expert",
    "Master",
    "Legend",
    "God",
    "Progressive"
};

/**
 * @struct LevelBlockRange
 * @brief Min and max number of blocks generated per row for a difficulty mode.
 */
struct LevelBlockRange {
    int minBlocks;
    int maxBlocks;
};

constexpr std::array<LevelBlockRange, MAX_LEVELS> LEVEL_BLOCK_RANGES = {{
    {8, 9}, // 0: Super EZ
    {7, 9}, // 1: Easy
    {7, 8}, // 2: Beginner
    {7, 8}, // 3: Medium
    {6, 7}, // 4: Hard
    {6, 7}, // 5: Super Hard
    {6, 7}, // 6: Expert
    {5, 6}, // 7: Master
    {5, 6}, // 8: Legend
    {5, 5}, // 9: God
    {5, 9}  // 10: Progressive
}};

/**
 * @brief Base block falling speed multipliers for static difficulty levels.
 */
constexpr std::array<float, 10> FIXED_LEVEL_SPEEDS = {{
    DFT_SPEED,                       // 0: Super EZ   (0.30)
    (4.0f / 3.0f) * DFT_SPEED,       // 1: Easy       (0.40)
    (5.0f / 3.0f) * DFT_SPEED,       // 2: Beginner   (0.50)
    2.0f * DFT_SPEED,                // 3: Medium     (0.60)
    (7.0f / 3.0f) * DFT_SPEED,       // 4: Hard       (0.70)
    (8.0f / 3.0f) * DFT_SPEED,       // 5: Super Hard (0.80)
    3.0f * DFT_SPEED,                // 6: Expert     (0.90)
    (10.0f / 3.0f) * DFT_SPEED,      // 7: Master     (1.00)
    (11.0f / 3.0f) * DFT_SPEED,      // 8: Legend     (1.10)
    4.0f * DFT_SPEED                 // 9: God        (1.20)
}};

/**
 * @brief Cross-platform terminal console clearing utility.
 */
inline void clearConsole() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}
