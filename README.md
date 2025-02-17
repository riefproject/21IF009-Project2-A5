# 21IF009-Project2-A5

This is a **Brick Shooter** game project for the **Project 2: Library-Based Application Development** course.

## 📌 Features

-   Players shoot bricks to clear rows while avoiding falling bricks.
-   Increasing difficulty as the game progresses.
-   Developed **entirely in C**.

## 📂 Project Structure

-   **src/** → Contains the source code files (.c), including the main game logic and entry point (`main.c`).
-   **include/** → Stores the header files (.h), which contain declarations for functions and structures.
-   **library/** → - **library/** → Contains C source files (.c) for the game's logic modules, such as player actions, brick mechanics, etc. Each `.c` file corresponds to a specific part of the game’s functionality (e.g., `arief.c`, `naira.c`).
-   **assets/** → Stores game assets such as sprites and sounds used within the game.
-   **build/** → Directory for compiled files. Contains subdirectories to store object files and output (`build/output/`).
-   **bin/** → Output directory for the final executable file (`game.exe`).
-   **scripts/** → Contains build and clean scripts for automation (`build.bat` and `clean.bat`).

## ⚙️ How to Build and Run

1. Clone the repository:  
   `git clone https://github.com/riefproject/21IF009-Project2-A5.git`
2. Navigate to the project directory:  
   `cd 21IF009-Project2-A5`
3. Build the project using `make`:
    ```bash
    make
    ```
    or using `.bat` script:
    ```bash
    scripts/build
    ```
4. Run the game:
    ```bash
    make run
    ```
    or manually:
    ```bash
    bin/game
    ```

## 🎮 Gameplay Instructions

-   Aim and shoot bricks to clear rows.
-   Avoid falling bricks to progress.
-   Try to reach the highest score before the game ends!

🚀 **Stay tuned for updates!**
