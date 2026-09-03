#pragma once

/**
 * @file Defines.hpp
 * @brief Core definitions, enumerations, data structures, and game constants.
 * @authors Arief, Faliq, Goklas, Naira, Raffi
 *
 * @details This header defines fundamental types, strongly-typed enum classes,
 * mathematical constants, screen dimensions, difficulty levels, data structures,
 * and inline input/scaling helpers used across the Block Shooter game.
 */

#include <string>
#include <vector>
#include <deque>
#include <array>
#include <memory>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>

#include "raylib.h"

// =============================================================================
// BASIC TYPE ALIASES
// =============================================================================
using uint = unsigned int;
using ll = long long int;
using ull = unsigned long long int;

// =============================================================================
// CORE GAME CONSTANTS
// =============================================================================
constexpr float LOADING_TIME = 2.0f;

constexpr int MAX_ROWS = 17;
constexpr int MAX_COLUMNS = 10;
constexpr int MAX_WIDTH_BLOCKS = 11;
constexpr int MAX_LEVELS = 11;

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

// Forward declaration of ScaleFactor helper
struct ScaleFactor;
ScaleFactor GetScreenScaleFactor();

// =============================================================================
// SCREEN SCALING HELPERS
// =============================================================================

/**
 * @brief Computes horizontally scaled pixel value.
 * @param var Unscaled base width value.
 * @return Scaled integer pixel value.
 */
inline int auto_x(float var);

/**
 * @brief Computes vertically scaled pixel value.
 * @param var Unscaled base height value.
 * @return Scaled integer pixel value.
 */
inline int auto_y(float var);

// =============================================================================
// INPUT HELPERS
// =============================================================================

inline bool isOkPressed() {
    return IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_Y);
}

inline bool isShootPressed() {
    return IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

inline bool isShootDown() {
    return IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_ENTER) || IsMouseButtonDown(MOUSE_BUTTON_LEFT);
}

inline bool isBackPressed() {
    return IsKeyPressed(KEY_B) || IsKeyPressed(KEY_BACKSPACE);
}

inline bool isForwardPressed() {
    return IsKeyPressed(KEY_F);
}

inline bool isMoveUp() {
    return IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP);
}

inline bool isMoveDown() {
    return IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN);
}

inline bool isMoveLeft() {
    return IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT);
}

inline bool isMoveRight() {
    return IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT);
}

/**
 * @brief Cross-platform terminal clear utility.
 */
inline void clearConsole() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

// =============================================================================
// ENUMERATIONS (STRONGLY TYPED ENUM CLASSES)
// =============================================================================

/**
 * @enum GameState
 * @brief Identifies current screen / gameplay state.
 */
enum class GameState {
    Loading,
    MainMenu,
    HighScores,
    Controls,
    Settings,
    Play,
    Quit,
    Pause,
    SelectLevel,
    GameOver,
    Scene,
    HowToPlay
};

/**
 * @enum SoundAsset
 * @brief Sound effect identifiers.
 */
enum class SoundAsset {
    Move = 0,
    Select,
    Shoot,
    Death,
    SpecialBullet,
    Heal,
    Poison,
    Slowdown,
    Speedup,
    Count
};

/**
 * @enum FontAsset
 * @brief Font style identifiers.
 */
enum class FontAsset {
    Body = 0,
    Header,
    Ingame,
    Count
};

/**
 * @enum TextureAsset
 * @brief Gameplay sprite texture identifiers.
 */
enum class TextureAsset {
    Block = 0,
    Bullet,
    Heart,
    LaserButton,
    Random,
    Speedup,
    Slowdown,
    Min1Hp,
    Pls1Hp,
    SpecialBullet,
    WhiteIcon,
    Skin1,
    Skin2,
    Count
};

/**
 * @enum ShooterSkinPart
 * @brief Multi-part shooter sprite positions.
 */
enum class ShooterSkinPart {
    Left = 0,
    Mid = 1,
    Right = 2,
    Top = 3
};

/**
 * @enum BgTexture
 * @brief Screen background texture identifiers.
 */
enum class BgTexture {
    Play = 0,
    MainMenu,
    Settings,
    HighScores,
    Paused,
    Controls,
    Confirm,
    Plain,
    Loading,
    HowToPlay,
    GameArea,
    UiGame,
    CreditScene,
    IconLoading,
    Count
};

/**
 * @enum BgModeTexture
 * @brief Mode-specific background artwork and text banners.
 */
enum class BgModeTexture {
    SuperEz = 0,
    Ez,
    Beginner,
    Medium,
    Hard,
    SuperHard,
    Expert,
    Master,
    Legend,
    God,
    Progressive,
    Count
};

/**
 * @enum PowerUpType
 * @brief Collectible power-up types.
 */
enum class PowerUpType {
    None = 0,
    SpeedUp,
    SlowDown,
    ExtraLife,
    Bomb,
    SpecialBullet,
    Random,
    Count
};

// =============================================================================
// CORE DATA STRUCTURES
// =============================================================================

/**
 * @struct Block
 * @brief Individual block tile within the grid.
 */
struct Block {
    bool active{false};
    int pos{0};
};

/**
 * @struct Bullets
 * @brief Active projectile fired by the player.
 */
struct Bullets {
    Vector2 position{0.0f, 0.0f};
    bool active{true};
};

/**
 * @struct HiScore
 * @brief High score entry per difficulty mode.
 */
struct HiScore {
    std::string mode{};
    ll score{0};
};

/**
 * @struct Settings
 * @brief User preferences configuration.
 */
struct Settings {
    int music{1};
    int sfx{1};
    int mode{0};
    uint skin{0};
};

/**
 * @struct PowerUp
 * @brief Collectible power-up entity state.
 */
struct PowerUp {
    PowerUpType type{PowerUpType::None};
    float duration{0.0f};
    bool active{false};
    float timer{0.0f};
};

/**
 * @struct ActivePowerup
 * @brief Active timed power-up effect in queue.
 */
struct ActivePowerup {
    PowerUpType type{PowerUpType::None};
    float duration{0.0f};
    bool active{false};
};

/**
 * @struct openingTransition
 * @brief Progress tracker for opening fade transitions.
 */
struct openingTransition {
    float progress{0.0f};
};

/**
 * @struct PowerUpVisuals
 * @brief Texture and timer color pair for power-up HUD rendering.
 */
struct PowerUpVisuals {
    Texture2D texture{};
    Color timerColor{WHITE};
};
