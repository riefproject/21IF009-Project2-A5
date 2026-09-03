# 21IF009-Project2-A5

A fast-paced **Block Shooter** game project developed for the **Project 2: Library-Based Application Development** course using the Raylib game framework.

## 📌 Features

-   Dynamic block-shooting gameplay with physics-based interactions
-   11 difficulty levels with progressive challenges:
    -   Super Easy to God mode
    -   Special Progressive mode with increasing difficulty
-   Power-up system with various effects:
    -   Speed modifications
    -   Extra lives
    -   Special abilities
-   High score system with persistent storage
-   Customizable settings with music and SFX controls
-   Responsive controls with keyboard/mouse support
-   Clean, modern UI with smooth animations

## Features (More details)

**Block Shooter** is a fast-paced vertical shooter where quick reflexes and smart decisions keep you alive. Here's what makes it exciting:

### 🎯 Grid-Based Shooting

Shoot falling blocks to complete rows. Clear space, score points, and stay alive. Let the grid fill up — and it's game over.

### 🚀 Difficulty Modes

Play across multiple challenge levels: from **Super Easy** to **God Mode**, plus **Progressive Mode** that gets harder the longer you last.

### 🧠 Scoring

Clear rows, survive longer, and chase high scores. Each difficulty tracks its own leaderboard.

### ❤️ Lives

Start with three lives. Stay alive by avoiding harmful items and mistakes. Power-ups can help — or hurt.

### 🧪 Power-Ups

Power-ups drop randomly. Catch them to trigger effects:

| .   | Power-Up       | Effect                                   |
| --- | -------------- | ---------------------------------------- |
| 🟢  | Extra Life     | Adds one life (max 3)                    |
| 🟢  | Slow Down      | Blocks fall slower temporarily           |
| 🔴  | Poison         | Reduces one life                         |
| 🔴  | Speed Up       | Blocks fall faster temporarily           |
| 🟡  | Special Bullet | Clears a row, but leaves floating blocks |
| 🎲  | Lucky Box      | Random effect — sometimes hidden         |

Only three effects can be active. Use them wisely.

### 🔦 Laser

Need precision? Activate the laser pointer with **E** or **Right Shift**. Aiming made easier — for a limited time.

### 🧩 Challenge & Flow

Speed builds. Mistakes stack. Power-ups surprise. Block Shooter throws you into chaos — and dares you to master it.

## 📂 Project Structure (Industry Standard)

The project follows the standard modern C++ application layout (Pitchfork Layout):

-   **include/** → Public C++ header files (`.hpp`):
    -   `Defines.hpp` - Core enum classes, game constants, types, and input helpers
    -   `Scale.hpp` - Resolution scaling and aspect-ratio adapter
    -   `AssetManager.hpp` - Centralized RAII asset management (textures, audio, fonts)
    -   `SettingsManager.hpp` - Game configuration and user preferences persistence
    -   `ScoreManager.hpp` - High score database management and points calculation
    -   `Player.hpp` - Player character entity, laser targeting, and intro animation
    -   `BulletManager.hpp` - Projectile physics and grid collision handling
    -   `PowerUpManager.hpp` - Power-up spawning, sinusoidal descent, and active effects queue
    -   `Grid.hpp` - 2D block grid mechanics, procedural generation, and row clears
    -   `Game.hpp` - Active game session state and difficulty calculations
    -   `UIManager.hpp` - UI rendering, menus, transitions, and in-game HUD
    -   `GameEngine.hpp` - Main application engine and state machine runner
    -   `BlockShooter.hpp` - Central umbrella header
-   **src/** → C++ implementation source files (`.cpp`):
    -   `main.cpp` - Application entry point with exception handling
    -   `Scale.cpp`, `AssetManager.cpp`, `SettingsManager.cpp`, `ScoreManager.cpp`
    -   `Player.cpp`, `BulletManager.cpp`, `PowerUpManager.cpp`, `Grid.cpp`
    -   `Game.cpp`, `UIManager.cpp`, `GameEngine.cpp`
-   **assets/** → Stores game assets (sprites, sounds, music, and fonts)
-   **db/** → Stores persistent data files:
    -   `settings.dat` - User settings (SFX, music, last mode, selected skin)
    -   `hiscores.dat` - Leaderboard records for all 11 difficulty modes
-   **bin/** → Output directory for executable binary (`BlockShooter` / `BlockShooter.exe`)
-   **build/** → Build artifacts directory (`build/output/`)
-   **scripts/** → Cross-platform build automation scripts (`build.sh`, `build.bat`)

> [!NOTE]
> **Author / PIC Attribution**: In accordance with industry best practices, all files are named descriptively after their architectural responsibility. Module authors/PICs are documented via standard **Doxygen** annotations (`@author`, `@file`, `@brief`, `@details`) in each respective source file.

## ⚙️ Build Requirements

-   C++17 Compiler (GCC 9+, Clang 10+, Apple Clang, or MSVC)
-   Raylib Graphics Library (tested on Raylib 5.5+; bundled Windows binaries, `pkg-config --libs raylib` on Linux/macOS)
-   Make or CMake (optional, for Makefile / CMake support)

## 🛠️ How to Build and Run

1. Clone the repository:
    ```bash
    git clone https://github.com/riefproject/21IF009-Project2-A5.git
    ```
2. Navigate to the project directory:

    ```bash
    cd 21IF009-Project2-A5
    ```

3. Rebuild and run the project:

    - Using **Makefile** (recommended for Linux/macOS/Windows with MinGW):
        ```bash
        make rebuild
        ```
    - Or using **CMake**:
        ```bash
        cmake -B build -S .
        cmake --build build
        ./bin/BlockShooter
        ```
    - Or manually using script:
        - **Windows:**
            ```sh
            ./scripts/build.bat rebuild
            ```
        - **Linux/macOS:**
            ```sh
            ./scripts/build.sh rebuild
            ```

4. Additional build options:
   Both build scripts support various commands for different build tasks. For all available commands and detailed documentation, see [`📋scripts/README.md`](https://github.com/riefproject/21IF009-Project2-A5/blob/main/scripts). Common commands include: build (default), run, clean, rebuild, test, and help.

### Cross-platform notes

-   Makefile and `scripts/build.sh` auto-detect Windows/Linux/macOS and choose the right linker flags.
-   Windows builds use bundled Raylib binaries (`vendor/raylib-v5.5`).
-   Linux/macOS builds link Raylib via pkg-config or Homebrew (`brew install raylib`).

## 🎮 Gameplay Instructions

### Controls

| Action     | Keybinding                        |
| ---------- | --------------------------------- |
| Move Left  | ← / A                             |
| Move Right | → / D                             |
| Shoot      | Space / Enter / Left Mouse Button |
| Laser      | E                                 |
| Pause      | P                                 |
| Help       | H                                 |

### Objectives

-   Shoot strategically to clear block rows
-   Collect power-ups for advantages
-   Avoid letting blocks reach the bottom
-   Achieve high scores in each difficulty mode

### Tips

-   **Clear full rows to earn bonus points** – Rows will disappear when completely filled.
-   **Use power-ups wisely** – Some power-ups can turn the tide of battle.
-   **Master the laser ability** – It has a cooldown, so use it at the right moment.
-   **Watch falling block patterns** – Predict the next move and adjust your shots.
-   **Optimize shot timing** – Spamming bullets might not always be the best strategy.

### Menu Controls

| Action      | Keybinding        | Description                     |
| ----------- | ----------------- | ------------------------------- |
| Navigate    | ↑ / ↓ or W / S    | Move through menu options       |
| Select      | Enter / Space     | Confirm selection               |
| Back        | A / B / Backspace | Return to previous menu         |
| Forward     | F                 | Go to next menu (e.g. settings) |
| Resume Game | R                 | Resume after pause              |
| Pause       | P                 | Open pause menu                 |
| Help        | H                 | Open help screen                |
| Force Quit  | Esc               | Force exit the game             |

## 📝 Team Members

| Name   | Role           | Contributions                                |
| ------ | -------------- | -------------------------------------------- |
| Arief  | Lead Developer | Core logic, UI, Game Loop, Integration       |
| Naira  | Developer      | Power-ups, Block Animations, Assets          |
| Raffi  | Developer      | Score System, High Score Persistence, Assets |
| Faliq  | Developer      | Player Controls, Shooting Mechanics, Assets  |
| Goklas | Developer      | Projectile Physics, Collision System, Assets |

## 🤝 Contributing

Feel free to submit issues and enhancement requests via GitHub issues.

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

-   Raylib development team for the graphics framework
-   Project team members for their contributions
-   Course instructors for guidance and support

🚀 **Stay tuned for updates!**
