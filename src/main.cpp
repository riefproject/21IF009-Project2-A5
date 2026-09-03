/**
 * @file main.cpp
 * @brief Entry point for the Block Shooter application.
 * @authors Arief, Faliq, Goklas, Naira, Raffi
 *
 * @details Initializes the GameEngine instance, runs the main state machine loop,
 * and handles top-level fatal exceptions gracefully.
 */

#include "BlockShooter.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    try {
        GameEngine engine;
        engine.run();
    } catch (const std::exception& e) {
        std::cerr << "[FATAL ERROR] " << e.what() << '\n';
        return 1;
    }

    clearConsole();
    std::cout << "\n\nThanks for playing \033[0;32mBlock Shooter\033[0m\n\n";

    return 0;
}
