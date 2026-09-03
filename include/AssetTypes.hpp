#pragma once

/**
 * @file AssetTypes.hpp
 * @brief Strongly-typed asset identifiers for audio, textures, fonts, and skins.
 * @author Arief
 *
 * @details Isolates multimedia asset enumeration keys so changes to asset lists
 * do not trigger re-compilation cascades across unrelated gameplay systems.
 */

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
