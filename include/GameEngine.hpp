#pragma once

/**
 * @file GameEngine.hpp
 * @brief Main application controller, Raylib lifecycle, and game state machine.
 * @author Arief
 *
 * @details Owns all top-level subsystems (AssetManager, SettingsManager, ScoreManager, Game),
 * manages window creation, resizing aspect-ratio enforcement, background music streams,
 * and executes the finite state machine driving all game screens.
 */

#include "Constants.hpp"
#include "Input.hpp"
#include "Scale.hpp"
#include "AssetManager.hpp"
#include "SettingsManager.hpp"
#include "ScoreManager.hpp"
#include "Game.hpp"
#include "UIManager.hpp"
#include <memory>

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
 * @class GameEngine
 * @brief Top-level game orchestrator and state machine runner.
 */
class GameEngine {
private:
    AssetManager m_assets;
    SettingsManager m_settings;
    ScoreManager m_scores;
    Music m_soundGameplay{};
    GameState m_currentState{GameState::Loading};
    GameState m_prevState{GameState::Loading};
    std::unique_ptr<Game> m_gameContext;
    float m_loadingTime{0.0f};
    bool m_running{true};

    void checkWindowResize();
    void setupAudio();

public:
    GameEngine();
    ~GameEngine();

    // Prevent copying of the engine
    GameEngine(const GameEngine&) = delete;
    GameEngine& operator=(const GameEngine&) = delete;

    /**
     * @brief Initializes Raylib window, audio system, loads assets, and starts audio playback.
     */
    void init();

    /**
     * @brief Executes the primary application game loop.
     */
    void run();

    // State machine handlers
    void handleLoading();
    void handleMainMenu();
    void handleSelectMode();
    void handlePlay();
    void handlePause();
    void handleHighScores();
    void handleSettings();
    void handleControls();
    void handleHowToPlay();
    void handleCredits();
    void handleGameOver(ll finalScore);
};
