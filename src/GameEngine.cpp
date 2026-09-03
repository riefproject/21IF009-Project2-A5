/**
 * @file GameEngine.cpp
 * @brief Implementation of GameEngine state machine, game screens, and window loop.
 * @author Arief
 */

#include "GameEngine.hpp"
#include "UIManager.hpp"
#include <vector>
#include <string>

GameEngine::GameEngine() = default;

GameEngine::~GameEngine() {
    if (m_soundGameplay.stream.buffer != nullptr) {
        UnloadMusicStream(m_soundGameplay);
    }
    m_assets.unloadAll();
    CloseVirtualCanvas();
    CloseAudioDevice();
    CloseWindow();
}

void GameEngine::init() {
    int screenWidth = MIN_SCREEN_WIDTH;
    int screenHeight = (screenWidth * ASPECT_RATIO_HEIGHT) / ASPECT_RATIO_WIDTH;

    InitWindow(screenWidth, screenHeight, "Block Shooter (C++17)");
    InitVirtualCanvas();

    Image ico = LoadImage("assets/icon/icon.png");
    SetWindowIcon(ico);
    UnloadImage(ico);

    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(MIN_SCREEN_WIDTH, MIN_SCREEN_HEIGHT);
    SetTargetFPS(60);

    InitAudioDevice();

    m_assets.loadAll();
    m_settings.load();
    m_scores.loadHiScores();

    setupAudio();

    // Intro opening animation
    float transProgress = 0.0f;
    openingAnimation(&transProgress, m_assets);

    m_currentState = GameState::Loading;
    m_prevState = GameState::Loading;
}

void GameEngine::setupAudio() {
    m_soundGameplay = LoadMusicStream("assets/sounds/gameplay.mp3");
    PlayMusicStream(m_soundGameplay);
    SetMusicVolume(m_soundGameplay, m_settings.get().music ? 0.5f : 0.0f);
    SetSoundVolume(m_assets.getSound(SoundAsset::Move), m_settings.get().sfx ? 1.0f : 0.0f);
    SetSoundVolume(m_assets.getSound(SoundAsset::Select), m_settings.get().sfx ? 1.0f : 0.0f);
}

void GameEngine::checkWindowResize() {
    // Toggle Fullscreen via F11 or Alt+Enter
    if (IsKeyPressed(KEY_F11) || (IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_ENTER))) {
        ToggleFullscreen();
    }
}

void GameEngine::run() {
    init();

    while (!WindowShouldClose() && m_running) {
        checkWindowResize();

        switch (m_currentState) {
        case GameState::Loading:
            handleLoading();
            break;
        case GameState::MainMenu:
            handleMainMenu();
            break;
        case GameState::SelectLevel:
            handleSelectMode();
            break;
        case GameState::Play:
            UpdateMusicStream(m_soundGameplay);
            handlePlay();
            break;
        case GameState::Pause:
            handlePause();
            break;
        case GameState::HighScores:
            handleHighScores();
            break;
        case GameState::Settings:
            handleSettings();
            break;
        case GameState::Controls:
            handleControls();
            break;
        case GameState::HowToPlay:
            handleHowToPlay();
            break;
        case GameState::Scene:
            handleCredits();
            break;
        case GameState::Quit:
            m_running = false;
            break;
        default:
            break;
        }
    }
}

void GameEngine::handleLoading() {
    if (loadingScreen(m_assets, &m_loadingTime)) {
        m_currentState = GameState::MainMenu;
    }
}

void GameEngine::handleMainMenu() {
    m_prevState = GameState::MainMenu;
    Font defaultFont = GetFontDefault();

    const char* lines[] = {
        "PLAY",
        "HIGH SCORE",
        "HOW TO PLAY",
        "SETTINGS",
        "QUIT"
    };

    int fontSize = auto_x(30);
    int spacing = auto_x(51);
    constexpr int lineCount = 5;
    static int selection = 0;

    // Tunggu key release agar tidak double press dari screen sebelumnya
    while (IsKeyDown(KEY_ENTER) || IsKeyDown(KEY_SPACE)) {
        BeginVirtualCanvas();
        ClearBackground(PRIMARY_COLOR);
        EndVirtualCanvas();
    }

    while (m_currentState == GameState::MainMenu && !WindowShouldClose()) {
        BeginVirtualCanvas();
        ClearBackground(PRIMARY_COLOR);
        drawBG(m_assets, BgTexture::MainMenu);

        const Texture2D& menuBg = m_assets.getBg(BgTexture::MainMenu);
        int startY = (menuBg.height > 0) ? (auto_y(1023) * VIRTUAL_SCREEN_HEIGHT / menuBg.height) : auto_y(300);

        for (int i = 0; i < lineCount; ++i) {
            drawCenteredText(defaultFont, lines[i], static_cast<float>(startY), fontSize, static_cast<float>(auto_x(2)), (selection == i) ? ORANGE : DARKGRAY);
            startY += fontSize + spacing;
        }

        selection = handleMenuNavigation(selection, lineCount, m_assets, m_settings.get().sfx);

        if (isOkPressed()) {
            if (m_settings.get().sfx) PlaySound(m_assets.getSound(SoundAsset::Select));
            switch (selection) {
            case 0: m_currentState = GameState::SelectLevel; break;
            case 1: m_currentState = GameState::HighScores; break;
            case 2: m_currentState = GameState::HowToPlay; break;
            case 3: m_currentState = GameState::Settings; break;
            case 4: m_currentState = GameState::Quit; break;
            }
        }
        EndVirtualCanvas();
    }
}

void GameEngine::handleSelectMode() {
    m_prevState = GameState::SelectLevel;
    int currentSelection = m_settings.get().mode;
    int targetSelection = currentSelection;
    int lineCount = MAX_LEVELS;
    bool selecting = true;

    float transition = 0.0f;
    float transitionSpeed = 4.0f;
    int transitionDirection = 0;

    int currentSkin = static_cast<int>(m_settings.get().skin);
    int targetSkin = currentSkin;
    float skinTransition = 0.0f;
    int skinTransitionDirection = 0;

    while (selecting && !WindowShouldClose()) {
        float dt = GetFrameTime();

        updateTransition(&transition, &currentSelection, targetSelection, &transitionDirection, dt, transitionSpeed);
        updateTransition(&skinTransition, &currentSkin, targetSkin, &skinTransitionDirection, dt, transitionSpeed);

        BeginVirtualCanvas();
        ClearBackground(PRIMARY_COLOR);

        renderModeTransition(m_assets, currentSelection, targetSelection, transition, transitionDirection);
        renderSkinTransition(m_assets, currentSkin, targetSkin, skinTransition, skinTransitionDirection);

        EndVirtualCanvas();

        if (transitionDirection == 0 && skinTransitionDirection == 0) {
            if (isMoveDown()) {
                if (m_settings.get().sfx) PlaySound(m_assets.getSound(SoundAsset::Move));
                targetSkin = currentSkin - 1;
                if (targetSkin < 0) targetSkin = 1;
                skinTransitionDirection = 1;
                skinTransition = 0.0f;
            }
            if (isMoveUp()) {
                if (m_settings.get().sfx) PlaySound(m_assets.getSound(SoundAsset::Move));
                targetSkin = currentSkin + 1;
                if (targetSkin > 1) targetSkin = 0;
                skinTransitionDirection = -1;
                skinTransition = 0.0f;
            }
            if (isMoveLeft()) {
                if (m_settings.get().sfx) PlaySound(m_assets.getSound(SoundAsset::Move));
                targetSelection = currentSelection - 1;
                if (targetSelection < 0) targetSelection = lineCount - 1;
                transitionDirection = -1;
                transition = 0.0f;
            }
            if (isMoveRight()) {
                if (m_settings.get().sfx) PlaySound(m_assets.getSound(SoundAsset::Move));
                targetSelection = currentSelection + 1;
                if (targetSelection >= lineCount) targetSelection = 0;
                transitionDirection = 1;
                transition = 0.0f;
            }
            if (isOkPressed()) {
                if (m_settings.get().sfx) PlaySound(m_assets.getSound(SoundAsset::Select));
                m_settings.get().mode = currentSelection;
                m_settings.get().skin = static_cast<uint>(currentSkin);
                m_settings.save();

                m_currentState = GameState::Play;
                selecting = false;
                showCountdown(m_assets);
            }
            if (isBackPressed()) {
                if (m_settings.get().sfx) PlaySound(m_assets.getSound(SoundAsset::Move));
                m_currentState = GameState::MainMenu;
                selecting = false;
            }
        }
    }
}

void GameEngine::handlePlay() {
    // Inisialisasi sesi game baru jika belum ada atau bukan resume dari pause
    if (!m_gameContext || (m_prevState != GameState::Play && m_prevState != GameState::Pause && m_prevState != GameState::Controls && m_prevState != GameState::HowToPlay)) {
        m_gameContext = std::make_unique<Game>();
        m_gameContext->init(m_settings.get().mode, m_assets);
    }
    m_prevState = GameState::Play;

    float dt = GetFrameTime();
    m_gameContext->update(dt, m_assets, m_settings.get().sfx);

    if (m_gameContext->gameOver) {
        handleGameOver(m_gameContext->score);
        return;
    }

    // Input pemain
    m_gameContext->player.move(isMoveLeft(), isMoveRight(), m_assets, m_settings.get().sfx);

    if (isShootPressed()) {
        m_gameContext->bullets.shoot(
            { static_cast<float>(m_gameContext->player.x), static_cast<float>(m_gameContext->player.y) },
            m_assets,
            m_settings.get().sfx
        );
    }
    if (isShootDown()) {
        m_gameContext->bullets.setCanShoot(true);
    }

    if ((IsKeyPressed(KEY_E) || IsKeyPressed(KEY_RIGHT_SHIFT))) {
        m_gameContext->player.triggerLaser();
    }

    if (IsKeyPressed(KEY_P)) {
        m_currentState = GameState::Pause;
        return;
    }
    if (IsKeyPressed(KEY_H)) {
        m_currentState = GameState::HowToPlay;
        return;
    }
    if (IsKeyPressed(KEY_F1)) {
        m_gameContext->grid.printDebug();
    }

    // Render game screen
    BeginVirtualCanvas();
    ClearBackground(WHITE);

    float blockSize = static_cast<float>(auto_x(32));
    float gameWidth = blockSize * MAX_COLUMNS;
    float gameHeight = blockSize * MAX_ROWS - blockSize;

    // Game area background
    const Texture2D& gameBg = m_assets.getBg(BgTexture::GameArea);
    if (gameBg.width > 0 && gameBg.height > 0) {
        DrawTexturePro(gameBg,
            { 0.0f, 0.0f, static_cast<float>(gameBg.width), static_cast<float>(gameBg.height) },
            { 0.0f, 0.0f, gameWidth, static_cast<float>(VIRTUAL_SCREEN_HEIGHT) },
            { 0.0f, 0.0f }, 0.0f, WHITE);
    }

    // Sidebar UI area background (Fixed 160x640, 100% immune to stretching)
    float sidebarX = gameWidth;
    float sidebarWidth = static_cast<float>(VIRTUAL_SCREEN_WIDTH) - gameWidth;
    const Texture2D& uiBg = m_assets.getBg(BgTexture::UiGame);
    if (uiBg.width > 0 && uiBg.height > 0) {
        DrawTexturePro(uiBg,
            { 0.0f, 0.0f, static_cast<float>(uiBg.width), static_cast<float>(uiBg.height) },
            { sidebarX, 0.0f, sidebarWidth, static_cast<float>(VIRTUAL_SCREEN_HEIGHT) },
            { 0.0f, 0.0f }, 0.0f, WHITE);
    }

    // Bottom boundary line
    DrawRectangle(0, static_cast<int>(gameHeight), static_cast<int>(gameWidth), auto_y(1), RAYWHITE);

    // Entity rendering
    m_gameContext->grid.draw(m_assets);
    m_gameContext->powerups.draw(m_assets);
    m_gameContext->player.drawLaser(m_gameContext->grid);
    m_gameContext->player.draw(m_assets, m_settings.get().skin);
    m_gameContext->bullets.draw(m_assets);

    drawGameUI(*m_gameContext, m_assets, m_scores);

    EndVirtualCanvas();
}

void GameEngine::handlePause() {
    const char* lines[] = { "RESUME", "HOW TO PLAY", "SETTINGS", "MAIN MENU", "QUIT" };
    int selection = 0;
    int fontSize = auto_y(20);
    constexpr int lineCount = 5;
    bool paused = true;

    while (m_currentState == GameState::Pause && paused && !WindowShouldClose()) {
        BeginVirtualCanvas();
        ClearBackground(Fade(RAYWHITE, 0.9f));
        drawBG(m_assets, BgTexture::Paused);
        drawMenu(m_assets, lines, lineCount, selection, fontSize, ORANGE);
        EndVirtualCanvas();

        selection = handleMenuNavigation(selection, lineCount, m_assets, m_settings.get().sfx);

        if (isOkPressed()) {
            if (m_settings.get().sfx) PlaySound(m_assets.getSound(SoundAsset::Select));
            paused = false;

            switch (selection) {
            case 0: // Resume
                m_currentState = GameState::Play;
                showCountdown(m_assets);
                break;
            case 1: // How to Play
                m_currentState = GameState::HowToPlay;
                break;
            case 2: // Settings
                m_currentState = GameState::Settings;
                break;
            case 3: // Main Menu
                if (showConfirmationDialog(m_assets, m_settings.get().sfx, "Back to Main Menu?", (const char*[]){"Yes", "No"}, 2, ORANGE)) {
                    m_currentState = GameState::MainMenu;
                    m_gameContext.reset();
                } else {
                    paused = true; // Kembali ke menu pause
                }
                break;
            case 4: // Quit
                m_currentState = GameState::Quit;
                break;
            }
        }
    }
}

void GameEngine::handleGameOver(ll finalScore) {
    m_currentState = GameState::GameOver;
    if (m_settings.get().sfx) {
        PlaySound(m_assets.getSound(SoundAsset::Death));
    }

    m_scores.updateHighScore(m_settings.get().mode, finalScore);
    ll currentHighScore = m_scores.getHighScoreForMode(m_settings.get().mode);

    const char* message = "GAME OVER";
    const char* options[] = { "Retry", "Main Menu" };
    int selection = 1;
    int fontSize = auto_y(20);
    constexpr int lineCount = 2;
    bool inGameOver = true;
    float countdown = 2.0f;
    bool canSelect = false;

    while (inGameOver && !WindowShouldClose()) {
        float dt = GetFrameTime();
        countdown -= dt;
        if (countdown <= 0.0f) canSelect = true;

        BeginVirtualCanvas();
        ClearBackground(PRIMARY_COLOR);
        drawBG(m_assets, BgTexture::Plain);

        drawCenteredText(m_assets.getFont(FontAsset::Header), message, static_cast<float>(auto_y(100)), 30, 2.0f, RAYWHITE);

        int startY = auto_y(220);
        drawGameOverScore(m_assets, finalScore, currentHighScore, &startY);

        if (!canSelect) {
            drawCountdownTimer(m_assets, &countdown, startY);
        } else {
            drawMenu(m_assets, options, lineCount, selection, fontSize, ORANGE);
            selection = handleMenuNavigation(selection, lineCount, m_assets, m_settings.get().sfx);

            if (isOkPressed()) {
                if (m_settings.get().sfx) PlaySound(m_assets.getSound(SoundAsset::Select));
                if (selection == 0) {
                    m_currentState = GameState::Play;
                    m_gameContext.reset();
                    showCountdown(m_assets);
                } else {
                    m_currentState = GameState::MainMenu;
                    m_gameContext.reset();
                }
                inGameOver = false;
            }
        }
        EndVirtualCanvas();
    }
}

void GameEngine::handleHighScores() {
    int fontSize = auto_y(20);
    int spacing = auto_x(4);
    m_scores.loadHiScores();

    std::vector<std::string> linesStr(MAX_LEVELS);
    std::vector<const char*> linePtrs(MAX_LEVELS);

    for (int i = 0; i < MAX_LEVELS; ++i) {
        linesStr[i] = std::string(LEVEL_NAMES[i]) + ": " + std::to_string(m_scores.getHighScoreForMode(i));
        linePtrs[i] = linesStr[i].c_str();
    }

    while (m_currentState == GameState::HighScores && !WindowShouldClose()) {
        BeginVirtualCanvas();
        ClearBackground(PRIMARY_COLOR);
        drawBG(m_assets, BgTexture::HighScores);

        int totalHeight = calculateTotalHeight(MAX_LEVELS, fontSize, static_cast<float>(spacing));
        int startY = (VIRTUAL_SCREEN_HEIGHT - totalHeight) / 2;

        int maxLabelWidth = 0, maxValueWidth = 0;
        calculateMaxWidths(linePtrs.data(), MAX_LEVELS, fontSize, static_cast<float>(spacing), &maxLabelWidth, &maxValueWidth, m_assets.getFont(FontAsset::Body));

        int padding = auto_x(20);
        int totalWidth = maxLabelWidth + 10 + static_cast<int>(MeasureTextEx(m_assets.getFont(FontAsset::Body), ":", static_cast<float>(fontSize), static_cast<float>(spacing)).x) + 5 + maxValueWidth;
        int startX = (VIRTUAL_SCREEN_WIDTH - totalWidth) / 2;
        if (startX < padding) startX = padding;

        drawLabelsAndValues(linePtrs.data(), MAX_LEVELS, startX, startY, maxLabelWidth, fontSize, static_cast<float>(spacing), m_assets.getFont(FontAsset::Body), RAYWHITE);

        const char* controlsText = "[A] / [B]: Main Menu";
        Vector2 tSize = MeasureTextEx(m_assets.getFont(FontAsset::Body), controlsText, static_cast<float>(auto_y(20)), static_cast<float>(auto_x(2)));
        float bottomX = (static_cast<float>(VIRTUAL_SCREEN_WIDTH) - tSize.x) / 2.0f;
        DrawTextEx(m_assets.getFont(FontAsset::Body), controlsText, { bottomX, static_cast<float>(auto_y(560)) }, static_cast<float>(auto_y(20)), static_cast<float>(auto_x(2)), DARKGRAY);

        EndVirtualCanvas();

        if (isMoveLeft() || isBackPressed()) {
            if (m_settings.get().sfx) PlaySound(m_assets.getSound(SoundAsset::Move));
            m_currentState = GameState::MainMenu;
        }
    }
}

void GameEngine::handleSettings() {
    int fontSize = auto_y(20);
    int spacing = auto_x(4);
    int menuSpacing = auto_y(25);

    std::string toggleLines[2];
    toggleLines[0] = std::string("Music : ") + (m_settings.get().music ? "On" : "Off");
    toggleLines[1] = std::string("Sfx   : ") + (m_settings.get().sfx ? "On" : "Off");

    const char* menuLines[] = {
        "Controls",
        "Reset High Scores",
        "Credits"
    };

    const char* togglePtrs[2] = { toggleLines[0].c_str(), toggleLines[1].c_str() };
    constexpr int toggleCount = 2;
    constexpr int menuCount = 3;
    constexpr int totalCount = toggleCount + menuCount;

    static int selection = 0;
    bool editing = false;

    while (m_currentState == GameState::Settings && !WindowShouldClose()) {
        BeginVirtualCanvas();
        ClearBackground(PRIMARY_COLOR);
        drawBG(m_assets, BgTexture::Settings);

        int totalHeight = calculateTotalHeight(totalCount, fontSize, static_cast<float>(menuSpacing));
        int startY = (VIRTUAL_SCREEN_HEIGHT - totalHeight) / 2;

        int maxLabelWidth = 0, maxValueWidth = 0;
        calculateMaxWidths(togglePtrs, toggleCount, fontSize, static_cast<float>(spacing), &maxLabelWidth, &maxValueWidth, m_assets.getFont(FontAsset::Body));

        int padding = auto_x(20);
        int totalWidth = maxLabelWidth + auto_x(10) + static_cast<int>(MeasureTextEx(m_assets.getFont(FontAsset::Body), ":", static_cast<float>(fontSize), static_cast<float>(spacing)).x) + auto_x(5) + maxValueWidth;
        int startX = (VIRTUAL_SCREEN_WIDTH - totalWidth) / 2;
        if (startX < padding) startX = padding;

        int y = startY;
        for (int i = 0; i < toggleCount; ++i) {
            bool isSelected = (selection == i);
            drawToggleOption(m_assets.getFont(FontAsset::Body), togglePtrs[i], startX, y, maxLabelWidth, fontSize, static_cast<float>(spacing), isSelected, editing, RAYWHITE);
            y += fontSize + spacing;
        }

        y += auto_y(50);
        for (int i = 0; i < menuCount; ++i) {
            int globalIndex = i + toggleCount;
            bool isSelected = (selection == globalIndex);
            drawCenteredText(m_assets.getFont(FontAsset::Body), menuLines[i], static_cast<float>(y), auto_x(20), 2.0f, isSelected ? ORANGE : RAYWHITE);
            y += auto_y(50);
        }

        const char* infoText = (m_prevState == GameState::Play || m_prevState == GameState::Pause)
            ? "[B]: Back    [R]: Resume" : "[A] / [B]: Main Menu";
        drawCenteredText(m_assets.getFont(FontAsset::Body), infoText, static_cast<float>(auto_y(560)), auto_y(20), 2.0f, RAYWHITE);

        EndVirtualCanvas();

        if (!editing) {
            selection = handleMenuNavigation(selection, totalCount, m_assets, m_settings.get().sfx);

            if (isOkPressed()) {
                if (m_settings.get().sfx) PlaySound(m_assets.getSound(SoundAsset::Select));
                if (selection < toggleCount) {
                    editing = true;
                } else {
                    switch (selection - toggleCount) {
                    case 0: m_currentState = GameState::Controls; break;
                    case 1:
                        if (showConfirmationDialog(m_assets, m_settings.get().sfx, "Reset all high scores?", (const char*[]){"Yes", "No"}, 2, RED)) {
                            m_scores.resetHiScores();
                            showMessageDialog(m_assets, "Scores Reset!", "All high scores reset to 0.", GREEN, RAYWHITE);
                        }
                        break;
                    case 2: m_currentState = GameState::Scene; break;
                    }
                }
            }
        } else {
            // Mode editing toggle
            if (isMoveLeft() || isMoveRight() || isMoveUp() || isMoveDown()) {
                if (m_settings.get().sfx) PlaySound(m_assets.getSound(SoundAsset::Move));
                if (selection == 0) { // Music toggle
                    m_settings.get().music = m_settings.get().music ? 0 : 1;
                    SetMusicVolume(m_soundGameplay, m_settings.get().music ? 0.5f : 0.0f);
                    toggleLines[0] = std::string("Music : ") + (m_settings.get().music ? "On" : "Off");
                } else if (selection == 1) { // SFX toggle
                    m_settings.get().sfx = m_settings.get().sfx ? 0 : 1;
                    SetSoundVolume(m_assets.getSound(SoundAsset::Move), m_settings.get().sfx ? 1.0f : 0.0f);
                    SetSoundVolume(m_assets.getSound(SoundAsset::Select), m_settings.get().sfx ? 1.0f : 0.0f);
                    toggleLines[1] = std::string("Sfx   : ") + (m_settings.get().sfx ? "On" : "Off");
                }
                togglePtrs[selection] = toggleLines[selection].c_str();
                m_settings.save();
            }

            if (isOkPressed()) {
                editing = false;
            }
        }

        if (isBackPressed() || isMoveLeft()) {
            if (m_settings.get().sfx) PlaySound(m_assets.getSound(SoundAsset::Move));
            if (m_prevState == GameState::Play || m_prevState == GameState::Pause) {
                m_currentState = GameState::Pause;
            } else {
                m_currentState = GameState::MainMenu;
            }
        }
    }
}

void GameEngine::handleControls() {
    int fontSize = auto_y(16);
    int spacing = auto_x(4);

    const char* lines[] = {
        "W / Up Arrow    : Up",
        "S / Down Arrow  : Down",
        "A / Left Arrow  : Left",
        "D / Right Arrow : Right",
        "H               : Help/Control",
        "P               : Pause",
        "Space / Enter   : Select/Shoot",
        "Mouse Left Click: Shoot",
        "E / RIGHT SHIFT : Activate Laser"
    };
    constexpr int lineCount = 9;

    while (m_currentState == GameState::Controls && !WindowShouldClose()) {
        BeginVirtualCanvas();
        ClearBackground(PRIMARY_COLOR);
        drawBG(m_assets, BgTexture::Controls);

        int totalHeight = calculateTotalHeight(lineCount, fontSize, static_cast<float>(spacing));
        int startY = (VIRTUAL_SCREEN_HEIGHT - totalHeight) / 2;

        int maxLabelWidth = 0, maxValueWidth = 0;
        calculateMaxWidths(lines, lineCount, fontSize, static_cast<float>(spacing), &maxLabelWidth, &maxValueWidth, m_assets.getFont(FontAsset::Body));

        int padding = auto_x(20);
        int totalWidth = maxLabelWidth + auto_x(10) + static_cast<int>(MeasureTextEx(m_assets.getFont(FontAsset::Body), ":", static_cast<float>(fontSize), static_cast<float>(spacing)).x) + 5 + maxValueWidth;
        int startX = (VIRTUAL_SCREEN_WIDTH - totalWidth) / 2;
        if (startX < padding) startX = padding;

        drawLabelsAndValues(lines, lineCount, startX, startY, maxLabelWidth, fontSize, static_cast<float>(spacing), m_assets.getFont(FontAsset::Body), RAYWHITE);

        const char* infoText = (m_prevState == GameState::Play || m_prevState == GameState::Pause)
            ? "[B]: Back    [R]: Resume    [F]: GUIDE"
            : "[A] / [B]: Main Menu    [F]: GUIDE";
        int infoFontSize = (m_prevState == GameState::Play) ? auto_y(15) : auto_y(20);
        drawCenteredText(m_assets.getFont(FontAsset::Body), infoText, static_cast<float>(auto_y(560)), infoFontSize, 2.0f, RAYWHITE);

        EndVirtualCanvas();

        if (m_prevState == GameState::Play && IsKeyPressed(KEY_R)) {
            if (m_settings.get().sfx) PlaySound(m_assets.getSound(SoundAsset::Move));
            m_currentState = GameState::Play;
            showCountdown(m_assets);
            break;
        }

        if (isMoveLeft() || isBackPressed()) {
            if (m_settings.get().sfx) PlaySound(m_assets.getSound(SoundAsset::Move));
            if (m_prevState == GameState::Play || m_prevState == GameState::Pause) {
                m_currentState = GameState::Pause;
            } else {
                m_currentState = GameState::MainMenu;
            }
        }

        if (isForwardPressed()) {
            if (m_settings.get().sfx) PlaySound(m_assets.getSound(SoundAsset::Move));
            m_currentState = GameState::HowToPlay;
        }
    }
}

void GameEngine::handleHowToPlay() {
    while (m_currentState == GameState::HowToPlay && !WindowShouldClose()) {
        BeginVirtualCanvas();
        ClearBackground(PRIMARY_COLOR);
        drawBG(m_assets, BgTexture::HowToPlay);

        const char* infoText = (m_prevState == GameState::Play || m_prevState == GameState::Pause)
            ? "[B]: Back    [R]: Resume    [F]: CONTROLS"
            : "[A] / [B]: Main Menu    [F]: CONTROLS";
        int infoFontSize = (m_prevState == GameState::Play || m_prevState == GameState::Pause) ? auto_y(15) : auto_y(20);
        drawCenteredText(m_assets.getFont(FontAsset::Body), infoText, static_cast<float>(auto_y(560)), infoFontSize, 2.0f, RAYWHITE);

        EndVirtualCanvas();

        if (m_prevState == GameState::Play && IsKeyPressed(KEY_R)) {
            if (m_settings.get().sfx) PlaySound(m_assets.getSound(SoundAsset::Move));
            m_currentState = GameState::Play;
            showCountdown(m_assets);
            break;
        }

        if (isMoveLeft() || isBackPressed()) {
            if (m_settings.get().sfx) PlaySound(m_assets.getSound(SoundAsset::Move));
            if (m_prevState == GameState::Play || m_prevState == GameState::Pause) {
                m_currentState = GameState::Pause;
            } else {
                m_currentState = GameState::MainMenu;
            }
        }

        if (isForwardPressed()) {
            if (m_settings.get().sfx) PlaySound(m_assets.getSound(SoundAsset::Move));
            m_currentState = GameState::Controls;
        }
    }
}

void GameEngine::handleCredits() {
    const Texture2D& sceneTex = m_assets.getBg(BgTexture::CreditScene);
    if (sceneTex.width == 0) return;

    float creditScale = static_cast<float>(VIRTUAL_SCREEN_WIDTH) / static_cast<float>(sceneTex.width);
    float creditWidth = static_cast<float>(sceneTex.width) * creditScale;
    float xPos = (static_cast<float>(VIRTUAL_SCREEN_WIDTH) - creditWidth) / 2.0f;

    float scrollY = static_cast<float>(VIRTUAL_SCREEN_HEIGHT) - static_cast<float>(auto_y(50));
    constexpr float scrollSpeed = 30.0f;

    while (m_currentState == GameState::Scene && !WindowShouldClose()) {
        scrollY -= scrollSpeed * GetFrameTime();

        BeginVirtualCanvas();
        ClearBackground(PRIMARY_COLOR);
        drawBG(m_assets, BgTexture::Plain);
        DrawTextureEx(sceneTex, { xPos, scrollY }, 0.0f, creditScale, WHITE);
        EndVirtualCanvas();

        if (scrollY <= -(static_cast<float>(sceneTex.height) * creditScale) || GetKeyPressed() != 0) {
            if (m_settings.get().sfx) PlaySound(m_assets.getSound(SoundAsset::Move));
            m_currentState = GameState::Settings;
        }
    }
}
