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

## 📂 Project Structure

-   **src/** → Contains the source code files (.c), including the main game logic and entry point (`main.c`).
-   **library/** → Contains core game logic modules:
    -   `arief.c` - Core game loop and UI management
    -   `naira.c` - Power-up system implementation
    -   `raffi.c` - Score handling and persistence
    -   `faliq.c` - Player movement and shooting mechanics
    -   `goklas.c` - Projectile and collision systems
-   **assets/** → Stores game assets:
    -   Sprites: Player, blocks, projectiles
    -   Sounds: Effects and background music
    -   Fonts: UI and display text
-   **db/** → Stores game data (saved settings & scores):
    -   `settings.dat` - Stores user preferences (SFX, music, last played level)
    -   `hiscores.dat` - Stores high scores for all difficulty modes
-   **build/** → Directory for compiled files (`build/output/`)
-   **bin/** → Output directory for the final executable (`BlockShooter` / `BlockShooter.exe`)
-   **scripts/** → Build automation scripts (`build.bat`, `clean.bat`)

## ⚙️ Build Requirements

-   GCC/Clang (MinGW/MSYS2 for Windows; Xcode CLT/Homebrew GCC for macOS)
-   Raylib Graphics Library (tested on Raylib 5.5+; bundled Windows binaries, `pkg-config --libs raylib` on Linux/macOS)
-   Reestruct v1.0.0 (vendored in `vendor/reestruct-v1.0.0`)
-   Make (optional, for Makefile support)

## 🛠️ How to Build and Run

1. Clone the repository:
    ```bash
    git clone https://github.com/riefproject/21IF009-Project2-A5.git
    ```
2. Navigate to the project directory:

    ```bash
    cd 21IF009-Project2-A5
    ```

3. rebuild and run the project:

    - Using **Makefile** (recommended for Linux/macOS/Windows with MinGW):
        ```bash
        make rebuild
        ```
    - Or manually using script:
        - **Windows:**
            ```sh
            ./scripts/build.bat rebuild
            ```
        - **Linux/macOS:**
            ```sh
            . scripts/build.sh rebuild
            # OR
            chmod +x scripts/build.sh  # Only needed the first time
            ./scripts/build.sh rebuild
            ```

4. Additioal build options:
   Both build scripts support various commands for different build tasks. For all available commands and detailed documentation, see [`📋scripts/README.md`](https://github.com/riefproject/21IF009-Project2-A5/blob/main/scripts). Common commands include: build (default), run, clean, rebuild, test, and help.

### Cross-platform notes

-   Makefile and `scripts/build.sh` auto-detect Windows/Linux/macOS and choose the right linker flags.
-   Windows builds use bundled Raylib binaries (`vendor/raylib-v5.5`) and vendored Reestruct v1.0.0.
-   Linux/macOS builds expect Raylib installed and discoverable via `pkg-config` (Homebrew example: `brew install raylib`); Reestruct is linked from `vendor/reestruct-v1.0.0/lib`.

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
