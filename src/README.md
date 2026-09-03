# Source Directory

This directory contains all C++ implementation files (`.cpp`) for the Block Shooter game, following standard modern C++ application layout.

## 📑 Implementation Files

- `main.cpp` - Application entry point with top-level exception handling.
- `Scale.cpp` - Resolution scaling and window size calculation implementations.
- `AssetManager.cpp` - RAII asset loading and unloading implementation.
- `SettingsManager.cpp` - User settings persistence implementation (`db/settings.dat`).
- `ScoreManager.cpp` - Leaderboard file I/O and scoring calculations (`db/hiscores.dat`).
- `Player.cpp` - Player movement, laser aiming, and intro animations.
- `BulletManager.cpp` - Projectile physics and collision detection.
- `PowerUpManager.cpp` - Power-up falling physics, collection, and duration queue.
- `Grid.cpp` - Block board management, procedural row generation, and row clears.
- `Game.cpp` - Active gameplay session updates and mode speeds.
- `UIManager.cpp` - Menu rendering, transitions, dialogs, and HUD displays.
- `GameEngine.cpp` - Window management, audio streams, and game state machine.

## 📝 Author Documentation (Doxygen)

All implementation files feature standard **Doxygen headers** detailing the author, purpose, and implementation specifics.