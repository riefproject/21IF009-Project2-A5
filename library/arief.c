#include "defines.h"
#include "all.h"
// =======================================
//                Database 
// =======================================
void loadSettings(Settings* settings) {
    FILE* file = fopen("db/settings.dat", "r");
    if (!file) {
        printf("File tidak ditemukan! Menggunakan default settings.\n");
        settings->music = 1;
        settings->sfx = 1;
        settings->mode = 1;
        return;
    }

    char key[20];
    int value;

    while (fscanf(file, "%[^,],%d\n", key, &value) != EOF) {
        if (strcmp(key, "music") == 0) {
            settings->music = value;
        }
        else if (strcmp(key, "sfx") == 0) {
            settings->sfx = value;
        }
        else if (strcmp(key, "mode") == 0) {
            settings->mode = value;
        }
    }
    fclose(file);
}

void saveSettings(Settings settings) {
    FILE* file = fopen("db/settings.dat", "w");
    if (!file) {
        printf("Gagal menyimpan settings!\n");
        return;
    }

    fprintf(file, "music,%d\n", settings.music);
    fprintf(file, "sfx,%d\n", settings.sfx);
    fprintf(file, "mode,%d\n", settings.mode);

    fclose(file);
}

// =======================================
//            Game Variables
// =======================================

DoublyLinkedList* initGameGrid(void) {
    DoublyLinkedList* grid = createDoublyLinkedList();
    if (!grid) return NULL;

    for (int i = 0; i < MAX_ROWS; i++) {
        DoublyLinkedList* row = createDoublyLinkedList();
        if (!row) {
            DLL_freeList(grid);
            return NULL;
        }

        for (int j = 0; j < MAX_COLUMNS; j++) {
            Block* block = new(Block);
            if (!block) {
                DLL_freeList(grid);
                return NULL;
            }
            block->active = false;
            block->pos = j;
            DLL_insertBack(row, block);  // kolom
        }

        DLL_insertBack(grid, row);  // baris
    }

    return grid;
}

Game* createGameContext(void) {
    Game* game = new(Game);
    if (!game) return NULL;

    game->bulletCount = 0;
    game->canShoot = true;
    game->reloadTimer = 0;
    game->playerDirection = 0;
    game->frameCounter = 0;
    game->rowAddDelay = 120;
    game->score = 0;
    game->gameOver = false;
    game->lives = 3;
    game->laserCooldown = 0;
    game->laserDuration = 0;
    game->laserActive = false;
    game->powerupTimer = 10.0f;
    game->powerupDuration = 0;
    game->powerupActive = false;
    game->currentPowerup.type = POWERUP_NONE;
    game->currentPowerup.active = false;
    game->powerupPosition = (Vector2){ 0, 0 };

    /*

    // SLL activePowerupsQ;
    // activePowerUpsQ berisi:
    // SLLNode* activePowerUpsQ.rear = NULL
    // SLLNode* activePowerUpsQ.front = NULL
    // uint activePowerUpsQ.size = 0;

    // SLLNode berisi:
    // void* data; -> casting (PowerUp*) data
    // SLLNode* next = NULL;
    game->activePowerupsQ = NULL;

    */
    game->activeEffectsCount = 0;
    for (int i = 0; i < 3; i++) {
        game->activePowerups[i].type = POWERUP_NONE;
        game->activePowerups[i].duration = 0;
        game->activePowerups[i].active = false;
    }
    for (int i = 0; i < MAX_BULLETS; i++) {
        game->bullets[i].active = false;
        game->bullets[i].position = (Vector2){ 0, 0 };
    }
    game->grid = initGameGrid();

    return game;
}

void destroyGameContext(Game* game) {
    if (!game) return;
    delete(game);
}

// =======================================
//                Display
// =======================================

/*        Helper
 * ====================== */

ScaleFactor GetScreenScaleFactor(void) {
    ScaleFactor scale;
    float screenWidth = (float)GetScreenWidth();
    float screenHeight = (float)GetScreenHeight();

    float scaleX = screenWidth / MIN_SCREEN_WIDTH;
    float scaleY = screenHeight / MIN_SCREEN_HEIGHT;

    float finalScale = (scaleX < scaleY) ? scaleX : scaleY;

    scale.x = finalScale;
    scale.y = finalScale;

    return scale;
}

void GetAdjustedWindowSize(int width, int height, int* outWidth, int* outHeight) {
    float targetRatio = (float)ASPECT_RATIO_WIDTH / ASPECT_RATIO_HEIGHT; // 3:4

    // Hitung ukuran yang sesuai dengan rasio 3:4
    if (width > height * targetRatio) {
        // Jika lebar terlalu besar, sesuaikan berdasarkan tinggi
        *outHeight = height;
        *outWidth = (int)(height * targetRatio);
    }
    else {
        // Jika tinggi terlalu besar, sesuaikan berdasarkan lebar
        *outWidth = width;
        *outHeight = (int)(width / targetRatio);
    }

    // Pastikan ukuran tidak lebih kecil dari minimum
    *outWidth = (*outWidth < MIN_SCREEN_WIDTH) ? MIN_SCREEN_WIDTH : *outWidth;
    *outHeight = (*outHeight < MIN_SCREEN_HEIGHT) ? MIN_SCREEN_HEIGHT : *outHeight;
}

InputAsset* inputAssets(TypeofAssets type, uint id, const char* path) {
    InputAsset* metadata = malloc(sizeof(InputAsset));
    if (!metadata) {
        printf("Failed to allocate memory for InputAsset.\n");
        return NULL;
    }

    metadata->id = id;

    switch (type) {
    case TYPE_SOUND: {
        metadata->data = malloc(sizeof(Sound));
        if (!metadata->data) {
            printf("Failed to allocate memory for Sound.\n");
            delete(metadata);
            return NULL;
        }
        *(Sound*)metadata->data = LoadSound(path);
        break;
    }
    case TYPE_FONT: {
        metadata->data = malloc(sizeof(Font));
        if (!metadata->data) {
            printf("Failed to allocate memory for Font.\n");
            delete(metadata);
            return NULL;
        }
        *(Font*)metadata->data = LoadFont(path);
        break;
    }
    case TYPE_TEXTURE: {
        metadata->data = malloc(sizeof(Texture2D));
        if (!metadata->data) {
            printf("Failed to allocate memory for Texture2D.\n");
            delete(metadata);
            return NULL;
        }
        *(Texture2D*)metadata->data = LoadTexture(path);
        break;
    }
    default: {
        printf("Unknown asset type.\n");
        delete(metadata);
        return NULL;
    }
    }

    return metadata;
}

Assets* createAssets(void) {
    Assets* assets = new(Assets);
    if (!assets) return NULL;

    // Load sounds
    SLL_insertFront(&assets->sounds, inputAssets(TYPE_SOUND, SOUND_MOVE, "assets/sounds/click.wav"));
    SLL_insertFront(&assets->sounds, inputAssets(TYPE_SOUND, SOUND_SELECT, "assets/sounds/select.wav"));
    SLL_insertFront(&assets->sounds, inputAssets(TYPE_SOUND, SOUND_SHOOT, "assets/sounds/gunshot.mp3"));
    SLL_insertFront(&assets->sounds, inputAssets(TYPE_SOUND, SOUND_DEATH, "assets/sounds/death.mp3"));
    SLL_insertFront(&assets->sounds, inputAssets(TYPE_SOUND, SOUND_SPECIAL_BULLET, "assets/sounds/special_bullet.mp3"));
    SLL_insertFront(&assets->sounds, inputAssets(TYPE_SOUND, SOUND_HEAL, "assets/sounds/heal.mp3"));
    SLL_insertFront(&assets->sounds, inputAssets(TYPE_SOUND, SOUND_POISON, "assets/sounds/poison.mp3"));
    SLL_insertFront(&assets->sounds, inputAssets(TYPE_SOUND, SOUND_SLOWDOWN, "assets/sounds/slowdown.mp3"));
    SLL_insertFront(&assets->sounds, inputAssets(TYPE_SOUND, SOUND_SPEEDUP, "assets/sounds/speedup.mp3"));

    // Load fonts
    SLL_insertFront(&assets->fonts, inputAssets(TYPE_FONT, FONT_BODY, "assets/fonts/Square.ttf"));
    SLL_insertFront(&assets->fonts, inputAssets(TYPE_FONT, FONT_HEADER, "assets/fonts/Square.ttf"));

    // Load textures
    SLL_insertFront(&assets->textures, inputAssets(TYPE_TEXTURE, TEXTURE_BLOCK, "assets/sprites/block.png"));
    SLL_insertFront(&assets->textures, inputAssets(TYPE_TEXTURE, TEXTURE_BULLET, "assets/sprites/bullet_brick.png"));
    SLL_insertFront(&assets->textures, inputAssets(TYPE_TEXTURE, TEXTURE_SHOOTER_L, "assets/sprites/shooter1.png"));
    SLL_insertFront(&assets->textures, inputAssets(TYPE_TEXTURE, TEXTURE_SHOOTER_R, "assets/sprites/shooter2.png"));
    SLL_insertFront(&assets->textures, inputAssets(TYPE_TEXTURE, TEXTURE_SHOOTER_M, "assets/sprites/shooter3.png"));
    SLL_insertFront(&assets->textures, inputAssets(TYPE_TEXTURE, TEXTURE_SHOOTER_T, "assets/sprites/shooter4.png"));
    SLL_insertFront(&assets->textures, inputAssets(TYPE_TEXTURE, TEXTURE_HEART, "assets/sprites/heart.png"));
    SLL_insertFront(&assets->textures, inputAssets(TYPE_TEXTURE, TEXTURE_LASER_BUTTON, "assets/sprites/laser_button.png"));
    SLL_insertFront(&assets->textures, inputAssets(TYPE_TEXTURE, TEXTURE_RANDOM, "assets/sprites/random.png"));
    SLL_insertFront(&assets->textures, inputAssets(TYPE_TEXTURE, TEXTURE_SPEEDUP, "assets/sprites/speed_up.png"));
    SLL_insertFront(&assets->textures, inputAssets(TYPE_TEXTURE, TEXTURE_SLOWDOWN, "assets/sprites/slow_down.png"));
    SLL_insertFront(&assets->textures, inputAssets(TYPE_TEXTURE, TEXTURE_MIN1_HP, "assets/sprites/minushp.png"));
    SLL_insertFront(&assets->textures, inputAssets(TYPE_TEXTURE, TEXTURE_PLS1_HP, "assets/sprites/heal.png"));
    SLL_insertFront(&assets->textures, inputAssets(TYPE_TEXTURE, TEXTURE_SPECIAL_BULLET, "assets/sprites/bomb.png"));
    SLL_insertFront(&assets->textures, inputAssets(TYPE_TEXTURE, TEXTURE_WHITE_ICON, "assets/icon/icon.png"));

    SLL_insertFront(&assets->bg, inputAssets(TYPE_TEXTURE, BG_PLAY, "assets/bg/play.png"));
    SLL_insertFront(&assets->bg, inputAssets(TYPE_TEXTURE, BG_MAIN_MENU, "assets/bg/MainMenu.png"));
    SLL_insertFront(&assets->bg, inputAssets(TYPE_TEXTURE, BG_SETTINGS, "assets/bg/BG_Settings.png"));
    SLL_insertFront(&assets->bg, inputAssets(TYPE_TEXTURE, BG_HIGHSCORES, "assets/bg/BG_HighScores.png"));
    SLL_insertFront(&assets->bg, inputAssets(TYPE_TEXTURE, BG_PAUSED, "assets/bg/BG_Paused.png"));
    SLL_insertFront(&assets->bg, inputAssets(TYPE_TEXTURE, BG_CONTROLS, "assets/bg/BG_Controls.png"));
    SLL_insertFront(&assets->bg, inputAssets(TYPE_TEXTURE, BG_CONFIRM, "assets/bg/BG_Confirm.png"));
    SLL_insertFront(&assets->bg, inputAssets(TYPE_TEXTURE, BG_PLAIN, "assets/bg/BG_Plain.png"));
    SLL_insertFront(&assets->bg, inputAssets(TYPE_TEXTURE, CREDIT_SCENE, "assets/bg/CreditScene.png"));

    SLL_insertFront(&assets->bgMode, inputAssets(TYPE_TEXTURE, BGMODE_SUPER_EZ, "assets/bg/mode/SuperEZ.png"));
    SLL_insertFront(&assets->bgMode, inputAssets(TYPE_TEXTURE, BGMODE_EZ, "assets/bg/mode/EZ.png"));
    SLL_insertFront(&assets->bgMode, inputAssets(TYPE_TEXTURE, BGMODE_BEGINNER, "assets/bg/mode/Beginner.png"));
    SLL_insertFront(&assets->bgMode, inputAssets(TYPE_TEXTURE, BGMODE_MEDIUM, "assets/bg/mode/Medium.png"));
    SLL_insertFront(&assets->bgMode, inputAssets(TYPE_TEXTURE, BGMODE_HARD, "assets/bg/mode/Hard.png"));
    SLL_insertFront(&assets->bgMode, inputAssets(TYPE_TEXTURE, BGMODE_SUPER_HARD, "assets/bg/mode/SuperHard.png"));
    SLL_insertFront(&assets->bgMode, inputAssets(TYPE_TEXTURE, BGMODE_EXPERT, "assets/bg/mode/Expert.png"));
    SLL_insertFront(&assets->bgMode, inputAssets(TYPE_TEXTURE, BGMODE_MASTER, "assets/bg/mode/Master.png"));
    SLL_insertFront(&assets->bgMode, inputAssets(TYPE_TEXTURE, BGMODE_LEGEND, "assets/bg/mode/Legend.png"));
    SLL_insertFront(&assets->bgMode, inputAssets(TYPE_TEXTURE, BGMODE_GOD, "assets/bg/mode/God.png"));
    SLL_insertFront(&assets->bgMode, inputAssets(TYPE_TEXTURE, BGMODE_PROGRESSIVE, "assets/bg/mode/Progressive.png"));

    SLL_insertFront(&assets->txMode, inputAssets(TYPE_TEXTURE, BGMODE_SUPER_EZ, "assets/bg/mode/tx/SuperEZ.png"));
    SLL_insertFront(&assets->txMode, inputAssets(TYPE_TEXTURE, BGMODE_EZ, "assets/bg/mode/tx/EZ.png"));
    SLL_insertFront(&assets->txMode, inputAssets(TYPE_TEXTURE, BGMODE_BEGINNER, "assets/bg/mode/tx/Beginner.png"));
    SLL_insertFront(&assets->txMode, inputAssets(TYPE_TEXTURE, BGMODE_MEDIUM, "assets/bg/mode/tx/Medium.png"));
    SLL_insertFront(&assets->txMode, inputAssets(TYPE_TEXTURE, BGMODE_HARD, "assets/bg/mode/tx/Hard.png"));
    SLL_insertFront(&assets->txMode, inputAssets(TYPE_TEXTURE, BGMODE_SUPER_HARD, "assets/bg/mode/tx/SuperHard.png"));
    SLL_insertFront(&assets->txMode, inputAssets(TYPE_TEXTURE, BGMODE_EXPERT, "assets/bg/mode/tx/Expert.png"));
    SLL_insertFront(&assets->txMode, inputAssets(TYPE_TEXTURE, BGMODE_MASTER, "assets/bg/mode/tx/Master.png"));
    SLL_insertFront(&assets->txMode, inputAssets(TYPE_TEXTURE, BGMODE_LEGEND, "assets/bg/mode/tx/Legend.png"));
    SLL_insertFront(&assets->txMode, inputAssets(TYPE_TEXTURE, BGMODE_GOD, "assets/bg/mode/tx/God.png"));
    SLL_insertFront(&assets->txMode, inputAssets(TYPE_TEXTURE, BGMODE_PROGRESSIVE, "assets/bg/mode/tx/Progressive.png"));

    return assets;
}

void* getAsset(SLLNode* head, uint id) {
    SLLNode* cur = head;
    while (cur != NULL) {
        InputAsset* asset = (InputAsset*)cur->data;
        if (asset != NULL && asset->id == id) {
            return asset->data;
        }
        cur = cur->next;
    }
    return NULL;
}

void unloadAndFree(SLLNode* head, void (*unloadFunc)(void*)) {
    SLLNode* temp = head;
    while (temp) {
        InputAsset* asset = (InputAsset*)temp->data;
        if (asset && asset->data) {
            unloadFunc(asset->data);
        }
        temp = temp->next;
    }
}

void destroyAssets(Assets* assets) {
    if (!assets) return;


    // Unload sounds
    unloadAndFree(assets->sounds.head, (void (*)(void*))UnloadSound);

    // Unload fonts
    unloadAndFree(assets->fonts.head, (void (*)(void*))UnloadFont);

    // Unload textures
    unloadAndFree(assets->textures.head, (void (*)(void*))UnloadTexture);

    // Unload backgrounds
    unloadAndFree(assets->bg.head, (void (*)(void*))UnloadTexture);

    // Unload background modes
    unloadAndFree(assets->bgMode.head, (void (*)(void*))UnloadTexture);
    unloadAndFree(assets->txMode.head, (void (*)(void*))UnloadTexture);

    delete(assets);
}

/*    Core of Display
 * ====================== */

void mainWindow(void) {
    GameResources* resources = malloc(sizeof(GameResources));
    if (!resources) {
        printf("Failed to allocate resources!\n");
        return;
    }

    loadSettings(&resources->settings);
    loadHiScores(resources->scores);

    if (!resources->assets) {
        delete(resources);
        printf("Failed to create assets!\n");
        return;
    }

    float loadingTime = 0.0f;

    int screenWidth = MIN_SCREEN_WIDTH;
    int screenHeight = (screenWidth * ASPECT_RATIO_HEIGHT) / ASPECT_RATIO_WIDTH;
    P.x = 160; // Ditambahkan oleh faliq
    P.y = 598; // Ditambahkan oleh faliq
    openingTransition opTrans; // Ditambahkan oleh faliq
    opTrans.progress = 0.0f; // Ditambahkan oleh faliq

    InitWindow(screenWidth, screenHeight, "Block Shooter");
    Image ico = LoadImage("assets/icon/icon.png");
    SetWindowIcon(ico);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(MIN_SCREEN_WIDTH, MIN_SCREEN_HEIGHT);
    InitAudioDevice();
    musicGameplay(resources); // Ditambahkan faliq
    SetTargetFPS(60);
    resources->assets = createAssets();

    SetMusicVolume(soundGameplay, resources->settings.sfx ? 0.5f : 0.0f);
    SetSoundVolume(SOUND(resources, SOUND_MOVE), resources->settings.sfx ? 1.0f : 0.0f);
    SetSoundVolume(SOUND(resources, SOUND_SELECT), resources->settings.sfx ? 1.0f : 0.0f);

    // loadTextureBlock(); // Ditambahkan oleh Faliq
    openingAnimation(&opTrans.progress, resources); // Ditambahkan oleh Faliq

    resources->currentState = STATE_LOADING;
    while (!WindowShouldClose()) {
        if (IsWindowResized() || (IsWindowState(FLAG_WINDOW_MAXIMIZED) &&
            (GetScreenWidth() != GetMonitorWidth(GetCurrentMonitor()) ||
                GetScreenHeight() != GetMonitorHeight(GetCurrentMonitor())))) {

            int width = GetScreenWidth();
            int height = GetScreenHeight();
            int adjustedWidth, adjustedHeight;

            GetAdjustedWindowSize(width, height, &adjustedWidth, &adjustedHeight);

            // Update window size
            if (IsWindowState(FLAG_WINDOW_MAXIMIZED)) {
                // Jika dimaksimalkan, buat window dengan ukuran yang disesuaikan
                ClearWindowState(FLAG_WINDOW_MAXIMIZED);
                SetWindowSize(adjustedWidth, adjustedHeight);

                int monitorWidth = GetMonitorWidth(GetCurrentMonitor());
                int monitorHeight = GetMonitorHeight(GetCurrentMonitor());
                SetWindowPosition(
                    (monitorWidth - adjustedWidth) / 2,
                    (monitorHeight - adjustedHeight) / 2
                );
            }
            else {
                SetWindowSize(adjustedWidth, adjustedHeight);
            }
        }
        switch (resources->currentState) {
        case STATE_LOADING:
            if (loadingScreen(&loadingTime)) {
                resources->currentState = STATE_MAIN_MENU;
            }
            break;
        case STATE_MAIN_MENU:
            mainMenu(resources);
            break;
        case STATE_HIGH_SCORES:
            showHiScore(resources);
            break;
        case STATE_CONTROLS:
            showControls(resources);
            break;
        case STATE_SETTINGS:
            showSettings(resources);
            break;
        case STATE_PLAY:
            UpdateMusicStream(soundGameplay);
            displayGame(resources);
            break;
        case STATE_QUIT:
            exitGame(resources);
            break;
        case STATE_PAUSE:
            pauseMenu(resources);
            break;
        case STATE_SELECT_LEVEL:
            selectMode(resources);
            break;
        case STATE_SCENE:
            showCredits(resources);
            break;
        }
    }

    destroyAssets(resources->assets);
    CloseWindow();
}

void drawBG(GameResources* resources, uint id) {
    float imgScale = (float)GetScreenHeight() / BG(resources, id).height;
    float scaledWidth = BG(resources, id).width * imgScale;
    float xPos = (GetScreenWidth() - scaledWidth) / 2;

    DrawTextureEx(BG(resources, id), (Vector2) { xPos, 0 }, 0.0f, imgScale, WHITE);
}

int loadingScreen(float* loadingTime) {

    const int blockSize = 50;
    const float stepDuration = 0.25f;
    const int totalSteps = 9;
    const float cycleDuration = stepDuration * totalSteps;

    // Update loading time
    *loadingTime += GetFrameTime();
    if (*loadingTime >= LOADING_TIME) return 1;

    Vector2 steps[9][4] = {
        // Step 1:
        { {auto_x(190), auto_y(265)}, {auto_x(240), auto_y(265)}, {auto_x(190), auto_y(315)}, {auto_x(240), auto_y(315)} },
        // Step 2:
        { {auto_x(140), auto_y(265)}, {auto_x(290), auto_y(265)}, {auto_x(190), auto_y(315)}, {auto_x(240), auto_y(315)} },
        // Step 3:
        { {auto_x(140), auto_y(315)}, {auto_x(290), auto_y(315)}, {auto_x(190), auto_y(315)}, {auto_x(240), auto_y(315)} },
        // Step 4:
        { {auto_x(140), auto_y(315)}, {auto_x(290), auto_y(315)}, {auto_x(190), auto_y(265)}, {auto_x(240), auto_y(265)} },
        // Step 5:
        { {auto_x(190), auto_y(315)}, {auto_x(240), auto_y(315)}, {auto_x(190), auto_y(265)}, {auto_x(240), auto_y(265)} },
        // Step 6:
        { {auto_x(190), auto_y(315)}, {auto_x(240), auto_y(315)}, {auto_x(140), auto_y(265)}, {auto_x(290), auto_y(265)} },
        // Step 7:
        { {auto_x(190), auto_y(315)}, {auto_x(240), auto_y(315)}, {auto_x(140), auto_y(315)}, {auto_x(290), auto_y(315)} },
        // Step 8:
        { {auto_x(190), auto_y(265)}, {auto_x(240), auto_y(265)}, {auto_x(140), auto_y(315)}, {auto_x(290), auto_y(315)} },
        // Step 9:
        { {auto_x(190), auto_y(265)}, {auto_x(240), auto_y(265)}, {auto_x(190), auto_y(315)}, {auto_x(240), auto_y(315)} }
    };

    float elapsed = fmodf(GetTime(), cycleDuration);
    int currentStep = (int)(elapsed / stepDuration);
    int nextStep = (currentStep + 1) % totalSteps;

    // interpolasi 
    float alpha = (elapsed - (currentStep * stepDuration)) / stepDuration;

    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (int i = 0; i < 4; i++) {
        Vector2 currentPos = steps[currentStep][i];
        Vector2 nextPos = steps[nextStep][i];

        Vector2 smoothPos = {
            currentPos.x + (nextPos.x - currentPos.x) * alpha,
            currentPos.y + (nextPos.y - currentPos.y) * alpha
        };
        DrawRectangleV(smoothPos, (Vector2) { blockSize, blockSize }, (Color) { 34, 104, 8, 255 });
    }
    EndDrawing();

    return 0;
}

void showCredits(GameResources* resources) {
    float creditScale = (float)GetScreenWidth() / BG(resources, CREDIT_SCENE).width;
    float creditWidth = BG(resources, CREDIT_SCENE).width * creditScale;
    float xPos = (GetScreenWidth() - creditWidth) / 2;

    float scrollY = GetScreenHeight();
    float scrollSpeed = 30.0f;

    while (resources->currentState == STATE_SCENE && !WindowShouldClose()) {
        scrollY -= scrollSpeed * GetFrameTime();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        drawBG(resources, BG_PLAIN);

        DrawTextureEx(BG(resources, CREDIT_SCENE), (Vector2) { xPos, scrollY }, 0.0f, creditScale, WHITE);
        EndDrawing();

        if (scrollY <= -(BG(resources, CREDIT_SCENE).height * creditScale) ||
            IsKeyPressed(KEY_ENTER)) {
            resources->currentState = STATE_SETTINGS;
        }
    }
}

void mainMenu(GameResources* resources) {
    resources->prevState = STATE_MAIN_MENU;
    Font defaultFont = GetFontDefault();
    const char* lines[] = {
        "PLAY",
        "HIGH SCORE",
        "CONTROLS",
        "SETTINGS",
        "QUIT"
    };


    int fontSize = auto_y(30);
    int spacing = auto_y(51);
    int lineCount = len(lines);
    int selection = 0;

    while (IsKeyDown(KEY_ENTER) || IsKeyDown(KEY_SPACE)) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    while (resources->currentState == STATE_MAIN_MENU && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        drawBG(resources, BG_MAIN_MENU);

        float totalHeight = 0;
        for (int i = 0; i < lineCount; i++) {
            Vector2 textSize = MeasureTextEx(defaultFont, lines[i], fontSize, 2);
            totalHeight += textSize.y + spacing;
        }
        totalHeight -= spacing;

        int startY = auto_y(1023) * MIN_SCREEN_HEIGHT / BG(resources, BG_MAIN_MENU).height;

        for (int i = 0; i < lineCount; i++) {
            Vector2 textSize = MeasureTextEx(defaultFont, lines[i], fontSize, 2);
            int startX = (GetScreenWidth() - textSize.x) / 2;

            DrawTextEx(defaultFont, lines[i], (Vector2) { startX, startY }, fontSize, 2, selection == i ? ORANGE : DARKGRAY);
            startY += textSize.y + spacing;
        }

        // Handle input
        if (MOVE_UP) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            selection--;
            if (selection < 0) selection = lineCount - 1;
        }
        if (MOVE_DOWN) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            selection++;
            if (selection >= lineCount) selection = 0;
        }

        if (OK_KEY) {
            PlaySound(SOUND(resources, SOUND_SELECT));
            switch (selection) {
            case 0: resources->currentState = STATE_SELECT_LEVEL; break;
            case 1: resources->currentState = STATE_HIGH_SCORES; break;
            case 2: resources->currentState = STATE_CONTROLS; break;
            case 3: resources->currentState = STATE_SETTINGS; break;
            case 4:
                resources->prevState = STATE_MAIN_MENU;
                resources->currentState = STATE_QUIT;
                break;
            }
        }

        EndDrawing();
    }
}

void showControls(GameResources* resources) {

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
        "Q / Backspace   : Kembali"
    };

    int lineCount = len(lines);

    while (resources->currentState == STATE_CONTROLS && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        drawBG(resources, BG_SETTINGS);
        int totalHeight = 0;
        for (int i = 0; i < lineCount; i++) {
            totalHeight += fontSize + spacing;
        }

        int startY = (GetScreenHeight() - totalHeight) / 2;

        int maxLabelWidth = 0;
        int maxValueWidth = 0;
        for (int i = 0; i < lineCount; i++) {
            char label[50], value[50];
            sscanf(lines[i], "%[^:]:%[^\n]", label, value);
            int labelWidth = MeasureTextEx(FONT(resources, FONT_BODY), label, fontSize, spacing).x;
            int valueWidth = MeasureTextEx(FONT(resources, FONT_BODY), value, fontSize, spacing).x;
            if (labelWidth > maxLabelWidth) { maxLabelWidth = labelWidth; }
            if (valueWidth > maxValueWidth) { maxValueWidth = valueWidth; }
        }

        int padding = auto_x(20);
        int totalWidth = maxLabelWidth + 10 + MeasureTextEx(FONT(resources, FONT_BODY), ":", fontSize, spacing).x + 5 + maxValueWidth;
        int startX = (GetScreenWidth() - totalWidth) / 2;
        if (startX < padding) { startX = padding; }

        int y = startY;
        for (int i = 0; i < lineCount; i++) {
            char label[50], value[50];
            sscanf(lines[i], "%[^:]:%[^\n]", label, value);
            int colonX = startX + maxLabelWidth + 10;
            int textX = colonX + MeasureTextEx(FONT(resources, FONT_BODY), ":", fontSize, spacing).x + 5;
            DrawTextEx(FONT(resources, FONT_BODY), label, (Vector2) { startX, y }, fontSize, spacing, RAYWHITE);
            DrawTextEx(FONT(resources, FONT_BODY), ":", (Vector2) { colonX, y }, fontSize, spacing, RAYWHITE);
            DrawTextEx(FONT(resources, FONT_BODY), value, (Vector2) { textX, y }, fontSize, spacing, RAYWHITE);
            y += fontSize + spacing;
        }
        {
            const char* infoText;
            // Jika game dijeda (resources->prevState == STATE_SELECT_LEVEL) tampilkan tiga shortcut
            if (resources->prevState == STATE_PLAY || resources->prevState == STATE_PAUSE) {
                infoText = "[R]: Resume    [P]: Pause Menu    [F]: ke Settings";
            }
            else {
                infoText = "[A]: Main Menu    [F]: Settings";
            }
            Vector2 textSize = MeasureTextEx(FONT(resources, FONT_BODY), infoText, (resources->prevState == STATE_PLAY) ? 15 : 20, 2.0f);
            int startXInfo = (GetScreenWidth() - textSize.x) / 2;
            DrawTextEx(FONT(resources, FONT_BODY), infoText, (Vector2) { startXInfo, 560 }, (resources->prevState == STATE_PLAY) ? 15 : 20, 2.0f, RAYWHITE);
        }
        EndDrawing();

        if (resources->prevState != STATE_PLAY) {
            if (MOVE_LEFT || BACK_KEY) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                if (resources->prevState == STATE_PLAY) {
                    resources->currentState = STATE_PAUSE;
                }
                else if (resources->prevState == STATE_PAUSE) {
                    resources->currentState = STATE_PAUSE;
                }
                else {
                    resources->currentState = STATE_MAIN_MENU;
                }
            }
        }

        if (resources->prevState == STATE_PLAY) {
            if (IsKeyPressed(KEY_R)) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                resources->currentState = STATE_PLAY;
                countdownPause(resources);
                break;
            }
            if (IsKeyPressed(KEY_P)) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                resources->currentState = STATE_PAUSE;
                break;
            }
        }

        if (FORWARD_KEY) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            resources->currentState = STATE_SETTINGS;
        }
    }
}

void showSettings(GameResources* resources) {

    int fontSize = auto_y(20);
    int spacing = auto_x(4);
    int menuSpacing = auto_y(25);

    char lines[5][100];
    snprintf(lines[0], sizeof(lines[0]), "Music : %s", resources->settings.music ? "On" : "Off");
    snprintf(lines[1], sizeof(lines[1]), "Sfx   : %s", resources->settings.sfx ? "On" : "Off");
    SetSoundVolume(SOUND(resources, SOUND_MOVE), resources->settings.sfx ? 1.0f : 0.0f);
    SetSoundVolume(SOUND(resources, SOUND_SELECT), resources->settings.sfx ? 1.0f : 0.0f);
    snprintf(lines[2], sizeof(lines[2]), "Controls");
    snprintf(lines[3], sizeof(lines[3]), "Reset High Scores");
    snprintf(lines[4], sizeof(lines[3]), "Credits");

    int lineCount = len(lines);
    int selection = 0;
    bool editing = false;

    while (resources->currentState == STATE_SETTINGS && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        {
            float imgScale = (float)GetScreenHeight() / BG(resources, BG_SETTINGS).height;
            float scaledWidth = BG(resources, BG_SETTINGS).width * imgScale;
            float xPos = (GetScreenWidth() - scaledWidth) / 2;

            DrawTextureEx(BG(resources, BG_SETTINGS), (Vector2) { xPos, 0 }, 0.0f, imgScale, WHITE);
        }
        int totalHeight = (2 * (fontSize + spacing)) +
            (2 * (fontSize + menuSpacing));

        int startY = (GetScreenHeight() - totalHeight) / 2;

        int maxLabelWidth = 0;
        int maxValueWidth = 0;
        for (int i = 0; i < 2; i++) {
            char label[50], value[50];
            sscanf(lines[i], "%[^:]:%[^\n]", label, value);
            int labelWidth = MeasureTextEx(FONT(resources, FONT_BODY), label, fontSize, spacing).x;
            int valueWidth = MeasureTextEx(FONT(resources, FONT_BODY), value, fontSize, spacing).x;
            if (labelWidth > maxLabelWidth) { maxLabelWidth = labelWidth; }
            if (valueWidth > maxValueWidth) { maxValueWidth = valueWidth; }
        }

        int padding = auto_x(20);
        int totalWidth = maxLabelWidth + 10 + MeasureTextEx(FONT(resources, FONT_BODY), ":", fontSize, spacing).x + 5 + maxValueWidth;
        int startX = (GetScreenWidth() - totalWidth) / 2;
        if (startX < padding) { startX = padding; }

        int y = startY;
        for (int i = 0; i < 2; i++) {
            char label[50], value[50];
            sscanf(lines[i], "%[^:]:%[^\n]", label, value);
            int colonX = startX + maxLabelWidth + 10;
            int textX = colonX + MeasureTextEx(FONT(resources, FONT_BODY), ":", fontSize, spacing).x + 5;
            DrawTextEx(FONT(resources, FONT_BODY), label, (Vector2) { startX, y }, fontSize, spacing, selection == i ? editing ? DARKBLUE : ORANGE : RAYWHITE);
            DrawTextEx(FONT(resources, FONT_BODY), ":", (Vector2) { colonX, y }, fontSize, spacing, RAYWHITE);
            DrawTextEx(FONT(resources, FONT_BODY), value, (Vector2) { textX, y }, fontSize, spacing, RAYWHITE);
            y += fontSize + spacing;
        }
        y += 50;
        for (int i = 2; i < 5; ++i) {
            const char* optionText = lines[i];
            Vector2 textSize = MeasureTextEx(FONT(resources, FONT_BODY), optionText, 20, 2.0f);
            int startXOption = (GetScreenWidth() - textSize.x) / 2;
            DrawTextEx(FONT(resources, FONT_BODY), optionText, (Vector2) { startXOption, y }, 20, 2.0f, selection == i ? ORANGE : RAYWHITE);
            y += 50;
        }
        {
            const char* infoText;
            if (resources->prevState == STATE_PLAY || resources->prevState == STATE_PAUSE) {
                infoText = "[R]: Resume    [P]: Pause Menu    [F]: ke Controls";
            }
            else {
                infoText = "[A]: Main Menu    [F]: Controls";
            }
            Vector2 textSize = MeasureTextEx(FONT(resources, FONT_BODY), infoText, (resources->prevState == STATE_PLAY) ? 15 : 20, 2.0f);
            int startXInfo = (GetScreenWidth() - textSize.x) / 2;
            DrawTextEx(FONT(resources, FONT_BODY), infoText, (Vector2) { startXInfo, 560 }, (resources->prevState == STATE_PLAY) ? 15 : 20, 2.0f, RAYWHITE);
        }
        EndDrawing();

        if (!editing) {
            if (MOVE_UP) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                selection--;
                if (selection < 0) selection = lineCount - 1;
            }
            if (MOVE_DOWN) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                selection++;
                if (selection >= lineCount) selection = 0;
            }
            if (OK_KEY) {
                PlaySound(SOUND(resources, SOUND_SELECT));
                if (selection < 2) {
                    editing = true;
                }
                else {
                    switch (selection) {
                    case 2:
                        resources->currentState = STATE_CONTROLS;
                        break;
                    case 3:
                        resetHiScores(resources);
                        break;
                    case 4:
                        resources->currentState = STATE_SCENE;
                        break;
                    }
                }
            }
        }
        else {
            if (MOVE_LEFT || MOVE_RIGHT || MOVE_UP || MOVE_DOWN) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                char label[50], value[50];
                sscanf(lines[selection], "%[^:]:%[^\n]", label, value);
                if (strcmp(value, " On") == 0) {
                    snprintf(lines[selection], sizeof(lines[selection]), "%s: Off", label);
                    if (selection == 0) {
                        resources->settings.music = 0;
                        SetMusicVolume(soundGameplay, 0.0f);
                    }
                    if (selection == 1) {
                        resources->settings.sfx = 0;
                        SetSoundVolume(SOUND(resources, SOUND_MOVE), 0.0f);
                        SetSoundVolume(SOUND(resources, SOUND_SELECT), 0.0f);
                    }
                }
                else {
                    snprintf(lines[selection], sizeof(lines[selection]), "%s: On", label);
                    if (selection == 0) {
                        resources->settings.music = 1;
                        SetMusicVolume(soundGameplay, 0.5f);
                    }
                    if (selection == 1) {
                        resources->settings.sfx = 1;
                        SetSoundVolume(SOUND(resources, SOUND_MOVE), 1.0f);
                        SetSoundVolume(SOUND(resources, SOUND_SELECT), 1.0f);
                    }
                }
                saveSettings(resources->settings);
            }
            if (OK_KEY) {
                editing = false;
            }
        }

        if (resources->prevState == STATE_PLAY) {
            if (IsKeyPressed(KEY_R)) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                resources->currentState = STATE_PLAY;
                countdownPause(resources);
                break;
            }
            if (IsKeyPressed(KEY_P)) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                resources->currentState = STATE_PAUSE;
                break;
            }
        }
        else {
            if (MOVE_LEFT || BACK_KEY) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                if (resources->prevState == STATE_PLAY) {
                    resources->currentState = STATE_PAUSE;
                }
                else if (resources->prevState == STATE_PAUSE) {
                    resources->currentState = STATE_PAUSE;
                }
                else {
                    resources->currentState = STATE_MAIN_MENU;
                }
            }
        }
        if (FORWARD_KEY) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            resources->currentState = STATE_CONTROLS;
        }
    }
}

void showHiScore(GameResources* resources) {

    int fontSize = auto_y(20);
    int spacing = auto_x(4);
    loadHiScores(resources->scores);

    char lines[MAX_LEVELS][65];
    for (int i = 0; i < MAX_LEVELS; i++) {
        snprintf(lines[i], sizeof(lines[i]), "%s: %lld", resources->scores[i].mode, resources->scores[i].score);
    }

    int lineCount = len(lines);

    while (resources->currentState == STATE_HIGH_SCORES && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        drawBG(resources, BG_HIGHSCORES);
        int totalHeight = 0;
        for (int i = 0; i < lineCount; i++) {
            totalHeight += fontSize + spacing;
        }
        int startY = (GetScreenHeight() - totalHeight) / 2;

        int maxLabelWidth = 0;
        int maxValueWidth = 0;
        for (int i = 0; i < lineCount; i++) {
            char label[50], value[50];
            sscanf(lines[i], "%[^:]:%[^\n]", label, value);

            int labelWidth = MeasureTextEx(FONT(resources, FONT_BODY), label, fontSize, spacing).x;
            int valueWidth = MeasureTextEx(FONT(resources, FONT_BODY), value, fontSize, spacing).x;
            if (labelWidth > maxLabelWidth) {
                maxLabelWidth = labelWidth;
            }
            if (valueWidth > maxValueWidth) {
                maxValueWidth = valueWidth;
            }
        }

        int padding = auto_x(20);
        int totalWidth = maxLabelWidth + 10 + MeasureTextEx(FONT(resources, FONT_BODY), ":", fontSize, spacing).x + 5 + maxValueWidth;
        int startX = (GetScreenWidth() - totalWidth) / 2;

        if (startX < padding) {
            startX = padding;
        }

        int y = startY;
        for (int i = 0; i < lineCount; i++) {
            char label[50], value[50];
            sscanf(lines[i], "%[^:]:%[^\n]", label, value);

            int colonX = startX + maxLabelWidth + 10;
            int textX = colonX + MeasureTextEx(FONT(resources, FONT_BODY), ":", fontSize, spacing).x + 5;

            DrawTextEx(FONT(resources, FONT_BODY), label, (Vector2) { startX, y }, fontSize, spacing, RAYWHITE);
            DrawTextEx(FONT(resources, FONT_BODY), ":", (Vector2) { colonX, y }, fontSize, spacing, RAYWHITE);
            DrawTextEx(FONT(resources, FONT_BODY), value, (Vector2) { textX, y }, fontSize, spacing, RAYWHITE);

            y += fontSize + spacing;
        }
        {
            const char* controlsText = "[A]: Main Menu";
            Vector2 textSize = MeasureTextEx(FONT(resources, FONT_BODY), controlsText, 20, 2.0f);
            int startX = (GetScreenWidth() - textSize.x) / 2;
            DrawTextEx(FONT(resources, FONT_BODY), controlsText, (Vector2) { startX, 560 }, 20, 2.0f, DARKGRAY);
        }

        if (MOVE_LEFT || BACK_KEY) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            resources->currentState = STATE_MAIN_MENU;
        }

        EndDrawing();
    }
}

bool confirmExit(GameResources* resources) {

    const char* message = "Are you sure you want to exit?";
    const char* options[] = { "Yes", "No" };
    int selection = 1;
    int fontSize = auto_y(20);
    int lineCount = len(options);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        drawBG(resources, BG_CONFIRM);
        Vector2 textSize = MeasureTextEx(FONT(resources, FONT_HEADER), message, fontSize, 2);
        int startX = (GetScreenWidth() - textSize.x) / 2;
        int startY = (640 - textSize.y) / 2 - 50;
        DrawTextEx(FONT(resources, FONT_HEADER), message, (Vector2) { startX, startY }, fontSize, 2, RAYWHITE);

        startY += textSize.y + auto_y(50);
        for (int i = 0; i < lineCount; i++) {
            textSize = MeasureTextEx(FONT(resources, FONT_HEADER), options[i], fontSize, 2);
            startX = (GetScreenWidth() - textSize.x) / 2;
            DrawTextEx(FONT(resources, FONT_BODY), options[i], (Vector2) { startX, startY }, fontSize, 2, selection == i ? ORANGE : RAYWHITE);
            startY += textSize.y + (auto_y(20));
        }
        EndDrawing();
        if (MOVE_UP) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            selection--;
            if (selection < 0) selection = lineCount - 1;
        }
        if (MOVE_DOWN) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            selection++;
            if (selection >= lineCount) selection = 0;
        }
        if (OK_KEY) {
            PlaySound(SOUND(resources, SOUND_SELECT));
            if (selection == 0) {
                EndDrawing();
                return true;
            }
            else {
                EndDrawing();
                return false;
            }
        }
        if (IsKeyPressed(KEY_N)) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            EndDrawing();
            return false;
        }


    }

    return false;
}

void exitGame(GameResources* resources) {
    if (confirmExit(resources)) {
        saveHiScores(resources->scores);
        destroyAssets(resources->assets);
        UnloadMusicStream(soundGameplay);
        CloseWindow();
        exit(0);
    }
    else {
        resources->currentState = resources->prevState;
    }
}

bool confirmBack(GameResources* resources) {

    const char* message1 = "Are you sure you want to go back";
    const char* message2 = "to the main menu?";
    const char* options[] = { "Yes", "No" };
    int selection = 1;
    int fontSize = auto_y(20);
    int lineCount = len(options);
    bool deciding = true;

    while (deciding && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        drawBG(resources, BG_CONFIRM);
        Vector2 textSize = MeasureTextEx(FONT(resources, FONT_HEADER), message1, fontSize, 2);
        int startX = (GetScreenWidth() - textSize.x) / 2;
        int startY = (GetScreenHeight() - textSize.y) / 2 - 50;
        DrawTextEx(FONT(resources, FONT_HEADER), message1, (Vector2) { startX, startY }, fontSize, 2, DARKGRAY);

        textSize = MeasureTextEx(FONT(resources, FONT_HEADER), message2, fontSize, 2);
        startX = (GetScreenWidth() - textSize.x) / 2;
        startY += textSize.y + (auto_y(20));
        DrawTextEx(FONT(resources, FONT_HEADER), message2, (Vector2) { startX, startY }, fontSize, 2, DARKGRAY);

        startY += textSize.y + auto_y(50);
        for (int i = 0; i < lineCount; i++) {
            textSize = MeasureTextEx(FONT(resources, FONT_BODY), options[i], fontSize, 2);
            startX = (GetScreenWidth() - textSize.x) / 2;
            DrawTextEx(FONT(resources, FONT_BODY), options[i], (Vector2) { startX, startY }, fontSize, 2, selection == i ? BLUE : DARKGRAY);
            startY += textSize.y + (auto_y(20));
        }
        EndDrawing();

        if (MOVE_UP || MOVE_LEFT) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            selection--;
            if (selection < 0) selection = lineCount - 1;
        }
        if (MOVE_DOWN || MOVE_RIGHT) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            selection++;
            if (selection >= lineCount) selection = 0;
        }
        if (OK_KEY) {
            PlaySound(SOUND(resources, SOUND_SELECT));
            deciding = false;
            return selection == 0; // Return true jika "Yes", false jika "No"
        }
        if (IsKeyPressed(KEY_N) || BACK_KEY) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            return false;
        }
    }
    return false;
}

void pauseMenu(GameResources* resources) {
    resources->prevState = STATE_PAUSE;

    bool paused = true;
    const char* lines[] = {
        "RESUME",
        "CONTROLS",
        "SETTINGS",
        "MAIN MENU",
        "QUIT"
    };
    int fontSize = auto_y(20);
    int lineCount = len(lines);
    int selection = 0;

    while (paused && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Fade(RAYWHITE, 0.9f));
        drawBG(resources, BG_PAUSED);

        int totalHeight = 0;
        for (int i = 0; i < lineCount; i++) {
            Vector2 textSize = MeasureTextEx(FONT(resources, FONT_BODY), lines[i], fontSize, 2);
            totalHeight += textSize.y + 25;
        }
        int startY = (GetScreenHeight() - totalHeight) / 2 + 20;
        for (int i = 0; i < lineCount; i++) {
            Vector2 textSize = MeasureTextEx(FONT(resources, FONT_BODY), lines[i], fontSize, 2);
            int startX = (GetScreenWidth() - textSize.x) / 2;
            if (selection == i) {
                DrawRectangle(startX - 10, startY - 5, textSize.x + 20, textSize.y + 10, Fade(LIGHTGRAY, 0.3f));
            }
            DrawTextEx(FONT(resources, FONT_BODY), lines[i], (Vector2) { startX, startY }, fontSize, 2, selection == i ? ORANGE : RAYWHITE);
            startY += textSize.y + auto_y(25);
        }
        EndDrawing();

        if (MOVE_UP) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            selection--;
            if (selection < 0) selection = lineCount - 1;
        }
        if (MOVE_DOWN) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            selection++;
            if (selection >= lineCount) selection = 0;
        }
        if (OK_KEY) {
            PlaySound(SOUND(resources, SOUND_SELECT));
            switch (selection) {
            case 0: // RESUME
                resources->currentState = STATE_PLAY;
                paused = false;
                countdownPause(resources);
                break;
            case 1: // CONTROLS
                resources->currentState = STATE_CONTROLS;
                paused = false;
                break;
            case 2: // SETTINGS
                resources->currentState = STATE_SETTINGS;
                paused = false;
                break;
            case 3: // MAIN MENU
                if (confirmBack(resources)) {
                    resources->currentState = STATE_MAIN_MENU;
                    resources->prevState = STATE_MAIN_MENU;
                    paused = false;
                    break;
                }
                break;
            case 4: // QUIT
                resources->prevState = STATE_PAUSE;
                resources->currentState = STATE_QUIT;
                paused = false;
                break;
            }
        }
        if (IsKeyPressed(KEY_P)) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            resources->currentState = STATE_PLAY;
            paused = false;
            countdownPause(resources);
        }
    }
}

void countdownPause(GameResources* resources) {
    if (resources->currentState == STATE_PLAY) {

        float counter = 3.0f;
        while (counter > 0.0f && !WindowShouldClose()) {
            counter -= GetFrameTime();
            BeginDrawing();
            ClearBackground(RAYWHITE);
            char text[8];
            sprintf(text, "%d", (int)ceilf(counter));
            Vector2 position = { auto_y(230), auto_x(300) };
            DrawTextEx(GetFontDefault(), text, position, auto_y(50), auto_x(2), RED);
            EndDrawing();
        }
    }
}

void selectMode(GameResources* resources) {
    resources->prevState = STATE_SELECT_LEVEL;
    int currentSelection = resources->settings.mode;
    int targetSelection = currentSelection;
    int lineCount = 11;
    bool selecting = true;
    float transition = 0.0f;
    float transitionSpeed = 4.0f; // Kecepatan transisi, bisa disesuaikan
    int transitionDirection = 0;  // -1 untuk kiri, 1 untuk kanan

    while (selecting && !WindowShouldClose()) {
        // Update transisi
        float deltaTime = GetFrameTime();
        if (transition < 1.0f && transitionDirection != 0) {
            transition += deltaTime * transitionSpeed;
            if (transition >= 1.0f) {
                transition = 0.0f;
                currentSelection = targetSelection;
                transitionDirection = 0;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw current image
        float imgScale = (float)GetScreenHeight() / BGMODE(resources, currentSelection).height;
        float scaledWidth = BGMODE(resources, currentSelection).width * imgScale;
        float baseXPos = (GetScreenWidth() - scaledWidth) / 2;

        // Calculate positions for transition
        float currentXPos = baseXPos;
        float nextXPos = baseXPos;

        if (transitionDirection != 0) {
            currentXPos += (transitionDirection * GetScreenWidth() * -transition);
            nextXPos += (transitionDirection * GetScreenWidth() * (1.0f - transition));

            // Draw next image
            DrawTextureEx(BGMODE(resources, targetSelection), (Vector2) { nextXPos, 0 }, 0.0f, imgScale, WHITE);
        }

        DrawTextureEx(BGMODE(resources, currentSelection), (Vector2) { currentXPos, 0 }, 0.0f, imgScale, WHITE);

        imgScale = (float)GetScreenHeight() / TXMODE(resources, currentSelection).height;
        scaledWidth = TXMODE(resources, currentSelection).width * imgScale;
        baseXPos = (GetScreenWidth() - scaledWidth) / 2;
        DrawTextureEx(TXMODE(resources, currentSelection), (Vector2) { baseXPos, 0 }, 0.0f, imgScale, WHITE);
        EndDrawing();

        // Handle input only when not transitioning
        if (transitionDirection == 0) {
            if (MOVE_LEFT || MOVE_DOWN) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                targetSelection = currentSelection - 1;
                if (targetSelection < 0) targetSelection = lineCount - 1;
                transitionDirection = -1;
                transition = 0.0f;
            }
            if (MOVE_RIGHT || MOVE_UP) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                targetSelection = currentSelection + 1;
                if (targetSelection >= lineCount) targetSelection = 0;
                transitionDirection = 1;
                transition = 0.0f;
            }
            if (OK_KEY) {
                PlaySound(SOUND(resources, SOUND_SELECT));
                resources->currentState = STATE_PLAY;
                selecting = false;
                resources->settings.mode = currentSelection;
                saveSettings(resources->settings);
                resources->gameLevel = currentSelection;
                countdownPause(resources);
            }
            if (BACK_KEY) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                resources->currentState = STATE_MAIN_MENU;
                selecting = false;
            }
        }
    }
}

bool confirmReset(GameResources* resources) {

    const char* message = "Reset all high scores?";
    const char* options[] = { "Yes", "No" };
    int selection = 1;
    int fontSize = auto_y(20);
    int lineCount = len(options);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        drawBG(resources, BG_CONFIRM);
        Vector2 textSize = MeasureTextEx(FONT(resources, FONT_HEADER), message, fontSize, 2);
        int startX = (GetScreenWidth() - textSize.x) / 2;
        int startY = (GetScreenHeight() - textSize.y) / 2 - 50;
        DrawTextEx(FONT(resources, FONT_HEADER), message, (Vector2) { startX, startY }, fontSize, 2, DARKGRAY);

        startY += textSize.y + auto_y(50);
        for (int i = 0; i < lineCount; i++) {
            textSize = MeasureTextEx(FONT(resources, FONT_HEADER), options[i], fontSize, 2);
            startX = (GetScreenWidth() - textSize.x) / 2;
            DrawTextEx(FONT(resources, FONT_BODY), options[i], (Vector2) { startX, startY }, fontSize, 2, selection == i ? BLUE : DARKGRAY);
            startY += textSize.y + (auto_y(20));
        }
        EndDrawing();

        if (MOVE_UP || MOVE_LEFT) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            selection--;
            if (selection < 0) selection = lineCount - 1;
        }
        if (MOVE_DOWN || MOVE_RIGHT) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            selection++;
            if (selection >= lineCount) selection = 0;
        }
        if (OK_KEY) {
            PlaySound(SOUND(resources, SOUND_SELECT));
            return selection == 0;
        }
        if (IsKeyPressed(KEY_N) || BACK_KEY) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            return false;
        }
    }
    return false;
}

void resetHiScores(GameResources* resources) {
    if (resources->prevState == STATE_PAUSE) {
        rejectReset(resources);
        return;
    }
    if (confirmReset(resources)) {
        HiScore scores[MAX_LEVELS];
        initializeDb(scores);
        saveHiScores(scores);
    }
    else return;
}

void rejectReset(GameResources* resources) {

    const char* message = "Cannot reset scores during gameplay!";
    const char* subMessage = "Press any key to go back";

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        drawBG(resources, BG_CONFIRM);
        Vector2 textSize = MeasureTextEx(FONT(resources, FONT_HEADER), message, 20, 2);
        int startX = (GetScreenWidth() - textSize.x) / 2;
        int startY = (GetScreenHeight() - textSize.y) / 2 - 20;
        DrawTextEx(FONT(resources, FONT_HEADER), message, (Vector2) { startX, startY }, 20, 2, RED);

        Vector2 subTextSize = MeasureTextEx(FONT(resources, FONT_BODY), subMessage, 15, 2);
        startX = (GetScreenWidth() - subTextSize.x) / 2;
        startY += textSize.y + 20;
        DrawTextEx(FONT(resources, FONT_BODY), subMessage, (Vector2) { startX, startY }, 15, 2, DARKGRAY);

        EndDrawing();

        if (GetKeyPressed() != 0) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            break;
        }
    }
}

void gameOver(GameResources* resources, ll currentScore) {

    const char* message = "GAME OVER";
    const char* options[] = { "Retry", "Main Menu" };
    int selection = 1;
    int fontSize = auto_y(20);
    int lineCount = len(options);
    bool inGameOver = true;
    PlaySound(SOUND(resources, SOUND_DEATH));

    // Load current high scores untuk tampilan
    HiScore scores[MAX_LEVELS];
    loadHiScores(scores);
    ll currentHighScore = scores[resources->gameLevel].score;

    float countdown = 2.0f;  // 2 second countdown
    bool canSelect = false;

    while (inGameOver && !WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        countdown -= deltaTime;
        if (countdown <= 0) canSelect = true;

        BeginDrawing();
        ClearBackground(RAYWHITE);
        drawBG(resources, BG_PLAIN);
        Vector2 textSize = MeasureTextEx(FONT(resources, FONT_HEADER), message, 30, 2);
        int startX = (GetScreenWidth() - textSize.x) / 2;
        int startY = (GetScreenHeight() - textSize.y) / 2 - 200;
        DrawTextEx(FONT(resources, FONT_HEADER), message, (Vector2) { startX, startY }, 30, 2, RAYWHITE);
        startY += 100;

        char highScoreText[50];
        sprintf(highScoreText, "High Score: %lld", currentHighScore);
        Vector2 highScoreSize = MeasureTextEx(FONT(resources, FONT_BODY), highScoreText, fontSize, 2);
        if (currentScore > (resources->scores[resources->gameLevel].score)) {
            const char* newTag = "[NEW]";
            Vector2 newTagSize = MeasureTextEx(FONT(resources, FONT_BODY), newTag, 12, 2);
            Rectangle newBox = {
                (GetScreenWidth() - highScoreSize.x) / 2,
                startY + auto_y(3),
                newTagSize.x + 10,
                newTagSize.y + auto_y(6)
            };
            DrawRectangle(newBox.x, newBox.y, newBox.width, newBox.height, ORANGE);
            DrawTextEx(FONT(resources, FONT_BODY), newTag,
                (Vector2) {
                newBox.x + 5,
                    newBox.y + 3
            },
                12, 2, WHITE);
            startY += 25;
        }

        // Draw High Score
        DrawTextEx(FONT(resources, FONT_BODY), highScoreText,
            (Vector2) {
            (GetScreenWidth() - highScoreSize.x) / 2,
                startY
        },
            fontSize, 2, RAYWHITE);

        // 3. Draw Current Score
        startY += 30;
        char scoreText[50];
        sprintf(scoreText, "Score: %lld", currentScore);
        Vector2 scoreSize = MeasureTextEx(FONT(resources, FONT_BODY), scoreText, fontSize, 2);
        DrawTextEx(FONT(resources, FONT_BODY), scoreText,
            (Vector2) {
            (GetScreenWidth() - scoreSize.x) / 2,
                startY
        },
            fontSize, 2, ORANGE);

        startY += 60;
        if (!canSelect) {
            char countdownText[20];
            sprintf(countdownText, "%.1f", countdown);
            Vector2 countSize = MeasureTextEx(FONT(resources, FONT_BODY), countdownText, fontSize, 2);
            DrawTextEx(FONT(resources, FONT_BODY), countdownText,
                (Vector2) {
                (GetScreenWidth() - countSize.x) / 2,
                    startY
            },
                fontSize, 2, RED);
        }
        else {
            startY += 20;
            for (int i = 0; i < lineCount; i++) {
                textSize = MeasureTextEx(FONT(resources, FONT_HEADER), options[i], fontSize, 2);
                startX = (GetScreenWidth() - textSize.x) / 2;
                DrawTextEx(FONT(resources, FONT_BODY), options[i],
                    (Vector2) {
                    startX, startY
                },
                    fontSize, 2,
                    selection == i ? ORANGE : RAYWHITE);
                startY += textSize.y + auto_y(20);
            }

            // Handle input only when countdown is finished
            if (MOVE_UP || MOVE_LEFT) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                selection--;
                if (selection < 0) selection = lineCount - 1;
            }
            if (MOVE_DOWN || MOVE_RIGHT) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                selection++;
                if (selection >= lineCount) selection = 0;
            }
            if (OK_KEY) {
                PlaySound(SOUND(resources, SOUND_SELECT));
                switch (selection) {
                case 0: // Retry
                    resources->currentState = STATE_PLAY;
                    inGameOver = false;
                    countdownPause(resources);
                    break;
                case 1: // Main Menu
                    resources->currentState = STATE_MAIN_MENU;
                    inGameOver = false;
                    break;
                }
            }
        }
        EndDrawing();
    }
}

// =================================================================================================
//                                       ... BLOCKS ...
// =================================================================================================

/*  FALLING BLOCKS LOGIC
 * ====================== */

float getSpeedForMode(Game* game, int mode) {
    switch (mode) {
    case 0: return MD1_SPEED;
    case 1: return MD2_SPEED;
    case 2: return MD3_SPEED;
    case 3: return MD4_SPEED;
    case 4: return MD5_SPEED;
    case 5: return MD6_SPEED;
    case 6: return MD7_SPEED;
    case 7: return MD8_SPEED;
    case 8: return MD9_SPEED;
    case 9: return MD10_SPEED;
    case 10: return MD1_SPEED * (1.0f + game->frameCounter / 3600.0f); // Progressive speed for Progressive
    default: return MD1_SPEED;
    }
}

static void getBlockRangeForMode(int mode, int* minBlocks, int* maxBlocks) {
    switch (mode) {
    case 0: MD1_RANGE; // Super EZ
    case 1: MD2_RANGE; // Easy
    case 2: MD3_RANGE; // Beginner
    case 3: MD4_RANGE; // Medium
    case 4: MD5_RANGE; // Hard
    case 5: MD6_RANGE; // Super Hard
    case 6: MD7_RANGE; // Expert
    case 7: MD8_RANGE; // Master
    case 8: MD9_RANGE; // Legend
    case 9: MD10_RANGE; // God
    case 10: MD11_RANGE; // Progressive
    default: MD1_RANGE;
    }
}

void displayGame(GameResources* resources) {

    if (!resources) return;
    resources->prevState = STATE_PLAY;
    static Game* gameContext = NULL;
    UpdateMusicStream(soundGameplay);
    // Inisialisasi new game
    if (!gameContext || (resources->currentState == STATE_PLAY && resources->prevState != STATE_PLAY)) {
        if (gameContext) {
            destroyGameContext(gameContext);
        }
        gameContext = createGameContext();
        initBlocks(gameContext, resources);
    }

    if (!gameContext->gameOver) {
        float deltaTime = GetFrameTime();
        gameContext->blockFallTimer += deltaTime;

        gameContext->powerupTimer -= deltaTime;
        if (gameContext->powerupTimer <= 0 && !gameContext->powerupActive) {
            spawnPowerUp(gameContext);
            gameContext->powerupTimer = 3.0f;
        }

        updatePowerUp(gameContext, resources);
        updateBlocks(gameContext, resources);

        if (isGameOverCheck(gameContext)) {
            gameContext->gameOver = true;
            resources->currentState = STATE_GAME_OVER;
            updateHighScore(gameContext, resources);
            gameOver(resources, gameContext->score);
            delete(gameContext);
            gameContext = NULL;
            return;
        }
    }


    BeginDrawing();
    ClearBackground(WHITE);

    float blockSize = auto_x(32);
    float gameWidth = blockSize * MAX_COLUMNS;
    float gameHeight = blockSize * MAX_ROWS - blockSize;

    float gameAreaX = 0;
    float gameAreaY = 0;

    // main game area
    DrawRectangle(gameAreaX, gameAreaY, gameWidth, GetScreenHeight(), (Color) { 236, 244, 255, 255 });

    float sidebarX = gameWidth;
    float sidebarWidth = GetScreenWidth() - gameWidth;

    // Left border
    DrawRectangle(sidebarX, 0, auto_x(3), GetScreenHeight(), (Color) { 27, 45, 4, 255 });

    // Middle border
    DrawRectangle(sidebarX + auto_x(3), 0, auto_x(7), GetScreenHeight(), (Color) { 65, 71, 71, 255 });

    // UI area
    DrawRectangle(sidebarX + auto_x(10), 0, sidebarWidth - auto_x(10), GetScreenHeight(), (Color) { 25, 38, 47, 255 });

    // Bottom boundary 
    DrawRectangle(gameAreaX, gameHeight, gameWidth, auto_y(1), BLACK);


    // (Color) { 25, 38, 47, 255 }
    drawBlocks(gameContext, resources);
    drawPowerUp(gameContext, resources);

    if (IsKeyPressed(KEY_E) && gameContext->laserCooldown <= 0) {
        gameContext->laserActive = true;
        gameContext->laserDuration = 5.0f;
        gameContext->laserCooldown = 15.0f; // 5s aktif + 10s cooldown
    }

    if (gameContext->laserActive) {
        gameContext->laserDuration -= GetFrameTime();
        if (gameContext->laserDuration <= 0) {
            gameContext->laserActive = false;
        }
        handleLaser(gameContext);
    }

    if (gameContext->laserCooldown > 0) {
        gameContext->laserCooldown -= GetFrameTime();
    }

    // Update powerups
    float deltaTime = GetFrameTime();
    for (int i = 0; i < gameContext->activeEffectsCount; i++) {
        if (gameContext->activePowerups[i].active) {
            gameContext->activePowerups[i].duration -= deltaTime;

            if (gameContext->activePowerups[i].duration <= 0) {
                // Reverse efek
                switch (gameContext->activePowerups[i].type) {
                case POWERUP_SPEED_UP:
                    gameContext->rowAddDelay *= 4;
                    break;
                case POWERUP_SLOW_DOWN:
                    gameContext->rowAddDelay = (int)(gameContext->rowAddDelay / 2.5f);
                    break;
                }

                // Hapus efek
                for (int j = i; j < gameContext->activeEffectsCount - 1; j++) {
                    gameContext->activePowerups[j] = gameContext->activePowerups[j + 1];
                }
                gameContext->activeEffectsCount--;
                i--;
            }
        }
    }
    drawGameUI(gameContext, resources);
    if (gameContext->lives <= 0) {
        gameContext->gameOver = true;
        resources->currentState = STATE_GAME_OVER;
        updateHighScore(gameContext, resources);
        gameOver(resources, gameContext->score);
        delete(gameContext);
        gameContext = NULL;
        return;
    }
    // Debug grid visualization
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        for (int i = 0; i <= MAX_ROWS; i++) {
        }
        for (int j = 0; j <= MAX_COLUMNS; j++) {
        }
    }

    // Player and bullets
    shooter(&P.x, &P.y, resources);
    moveSet(&P.x, resources);

    MoveBullets(gameContext->bullets);
    DrawBullets(gameContext->bullets, resources);

    // Handle input
    if (SHOOT_KEY) {
        ShootBullets(gameContext->bullets, (Vector2) { P.x, P.y }, & gameContext->bulletCount,
            & gameContext->canShoot, 0, resources);
    }
    if (SHOOT_RELEASE) {
        gameContext->canShoot = true;
    }
    EndDrawing();

    // Keperluan debug
    // if (IsKeyPressed(KEY_G)) {
    //     gameOver(resources);
    // }

    if (IsKeyPressed(KEY_P)) {
        pauseMenu(resources);
        if (resources->currentState == STATE_MAIN_MENU) {
            updateHighScore(gameContext, resources);
            destroyGameContext(gameContext);
            gameContext = NULL;
            return;
        }
    }

    if (IsKeyPressed(KEY_F1)) {
        printGrid(gameContext);
    }
}

bool isGameOverCheck(Game* game) {
    if (!game || !game->grid) return false;

    // Get the last row (tail) of the grid
    DLLNode* lastRowNode = game->grid->tail;
    if (!lastRowNode) return false;

    // Get the linked list representing the last row
    DoublyLinkedList* lastRow = (DoublyLinkedList*)lastRowNode->data;
    if (!lastRow) return false;

    // Check each block in the last row
    DLLNode* blockNode = lastRow->head;
    while (blockNode) {
        Block* block = (Block*)blockNode->data;
        if (block && block->active) {
            return true;
        }
        blockNode = blockNode->next;
    }

    return false;
}

bool isRowFull(Game* game, int row) {
    if (!game || !game->grid) return false;

    // Find the desired row node
    DLLNode* rowNode = game->grid->head;
    for (int i = 0; i < row && rowNode; i++) {
        rowNode = rowNode->next;
    }

    if (!rowNode) return false;

    // Get the linked list representing this row
    DoublyLinkedList* currentRow = (DoublyLinkedList*)rowNode->data;
    if (!currentRow) return false;

    // Check each block in this row
    DLLNode* blockNode = currentRow->head;
    while (blockNode) {
        Block* block = (Block*)blockNode->data;
        if (!block || !block->active) {
            return false;
        }
        blockNode = blockNode->next;
    }

    return true;
}

bool hasActiveBlockBelow(Game* game, int row) {
    if (!game || !game->grid) return false;

    // Find the row node
    DLLNode* rowNode = game->grid->head;
    for (int i = 0; i < row && rowNode; i++) {
        rowNode = rowNode->next;
    }

    if (!rowNode) return false;

    // Check all rows below current row
    rowNode = rowNode->next;
    while (rowNode) {
        DoublyLinkedList* currentRow = (DoublyLinkedList*)rowNode->data;
        if (!currentRow) continue;

        // Check each block in this row
        DLLNode* blockNode = currentRow->head;
        while (blockNode) {
            Block* block = (Block*)blockNode->data;
            if (block && block->active) {
                return true;
            }
            blockNode = blockNode->next;
        }
        rowNode = rowNode->next;
    }

    return false;
}

void shiftRowsUp(Game* game, int startRow) {
    if (!game || !game->grid) return;

    // Find the starting row node
    DLLNode* rowNode = game->grid->head;
    for (int i = 0; i < startRow && rowNode; i++) {
        rowNode = rowNode->next;
    }

    // Shift rows up from startRow to second-to-last row
    while (rowNode && rowNode->next) {
        DoublyLinkedList* currentRow = (DoublyLinkedList*)rowNode->data;
        DoublyLinkedList* nextRow = (DoublyLinkedList*)rowNode->next->data;

        // Copy blocks from next row to current row
        DLLNode* currentBlock = currentRow->head;
        DLLNode* nextBlock = nextRow->head;

        while (currentBlock && nextBlock) {
            Block* current = (Block*)currentBlock->data;
            Block* next = (Block*)nextBlock->data;
            current->active = next->active;

            currentBlock = currentBlock->next;
            nextBlock = nextBlock->next;
        }

        rowNode = rowNode->next;
    }

    // Clear the last row
    if (rowNode) {
        DoublyLinkedList* lastRow = (DoublyLinkedList*)rowNode->data;
        DLLNode* blockNode = lastRow->head;
        while (blockNode) {
            Block* block = (Block*)blockNode->data;
            block->active = false;
            blockNode = blockNode->next;
        }
    }
}

void handleFullRow(Game* game, int row) {
    if (isRowFull(game, row)) {
        if (hasActiveBlockBelow(game, row)) {
            shiftRowsUp(game, row);
            addScore(game, row);
        }
        else {
            // Clear blocks in the specified row
            DLLNode* rowNode = game->grid->head;
            for (int i = 0; i < row && rowNode; i++) {
                rowNode = rowNode->next;
            }

            if (rowNode) {
                DoublyLinkedList* currentRow = (DoublyLinkedList*)rowNode->data;
                DLLNode* blockNode = currentRow->head;
                while (blockNode) {
                    Block* block = (Block*)blockNode->data;
                    block->active = false;
                    blockNode = blockNode->next;
                }
            }
            addScore(game, row);
        }
    }
}

void handleBlockMovement(Game* game, int minBlocks, int maxBlocks) {
    int emptyColLength[MAX_COLUMNS] = { 0 };
    int totalEmptyColumns = 0;

    // Count empty columns from top
    DLLNode* rowNode = game->grid->head;
    for (int j = 0; j < MAX_COLUMNS; j++) {
        int emptyCount = 0;
        DLLNode* currentRowNode = rowNode;

        while (currentRowNode) {
            DoublyLinkedList* row = (DoublyLinkedList*)currentRowNode->data;
            DLLNode* blockNode = row->head;

            // Navigate to correct column
            for (int col = 0; col < j && blockNode; col++) {
                blockNode = blockNode->next;
            }

            if (blockNode) {
                Block* block = (Block*)blockNode->data;
                if (!block->active) {
                    emptyCount++;
                }
                else {
                    break;
                }
            }
            currentRowNode = currentRowNode->next;
        }

        emptyColLength[j] = emptyCount;
        if (emptyCount >= 4) {
            totalEmptyColumns++;
        }
    }

    moveBlocksDown(game);
    generateNewBlocks(game, minBlocks, maxBlocks, emptyColLength, totalEmptyColumns);
}

void moveBlocksDown(Game* game) {
    if (!game || !game->grid) return;

    // Start from the second-to-last row (tail->prev) and move up
    DLLNode* rowNode = game->grid->tail->prev;

    while (rowNode) {
        DoublyLinkedList* currentRow = (DoublyLinkedList*)rowNode->data;
        DoublyLinkedList* nextRow = (DoublyLinkedList*)rowNode->next->data;

        // Copy blocks from current row to next row
        DLLNode* currentBlock = currentRow->head;
        DLLNode* nextBlock = nextRow->head;

        while (currentBlock && nextBlock) {
            Block* current = (Block*)currentBlock->data;
            Block* next = (Block*)nextBlock->data;
            next->active = current->active;

            currentBlock = currentBlock->next;
            nextBlock = nextBlock->next;
        }

        rowNode = rowNode->prev;
    }

    // Clear the first row (head)
    DoublyLinkedList* firstRow = (DoublyLinkedList*)game->grid->head->data;
    DLLNode* blockNode = firstRow->head;
    while (blockNode) {
        Block* block = (Block*)blockNode->data;
        block->active = false;
        blockNode = blockNode->next;
    }
}

void generateNewBlocks(Game* game, int minBlocks, int maxBlocks, int* emptyColLength, int totalEmptyColumns) {
    if (!game || !game->grid) return;

    int numBlocks = minBlocks + (rand() % (maxBlocks - minBlocks + 1));
    int remainingBlocks = numBlocks;

    // Get the first row
    DLLNode* firstRowNode = game->grid->head;
    if (!firstRowNode) return;

    DoublyLinkedList* firstRow = (DoublyLinkedList*)firstRowNode->data;
    if (!firstRow) return;

    // Step 1: Fill critical gaps first (kolom dengan 4+ blok kosong berturut-turut)
    if (totalEmptyColumns > 0) {
        DLLNode* blockNode = firstRow->head;
        int col = 0;

        while (blockNode && remainingBlocks > 0) {
            if (emptyColLength[col] >= 4) {
                Block* block = (Block*)blockNode->data;
                block->active = true;
                remainingBlocks--;
            }
            blockNode = blockNode->next;
            col++;
        }
    }

    // Step 2: Distribusi blok dengan gap maksimal 2 kolom
    if (remainingBlocks > 0) {
        int lastPlacedCol = -3; // Start dengan offset negatif
        int attempts = 0;
        int maxAttempts = MAX_COLUMNS * 2; // Prevent infinite loop

        while (remainingBlocks > 0 && attempts < maxAttempts) {
            int pos = rand() % MAX_COLUMNS;

            // Cek apakah posisi valid (tidak terlalu dekat dengan blok terakhir)
            if (pos - lastPlacedCol >= 2) {
                DLLNode* blockNode = firstRow->head;
                for (int i = 0; i < pos && blockNode; i++) {
                    blockNode = blockNode->next;
                }

                if (blockNode) {
                    Block* block = (Block*)blockNode->data;
                    if (!block->active) {
                        block->active = true;
                        lastPlacedCol = pos;
                        remainingBlocks--;
                    }
                }
            }
            attempts++;
        }

        // Step 3: Jika masih ada blok tersisa, tempatkan di posisi random
        while (remainingBlocks > 0) {
            int pos = rand() % MAX_COLUMNS;
            DLLNode* blockNode = firstRow->head;

            for (int i = 0; i < pos && blockNode; i++) {
                blockNode = blockNode->next;
            }

            if (blockNode) {
                Block* block = (Block*)blockNode->data;
                if (!block->active) {
                    block->active = true;
                    remainingBlocks--;
                }
            }
        }
    }
}

int fillCriticalGaps(Game* game, int remainingBlocks, int* emptyColLength) {
    if (!game || !game->grid) return remainingBlocks;

    // Get first row
    DLLNode* firstRowNode = game->grid->head;
    if (!firstRowNode) return remainingBlocks;

    DoublyLinkedList* firstRow = (DoublyLinkedList*)firstRowNode->data;
    if (!firstRow) return remainingBlocks;

    DLLNode* blockNode = firstRow->head;
    int col = 0;

    // Check each column for critical gaps
    while (blockNode && remainingBlocks > 0) {
        if (emptyColLength[col] >= 4) {
            Block* block = (Block*)blockNode->data;
            block->active = true;
            remainingBlocks--;
        }
        blockNode = blockNode->next;
        col++;
    }

    return remainingBlocks;
}

void fillRemainingBlocks(Game* game, int remainingBlocks) {
    int maxAttemptsPerBlock = 3;

    // Get first row
    DLLNode* firstRowNode = game->grid->head;
    if (!firstRowNode) return;

    DoublyLinkedList* firstRow = (DoublyLinkedList*)firstRowNode->data;
    if (!firstRow) return;

    while (remainingBlocks > 0) {
        for (int attempt = 0; attempt < maxAttemptsPerBlock && remainingBlocks > 0; attempt++) {
            int pos = rand() % MAX_COLUMNS;

            // Navigate to the desired column
            DLLNode* blockNode = firstRow->head;
            for (int i = 0; i < pos && blockNode; i++) {
                blockNode = blockNode->next;
            }

            if (blockNode) {
                Block* block = (Block*)blockNode->data;
                if (!block->active) {
                    block->active = true;
                    remainingBlocks--;
                    break;
                }
            }
        }
        remainingBlocks--; // Prevent infinite loop if can't place all blocks
    }
}

void handleBulletCollisions(Game* game) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (game->bullets[i].active) {
            float blockSize = auto_x(32);

            int gridX = (int)(game->bullets[i].position.x / blockSize);
            int gridY = (int)(game->bullets[i].position.y / blockSize);

            if (isValidGridPosition(gridX, gridY)) {
                DLLNode* rowNode = game->grid->head;
                for (int y = 0; y < gridY && rowNode; y++) {
                    rowNode = rowNode->next;
                }

                if (rowNode) {
                    DoublyLinkedList* row = (DoublyLinkedList*)rowNode->data;

                    DLLNode* blockNode = row->head;
                    for (int x = 0; x < gridX && blockNode; x++) {
                        blockNode = blockNode->next;
                    }

                    if (blockNode) {
                        Block* block = (Block*)blockNode->data;
                        if (block->active) {
                            processBulletHit(game, gridX, gridY, i);
                        }
                    }
                }
            }
        }
    }
}

bool isValidGridPosition(int x, int y) {
    return x >= 0 && x < MAX_COLUMNS && y >= 0 && y < MAX_ROWS;
}

void processBulletHit(Game* game, int gridX, int gridY, int bulletIndex) {
    bool hasSpecialBullet = false;
    for (int i = 0; i < game->activeEffectsCount; i++) {
        if (game->activePowerups[i].active &&
            game->activePowerups[i].type == POWERUP_SPECIAL_BULLET) {
            hasSpecialBullet = true;
            break;
        }
    }

    // Find the target row
    DLLNode* rowNode = game->grid->head;
    for (int y = 0; y < gridY && rowNode; y++) {
        rowNode = rowNode->next;
    }

    if (hasSpecialBullet && rowNode) {
        // Clear entire row for special bullet
        DoublyLinkedList* row = (DoublyLinkedList*)rowNode->data;
        DLLNode* blockNode = row->head;
        while (blockNode) {
            Block* block = (Block*)blockNode->data;
            block->active = false;
            blockNode = blockNode->next;
        }
        game->score += 40;
    }
    else if (gridY < MAX_ROWS - 1) {
        // Set block below target to active
        DLLNode* nextRowNode = rowNode->next;
        if (nextRowNode) {
            DoublyLinkedList* nextRow = (DoublyLinkedList*)nextRowNode->data;
            DLLNode* blockNode = nextRow->head;
            for (int x = 0; x < gridX && blockNode; x++) {
                blockNode = blockNode->next;
            }
            if (blockNode) {
                Block* block = (Block*)blockNode->data;
                block->active = true;
            }
        }
    }

    // Check for full rows
    for (int row = 0; row < MAX_ROWS; row++) {
        handleFullRow(game, row);
    }

    game->bullets[bulletIndex].active = false;
    game->score += 10;
}

void updateBlocks(Game* game, GameResources* resources) {
    if (!game || !resources) return;

    int minBlocks, maxBlocks;
    getBlockRangeForMode(resources->gameLevel, &minBlocks, &maxBlocks);
    float currentSpeed = getSpeedForMode(game, resources->gameLevel);

    game->rowAddDelay = (int)(60 / currentSpeed);
    game->frameCounter++;

    if (game->frameCounter >= game->rowAddDelay) {
        handleBlockMovement(game, minBlocks, maxBlocks);
        game->frameCounter = 0;
    }

    handleBulletCollisions(game);
}

void initBlocks(Game* game, GameResources* resources) {
    DLLNode* firstRowNode = game->grid->head;
    if (!firstRowNode) return;
    DoublyLinkedList* firstRow = (DoublyLinkedList*)firstRowNode->data;
    if (!firstRow) return;


    DLLNode* rowNode = game->grid->head;
    while (rowNode) {
        DoublyLinkedList* row = (DoublyLinkedList*)rowNode->data;
        DLLNode* blockNode = row->head;
        while (!blockNode) {
            Block* block = (Block*)blockNode->data;
            block->active = false;
            blockNode = blockNode->next;
        }
        rowNode = rowNode->next;
    }

    int minBlocks, maxBlocks;
    getBlockRangeForMode(resources->gameLevel, &minBlocks, &maxBlocks);
    int numBlocks = minBlocks + (rand() % (maxBlocks - minBlocks + 1));

    while (numBlocks > 0) {
        int pos = rand() % MAX_COLUMNS;
        DLLNode* blockNode = firstRow->head;

        // Navigate to desired column
        for (int i = 0; i < pos && blockNode; i++) {
            blockNode = blockNode->next;
        }

        if (blockNode) {
            Block* block = (Block*)blockNode->data;
            if (!block->active) {
                block->active = true;
                numBlocks--;
            }
        }
    }

    game->frameCounter = 0;
    game->score = 0;
    game->gameOver = false;
}

void drawBlocks(Game* game, GameResources* resources) {

    float blockSize = auto_x(32);

    // Iterate through rows
    DLLNode* rowNode = game->grid->head;
    int rowIndex = 0;

    while (rowNode && rowIndex < MAX_ROWS) {
        DoublyLinkedList* row = (DoublyLinkedList*)rowNode->data;
        DLLNode* blockNode = row->head;
        int colIndex = 0;

        // Iterate through blocks in current row
        while (blockNode && colIndex < MAX_COLUMNS) {
            Block* block = (Block*)blockNode->data;
            if (block && block->active) {
                if (TEXTURE(resources, TEXTURE_BLOCK).id != 0) {
                    float texScale = (float)blockSize / (float)TEXTURE(resources, TEXTURE_BLOCK).width;
                    DrawTextureEx(
                        TEXTURE(resources, TEXTURE_BLOCK),
                        (Vector2) {
                        colIndex* blockSize, rowIndex* blockSize
                    },
                        0.0f,
                        texScale,
                        WHITE
                    );
                }
                else {
                    DrawRectangle(
                        colIndex * blockSize,
                        rowIndex * blockSize,
                        blockSize,
                        blockSize,
                        BLUE
                    );
                }
            }
            blockNode = blockNode->next;
            colIndex++;
        }
        rowNode = rowNode->next;
        rowIndex++;
    }
}

void printGrid(Game* game) {
    system("cls");
    printf("\n--- Grid State ---\n");

    DLLNode* rowNode = game->grid->head;
    while (rowNode) {
        DoublyLinkedList* row = (DoublyLinkedList*)rowNode->data;
        DLLNode* blockNode = row->head;

        while (blockNode) {
            Block* block = (Block*)blockNode->data;
            printf(block->active ? "[x]" : "   ");
            blockNode = blockNode->next;
        }
        printf("\n");
        rowNode = rowNode->next;
    }

    printf("-----------------\n");
}

void drawGameUI(Game* game, GameResources* resources) {


    char obj[32];
    Vector2 textSize;
    int startX, areaWidth;
    const int mid = auto_x(405);
    const int linespacing = auto_y(30);
    int curY = 50;

    // Mode ==========================================================
    {
        textSize = MeasureTextEx(GetFontDefault(), "MODE", 20, 2);
        startX = mid - (textSize.x / 2);
        DrawTextEx(GetFontDefault(), "MODE", (Vector2) { startX, curY }, 20, 2, WHITE);
        curY += linespacing;

        sprintf(obj, "%s", gameMode(resources));
        textSize = MeasureTextEx(GetFontDefault(), obj, 18, 2);

        Rectangle rect = { auto_x(345), curY - 5, auto_x(120), textSize.y + 10 };
        DrawRectangle(rect.x, rect.y, rect.width, rect.height, (Color) { 214, 214, 214, 255 });

        startX = rect.x + (rect.width - textSize.x) / 2;
        int centerY = rect.y + (rect.height - textSize.y) / 2;

        DrawTextEx(GetFontDefault(), obj, (Vector2) { startX, centerY }, 18, 2, WHITE);
        curY += linespacing;
        curY += auto_y(30);
    }
    // High-Score ====================================================
    {
        if (game->score > (resources->scores[resources->gameLevel].score)) {
            const char* newTag = "[NEW]";
            Vector2 title = MeasureTextEx(GetFontDefault(), "HIGH-SCORE", 20, 2);
            Vector2 tag = MeasureTextEx(GetFontDefault(), newTag, 12, 2);
            Rectangle newBox = {
                mid - (title.x / 2),
                curY + auto_y(3),
                tag.x + auto_x(10),
                tag.y + auto_y(6)
            };
            DrawRectangle(newBox.x, newBox.y, newBox.width, newBox.height, ORANGE);
            DrawTextEx(GetFontDefault(), newTag,
                (Vector2) {
                newBox.x + 5,
                    newBox.y + auto_y(3)
            },
                12, 2, WHITE);
        }
        curY += auto_y(21);
        textSize = MeasureTextEx(GetFontDefault(), "HIGH-SCORE", 20, 2);
        startX = mid - (textSize.x / 2);
        DrawTextEx(GetFontDefault(), "HIGH-SCORE", (Vector2) { startX, curY }, 20, 2, WHITE);
        curY += linespacing;


        sprintf(obj, "%lld", getMax(resources->scores, game, resources));
        textSize = MeasureTextEx(GetFontDefault(), obj, 18, 2);

        Rectangle rect = { auto_x(345), curY - 5, auto_x(120), textSize.y + 10 };
        DrawRectangle(rect.x, rect.y, rect.width, rect.height, (Color) { 214, 214, 214, 255 });

        startX = rect.x + (rect.width - textSize.x) / 2;
        int centerY = rect.y + (rect.height - textSize.y) / 2;

        DrawTextEx(GetFontDefault(), obj, (Vector2) { startX, centerY }, 18, 2, WHITE);
        curY += linespacing;
        curY += auto_y(30);
    }
    // Score =========================================================
    {
        textSize = MeasureTextEx(GetFontDefault(), "SCORE", 20, 2);
        startX = mid - (textSize.x / 2);
        DrawTextEx(GetFontDefault(), "SCORE", (Vector2) { startX, curY }, 20, 2, WHITE);
        curY += linespacing;


        sprintf(obj, "%lld", playerScore(game));
        textSize = MeasureTextEx(GetFontDefault(), obj, 18, 2);

        Rectangle rect = { auto_x(345), curY - 5, auto_x(120), textSize.y + 10 };
        DrawRectangle(rect.x, rect.y, rect.width, rect.height, (Color) { 214, 214, 214, 255 });

        startX = rect.x + (rect.width - textSize.x) / 2;
        int centerY = rect.y + (rect.height - textSize.y) / 2;

        DrawTextEx(GetFontDefault(), obj, (Vector2) { startX, centerY }, 18, 2, WHITE);
        curY += linespacing;
        curY += auto_y(30);
    }
    {
        textSize = MeasureTextEx(GetFontDefault(), "POWERUP", 20, 2);
        startX = mid - (textSize.x / 2);
        DrawTextEx(GetFontDefault(), "POWER-UP", (Vector2) { startX, curY }, 20, 2, ORANGE);
        curY += linespacing;

        // Power-up icons container
        {
            const int ICON_SIZE = auto_x(40);   // Ukuran yang diinginkan untuk semua icon
            const int SPACING = auto_x(10);     // Spacing antar icon
            const int startIconX = auto_x(345); // Align left dengan hearts

            // Draw active power-ups
            for (int i = 0; i < game->activeEffectsCount; i++) {
                if (game->activePowerups[i].active) {
                    Texture2D iconTexture;
                    Color timerColor;
                    float scale;

                    // Pilih texture, warna, dan scale berdasarkan tipe
                    switch (game->activePowerups[i].type) {
                    case POWERUP_SPEED_UP:
                        iconTexture = TEXTURE(resources, TEXTURE_SPEEDUP);
                        scale = (float)ICON_SIZE / 640.0f;
                        timerColor = GREEN;
                        break;
                    case POWERUP_SLOW_DOWN:
                        iconTexture = TEXTURE(resources, TEXTURE_SLOWDOWN);
                        scale = (float)ICON_SIZE / 1024.0f;
                        timerColor = RED;
                        break;
                    case POWERUP_SPECIAL_BULLET:
                        iconTexture = TEXTURE(resources, TEXTURE_SPECIAL_BULLET); // Temporary
                        scale = (float)ICON_SIZE / 1024.0f;
                        timerColor = BLUE;
                        break;
                    default:
                        iconTexture = TEXTURE(resources, TEXTURE_RANDOM);
                        scale = (float)ICON_SIZE / 1024.0f;
                        timerColor = WHITE;
                    }

                    int iconX = startIconX + (i * (ICON_SIZE + SPACING));

                    // Draw icon
                    DrawTextureEx(iconTexture,
                        (Vector2) {
                        iconX, curY
                    },
                        0, scale, WHITE);

                    // Draw timer below icon
                    char timerText[8];
                    sprintf(timerText, "%.1fs", game->activePowerups[i].duration);
                    Vector2 timerSize = MeasureTextEx(GetFontDefault(), timerText, 15, 2);
                    float timerX = iconX + (ICON_SIZE - timerSize.x) / 2;
                    float timerY = curY + ICON_SIZE + 5;

                    DrawTextEx(GetFontDefault(), timerText,
                        (Vector2) {
                        timerX, timerY
                    },
                        15, 2, timerColor);
                }
            }

            curY += ICON_SIZE + auto_y(40);
        }
    }
    textSize = MeasureTextEx(GetFontDefault(), "HITPOINT", 18, 2);
    startX = mid - (textSize.x / 2);
    DrawTextEx(GetFontDefault(), "HITPOINT", (Vector2) { auto_x(345), auto_y(480) - textSize.y }, 18, 2, WHITE);
    curY += linespacing;
    for (int i = 0; i < game->lives; i++) {
        float local_scale = 30.0f / 640.0f;
        DrawTextureEx(TEXTURE(resources, TEXTURE_HEART),
            (Vector2) {
            auto_x(345) + (i * auto_x(35)), auto_y(480)
        }, // posisi
            0,  // rotation
            local_scale, // local_scale factor
            WHITE);
    }

    float local_scale = (float)auto_x(80.0f) / 640.0f;
    Vector2 buttonPos = (Vector2){ auto_x(345), auto_y(540) };
    Rectangle buttonRect = {
        buttonPos.x,
        buttonPos.y,
        TEXTURE(resources, TEXTURE_LASER_BUTTON).width * local_scale,
        TEXTURE(resources, TEXTURE_LASER_BUTTON).height * local_scale
    };

    if (game->laserCooldown > 0) {
        DrawTextureEx(TEXTURE(resources, TEXTURE_LASER_BUTTON),
            buttonPos, 0, local_scale,
            (Color) {
            255, 255, 255, 80
        });

        char cooldownText[5];
        sprintf(cooldownText, "%.1f", game->laserCooldown);
        Vector2 textSize = MeasureTextEx(GetFontDefault(), cooldownText, 20, 2);
        float textX = buttonRect.x + (buttonRect.width - textSize.x) / 2;
        float textY = buttonRect.y + (buttonRect.height - textSize.y) / 2;
        DrawTextEx(GetFontDefault(), cooldownText,
            (Vector2) {
            textX, textY
        }, 20, 2, WHITE);
    }
    else {
        DrawTextureEx(TEXTURE(resources, TEXTURE_LASER_BUTTON),
            buttonPos, 0, local_scale, WHITE);
    }
    textSize = MeasureTextEx(GetFontDefault(), "E", 15, 2);
    float boxX = buttonRect.x + buttonRect.width - (textSize.x + auto_x(8)); // 8 untuk padding
    float boxY = buttonRect.y + buttonRect.height - (textSize.y + auto_y(8));

    DrawRectangle(boxX, boxY, textSize.x + auto_x(8), textSize.y + auto_y(8), DARKGRAY);
    DrawTextEx(GetFontDefault(), "E",
        (Vector2) {
        boxX + auto_x(4),  // +4 untuk padding
            boxY + auto_y(4)   // +4 untuk padding
    },
        15, 2, WHITE);
}