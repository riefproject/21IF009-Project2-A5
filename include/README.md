# Include Directory

This directory contains all public C++ header files (`.hpp`) for the Block Shooter game project, structured according to modern C++ industry standards.

## 📑 Files

- `BlockShooter.hpp` - Central umbrella header providing access to all subsystems.
- `Defines.hpp` - Strongly-typed enum classes, game constants, types, and input helpers.
- `Scale.hpp` - Screen scaling factor and aspect-ratio adapter functions.
- `AssetManager.hpp` - Centralized RAII asset manager interface (textures, sounds, fonts).
- `SettingsManager.hpp` - User preferences load/save interface (`db/settings.dat`).
- `ScoreManager.hpp` - High score database interface and points calculation (`db/hiscores.dat`).
- `Player.hpp` - Player character entity, laser targeting, and intro animation declarations.
- `BulletManager.hpp` - Projectile physics and grid collision handling declarations.
- `PowerUpManager.hpp` - Power-up item generator, sinusoidal physics, and active effects queue.
- `Grid.hpp` - 2D block grid mechanics, procedural generation, and row clears.
- `Game.hpp` - Active game session state and difficulty calculations.
- `UIManager.hpp` - UI rendering, menus, transitions, and in-game HUD displays.
- `GameEngine.hpp` - Top-level application controller and state machine runner.

## 📝 Author Documentation (Doxygen)

Per modern software engineering standards, file names represent their architectural responsibilities rather than student names. Authorship and responsibilities are fully documented using standard **Doxygen tags** (`@file`, `@brief`, `@author`, `@details`) in each header and source file.
