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
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLUMNS; j++) {
            game->grid[i][j] = false;
        }
    }

    return game;
}

void destroyGameContext(Game* game) {
    if (!game) return;
    free(game);
}

// =======================================
//          Tipe Data Bentukan
// =======================================

/* Linked List Operations
 * ====================== */
void addBlock(BlockList* list, const int x, const int y, const int jumlah) {
    Block* newBlock = new(Block);

    newBlock->x = x;
    newBlock->y = y;
    newBlock->jumlah = jumlah;

    newBlock->next = list->head;
    list->head = newBlock;

    if (list->size == 0) {
        list->tail = newBlock;
    }

    list->size++;
}

void updateBlock(BlockList* list, const int pos, const int x, const int y, int jumlah) {
    if (pos < 1 || pos > list->size) {
        printf("position out of range\n");
        return;
    }
    if (pos == list->size) {
        updateBlock(list, countBlock(list), x, y, jumlah);
        return;
    }

    Block* cur = list->head;
    for (int i = 1; i < pos; i++) {
        cur = cur->next;
    }

    cur->x = x;
    cur->y = y;
    cur->jumlah = jumlah;
}

void removeBlock(BlockList* list, int pos) {
    if (pos < -1 || pos > list->size || pos == 0) {
        printf("position out of range\n");
        return;
    }
    if (pos == -1) {
        removeBlock(list, countBlock(list));
    }

    Block* cur = list->head;
    Block* before = NULL;
    for (int i = 1; i < pos; ++i) {
        before = cur;
        cur = cur->next;
    }

    before->next = cur->next;
    delete(cur);

    list->size--;
}

void printBlockList(BlockList* list) {
    printf("Number of Blocks: %d\n", countBlock(list));
    Block* cur = list->head;
    int i = 1;
    while (cur != NULL) {
        printf("Block %d\n", i);
        printf("{%d, %d}\tjumlah: %d\n", cur->x, cur->y, cur->jumlah);
        cur = cur->next;
        i++;
    }
}

int countBlock(BlockList* list) {
    Block* cur = list->head;
    int count = 0;
    while (cur != NULL) {
        count++;
        cur = cur->next;
    }
    return count;
}

/*    Queue Operations
 * ====================== */
void initQueue(BlockQueue* q) {
    q->front = -1;
    q->rear = -1;
}

int isEmpty(BlockQueue* q) {
    return q->rear == -1;
}

int isFull(BlockQueue* q) {
    return q->rear == MAX_WIDTH_BLOCKS - 1;
}

int countQueue(BlockQueue* q) {
    if (isEmpty(q)) return 0;
    return q->rear - q->front + 1;
}

void enqueue(BlockQueue* q, Block* element) {
    if (isFull(q)) {
        printf("BlockQueue is full!\n");
    }
    else {
        if (q->front == -1) q->front = 0;
        q->rear++;
        q->items[q->rear] = element;
        printf("Inserted -> %p\n", (void*)element);
    }
}

Block* dequeue(BlockQueue* q) {
    if (isEmpty(q)) {
        printf("BlockQueue is empty!\n");
        return NULL;
    }
    else {
        Block* element = q->items[q->front];
        if (q->front >= q->rear) {
            q->front = -1;
            q->rear = -1;
        }
        else {
            q->front++;
        }
        printf("Deleted -> %p\n", (void*)element);
        return element;
    }
}

void displayQueue(BlockQueue* q) {
    if (isEmpty(q)) {
        printf("BlockQueue is empty!\n");
    }
    else {
        printf("BlockQueue elements are:\n");
        for (int i = q->front; i <= q->rear; i++)
            printf("%p ", (void*)q->items[i]);
        printf("\n");
    }
}

void clearQueue(BlockQueue* q) {
    q->front = -1;
    q->rear = -1;
    printf("BlockQueue cleared!\n");
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

// Tambahkan fungsi ini di arief.c
Assets* createAssets(void) {
    Assets* assets = new(Assets);
    if (!assets) return NULL;

    // Load sounds
    assets->sounds[SOUND_MOVE] = LoadSound("assets/sounds/click.wav");
    assets->sounds[SOUND_SELECT] = LoadSound("assets/sounds/select.wav");
    assets->sounds[SOUND_SHOOT] = LoadSound("assets/sounds/gunshot.mp3");
    assets->sounds[SOUND_DEATH] = LoadSound("assets/sounds/death.mp3");
    assets->sounds[SOUND_SPECIAL_BULLET] = LoadSound("assets/sounds/special_bullet.mp3");
    assets->sounds[SOUND_HEAL] = LoadSound("assets/sounds/heal.mp3");
    assets->sounds[SOUND_POISON] = LoadSound("assets/sounds/poison.mp3");
    assets->sounds[SOUND_SLOWDOWN] = LoadSound("assets/sounds/slowdown.mp3");
    assets->sounds[SOUND_SPEEDUP] = LoadSound("assets/sounds/speedup.mp3");

    // Load fonts
    assets->fonts[FONT_BODY] = LoadFont("assets/fonts/Ubuntu-Bold.ttf");
    assets->fonts[FONT_HEADER] = LoadFont("assets/fonts/Ubuntu-Medium.ttf");
    assets->fonts[FONT_INGAME] = LoadFont("assets/fonts/assets/fonts/Orbitron-Medium.ttf");

    // Load textures
    assets->textures[TEXTURE_BLOCK] = LoadTexture("assets/sprites/block.png");
    assets->textures[TEXTURE_BULLET] = LoadTexture("assets/sprites/bullet_brick.png");
    assets->textures[TEXTURE_SHOOTER_L] = LoadTexture("assets/sprites/left.png");
    assets->textures[TEXTURE_SHOOTER_R] = LoadTexture("assets/sprites/right.png");
    assets->textures[TEXTURE_SHOOTER_M] = LoadTexture("assets/sprites/mid.png");
    assets->textures[TEXTURE_SHOOTER_T] = LoadTexture("assets/sprites/Top.png");
    assets->textures[TEXTURE_HEART] = LoadTexture("assets/sprites/heart.png");
    assets->textures[TEXTURE_LASER_BUTTON] = LoadTexture("assets/sprites/laser_button.png");
    assets->textures[TEXTURE_RANDOM] = LoadTexture("assets/sprites/random.png");
    assets->textures[TEXTURE_SPEEDUP] = LoadTexture("assets/sprites/speed_up.png");
    assets->textures[TEXTURE_SLOWDOWN] = LoadTexture("assets/sprites/slow_down.png");
    assets->textures[TEXTURE_MIN1_HP] = LoadTexture("assets/sprites/minushp.png");
    assets->textures[TEXTURE_PLS1_HP] = LoadTexture("assets/sprites/heal.png");
    assets->textures[TEXTURE_SPECIAL_BULLET] = LoadTexture("assets/sprites/bomb.png");

    assets->bg[BG_PLAY] = LoadTexture("assets/background/play.png");

    return assets;
}

void destroyAssets(Assets* assets) {
    if (!assets) return;

    // Unload sounds
    for (int i = 0; i < SOUND_COUNT; i++) {
        UnloadSound(assets->sounds[i]);
    }

    // Unload fonts
    for (int i = 0; i < FONT_COUNT; i++) {
        UnloadFont(assets->fonts[i]);
    }

    // Unload textures
    for (int i = 0; i < TEXTURE_COUNT; i++) {
        UnloadTexture(assets->textures[i]);
    }

    // Unload bg
    for (int i = 0; i < BG_COUNT; i++) {
        UnloadTexture(assets->bg[i]);
    }

    free(assets);
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
        free(resources);
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
    Image ico = LoadImage("assets/ico.png");
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
    openingAnimation(&opTrans.progress); // Ditambahkan oleh Faliq

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
        }
    }

    destroyAssets(resources->assets);
    CloseWindow();
}

int loadingScreen(float* loadingTime) {
    ScaleFactor scale = GetScreenScaleFactor();
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

    ScaleFactor scale = GetScreenScaleFactor();
    int fontSize = auto_x(30);
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
        int totalTextHeight = 0;
        for (int i = 0; i < lineCount; i++) {
            Vector2 textSize = MeasureTextEx(defaultFont, lines[i], fontSize, 2);
            totalTextHeight += textSize.y + 30;
        }

        int startY = (GetScreenHeight() - totalTextHeight) / 2;
        for (int i = 0; i < lineCount; i++) {
            Vector2 textSize = MeasureTextEx(defaultFont, lines[i], fontSize, 2);
            int startX = (GetScreenWidth() - textSize.x) / 2;
            DrawTextEx(defaultFont, lines[i], (Vector2) { startX, startY }, fontSize, 2, selection == i ? BLUE : DARKGRAY);
            startY += textSize.y + auto_y(50);
        }
        if (IsKeyPressed(KEY_F1)) { // Use any function key for testing
            PlaySound(SOUND(resources, SOUND_SELECT));
            gameOver(resources);
        }
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
            case 0:
                resources->currentState = STATE_SELECT_LEVEL;
                break;
            case 1:
                resources->currentState = STATE_HIGH_SCORES;
                break;
            case 2:
                resources->currentState = STATE_CONTROLS;
                break;
            case 3:
                resources->currentState = STATE_SETTINGS;
                break;
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
    ScaleFactor scale = GetScreenScaleFactor();
    int fontSize = auto_y(20);
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

        int totalHeight = 0;
        for (int i = 0; i < lineCount; i++) {
            totalHeight += fontSize + spacing;
        }

        int startY = (GetScreenHeight() - totalHeight) / 2;
        {
            Vector2 textSize = MeasureTextEx(FONT(resources, FONT_HEADER), "CONTROLS", auto_y(30), 0);
            int startX = (GetScreenWidth() - textSize.x) / 2;
            DrawTextEx(FONT(resources, FONT_HEADER), "CONTROLS", (Vector2) { startX, startY - auto_y(60) }, 30, 0, DARKGRAY);
        }

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
            DrawTextEx(FONT(resources, FONT_BODY), label, (Vector2) { startX, y }, fontSize, spacing, DARKGRAY);
            DrawTextEx(FONT(resources, FONT_BODY), ":", (Vector2) { colonX, y }, fontSize, spacing, DARKGRAY);
            DrawTextEx(FONT(resources, FONT_BODY), value, (Vector2) { textX, y }, fontSize, spacing, DARKGRAY);
            y += fontSize + spacing;
        }
        {
            const char* infoText;
            // Jika game dijeda (resources->prevState == STATE_SELECT_LEVEL) tampilkan tiga shortcut
            if (resources->prevState == STATE_PLAY)
                infoText = "[R]: Resume    [P]: Pause Menu    [F]: ke Settings";
            else
                infoText = "[A]: Main Menu    [F]: Settings";
            Vector2 textSize = MeasureTextEx(FONT(resources, FONT_BODY), infoText, (resources->prevState == STATE_PLAY) ? 15 : 20, 2.0f);
            int startXInfo = (GetScreenWidth() - textSize.x) / 2;
            DrawTextEx(FONT(resources, FONT_BODY), infoText, (Vector2) { startXInfo, 560 }, (resources->prevState == STATE_PLAY) ? 15 : 20, 2.0f, DARKGRAY);
        }
        EndDrawing();

        if (resources->prevState != STATE_PLAY) {
            if (IsKeyPressed(MOVE_LEFT || BACK_KEY)) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                resources->currentState = (resources->prevState == STATE_PLAY) ? STATE_PAUSE : STATE_MAIN_MENU;
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
    ScaleFactor scale = GetScreenScaleFactor();
    int fontSize = auto_y(20);
    int spacing = auto_x(4);
    int menuSpacing = auto_y(25);

    char lines[4][100];
    snprintf(lines[0], sizeof(lines[0]), "Music : %s", resources->settings.music ? "On" : "Off");
    snprintf(lines[1], sizeof(lines[1]), "Sfx   : %s", resources->settings.sfx ? "On" : "Off");
    SetSoundVolume(SOUND(resources, SOUND_MOVE), resources->settings.sfx ? 1.0f : 0.0f);
    SetSoundVolume(SOUND(resources, SOUND_SELECT), resources->settings.sfx ? 1.0f : 0.0f);
    snprintf(lines[2], sizeof(lines[2]), "Controls");
    snprintf(lines[3], sizeof(lines[3]), "Reset");

    int lineCount = len(lines);
    int selection = 0;
    bool editing = false;

    while (resources->currentState == STATE_SETTINGS && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        int totalHeight = (2 * (fontSize + spacing)) +
            (2 * (fontSize + menuSpacing));

        int startY = (GetScreenHeight() - totalHeight) / 2;
        {
            Vector2 textSize = MeasureTextEx(FONT(resources, FONT_HEADER), "SETTINGS", auto_y(30), 0);
            int startX = (GetScreenWidth() - textSize.x) / 2;
            DrawTextEx(FONT(resources, FONT_HEADER), "SETTINGS", (Vector2) { startX, auto_y(60) }, 30, 0, DARKGRAY);
        }

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
            DrawTextEx(FONT(resources, FONT_BODY), label, (Vector2) { startX, y }, fontSize, spacing, selection == i ? editing ? DARKBLUE : BLUE : DARKGRAY);
            DrawTextEx(FONT(resources, FONT_BODY), ":", (Vector2) { colonX, y }, fontSize, spacing, DARKGRAY);
            DrawTextEx(FONT(resources, FONT_BODY), value, (Vector2) { textX, y }, fontSize, spacing, DARKGRAY);
            y += fontSize + spacing;
        }
        y += 50;
        for (int i = 2; i < 4; ++i) {
            const char* optionText = lines[i];
            Vector2 textSize = MeasureTextEx(FONT(resources, FONT_BODY), optionText, 20, 2.0f);
            int startXOption = (GetScreenWidth() - textSize.x) / 2;
            DrawTextEx(FONT(resources, FONT_BODY), optionText, (Vector2) { startXOption, y }, 20, 2.0f, selection == i ? BLUE : DARKGRAY);
            y += 50;
        }
        {
            const char* infoText;
            if (resources->prevState == STATE_PLAY)
                infoText = "[R]: Resume    [P]: Pause Menu    [F]: ke Controls";
            else
                infoText = "[A]: Main Menu    [F]: Controls";
            Vector2 textSize = MeasureTextEx(FONT(resources, FONT_BODY), infoText, (resources->prevState == STATE_PLAY) ? 15 : 20, 2.0f);
            int startXInfo = (GetScreenWidth() - textSize.x) / 2;
            DrawTextEx(FONT(resources, FONT_BODY), infoText, (Vector2) { startXInfo, 560 }, (resources->prevState == STATE_PLAY) ? 15 : 20, 2.0f, DARKGRAY);
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
                        SetMusicVolume(soundGameplay, 0.0f);  // Add this
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
                        SetMusicVolume(soundGameplay, 0.5f);  // Add this
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
                resources->currentState = (resources->prevState == STATE_PLAY) ? STATE_PAUSE : STATE_MAIN_MENU;
            }
        }
        if (FORWARD_KEY) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            resources->currentState = STATE_CONTROLS;
        }
    }
}

void showHiScore(GameResources* resources) {
    ScaleFactor scale = GetScreenScaleFactor();
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
        int totalHeight = 0;
        for (int i = 0; i < lineCount; i++) {
            totalHeight += fontSize + spacing;
        }
        int startY = (GetScreenHeight() - totalHeight) / 2;
        {
            Vector2 textSize = MeasureTextEx(FONT(resources, FONT_HEADER), "HIGH SCORE", auto_y(30), 0);
            int startX = (GetScreenWidth() - textSize.x) / 2;
            DrawTextEx(FONT(resources, FONT_HEADER), "HIGH SCORE", (Vector2) { startX, auto_y(60) }, 30, 0, DARKGRAY);
        }

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

            DrawTextEx(FONT(resources, FONT_BODY), label, (Vector2) { startX, y }, fontSize, spacing, DARKGRAY);
            DrawTextEx(FONT(resources, FONT_BODY), ":", (Vector2) { colonX, y }, fontSize, spacing, DARKGRAY);
            DrawTextEx(FONT(resources, FONT_BODY), value, (Vector2) { textX, y }, fontSize, spacing, DARKGRAY);

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
    ScaleFactor scale = GetScreenScaleFactor();
    const char* message = "Are you sure you want to exit?";
    const char* options[] = { "Yes", "No" };
    int selection = 1;
    int fontSize = auto_y(20);
    int lineCount = len(options);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        Vector2 textSize = MeasureTextEx(FONT(resources, FONT_HEADER), message, fontSize, 2);
        int startX = (GetScreenWidth() - textSize.x) / 2;
        int startY = (640 - textSize.y) / 2 - 50;
        DrawTextEx(FONT(resources, FONT_HEADER), message, (Vector2) { startX, startY }, fontSize, 2, DARKGRAY);

        startY += textSize.y + auto_y(50);
        for (int i = 0; i < lineCount; i++) {
            textSize = MeasureTextEx(FONT(resources, FONT_HEADER), options[i], fontSize, 2);
            startX = (GetScreenWidth() - textSize.x) / 2;
            DrawTextEx(FONT(resources, FONT_BODY), options[i], (Vector2) { startX, startY }, fontSize, 2, selection == i ? BLUE : DARKGRAY);
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
        exit(0); // Exit program
    }
    else {
        resources->currentState = resources->prevState;
    }
}

bool confirmBack(GameResources* resources) {
    ScaleFactor scale = GetScreenScaleFactor();
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
    ScaleFactor scale = GetScreenScaleFactor();
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
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.8f));
        {
            const char* pausedText = "PAUSED";
            Vector2 textSize = MeasureTextEx(FONT(resources, FONT_HEADER), pausedText, 40, 2);
            int startX = (GetScreenWidth() - textSize.x) / 2;
            DrawTextEx(FONT(resources, FONT_HEADER), pausedText, (Vector2) { startX, 140 }, 40, 2, DARKGRAY);
        }

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
            DrawTextEx(FONT(resources, FONT_BODY), lines[i], (Vector2) { startX, startY }, fontSize, 2, selection == i ? BLUE : DARKGRAY);
            startY += textSize.y + (25 * scale.y);
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
        ScaleFactor scale = GetScreenScaleFactor();
        float counter = 3.0f;
        while (counter > 0.0f && !WindowShouldClose()) {
            counter -= GetFrameTime();
            BeginDrawing();
            ClearBackground(RAYWHITE);
            char text[8];
            sprintf(text, "%d", (int)ceilf(counter));
            Vector2 position = {
                230 * scale.x,
                300 * scale.y
            };
            DrawTextEx(GetFontDefault(), text, position, 50 * scale.y, 2 * scale.x, RED);
            EndDrawing();
        }
    }
}

void selectMode(GameResources* resources) {
    ScaleFactor scale = GetScreenScaleFactor();
    resources->prevState = STATE_SELECT_LEVEL;
    const char* title = "SELECT MODE";
    const char* modes[] = {
        "Super EZ",
        "Easy",
        "Beginner",
        "Medium",
        "Hard",
        "Super Hard",
        "Expert",
        "Master",
        "Legend",
        "God",
        "Progressive"
    };
    int selection = resources->settings.mode;
    int lineCount = len(modes);
    bool selecting = true;

    while (selecting && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        {
            Vector2 textSize = MeasureTextEx(FONT(resources, FONT_HEADER), title, 30, 2);
            int startX = (GetScreenWidth() - textSize.x) / 2;
            DrawTextEx(FONT(resources, FONT_HEADER), title, (Vector2) { startX, 100 }, 30, 2, DARKGRAY);
        }
        {
            Vector2 arrowSize = MeasureTextEx(FONT(resources, FONT_HEADER), ">", 40, 2);
            int startY = (GetScreenHeight() - arrowSize.y) / 2;

            int windowCenterX = GetScreenWidth() / 2;
            int arrowOffset = 200 * scale.x;  // Fixed distance from center

            // Draw arrows at fixed positions from window edges
            DrawTextEx(FONT(resources, FONT_HEADER), "<", (Vector2) { windowCenterX - arrowOffset, startY }, 40, 2, DARKGRAY);
            DrawTextEx(FONT(resources, FONT_HEADER), ">", (Vector2) { windowCenterX + arrowOffset - arrowSize.x, startY }, 40, 2, DARKGRAY);
        }
        {
            Vector2 textSize = MeasureTextEx(FONT(resources, FONT_HEADER), modes[selection], 60, 2);
            int startX = (GetScreenWidth() - textSize.x) / 2;
            int startY = (GetScreenHeight() - textSize.y) / 2;
            DrawTextEx(FONT(resources, FONT_HEADER), modes[selection], (Vector2) { startX, startY }, 60, 2, BLUE);
        }
        {
            const char* controlsText = "[↑/↓]: Select    [Enter]: Play    [B]: Back";
            Vector2 textSize = MeasureTextEx(FONT(resources, FONT_BODY), controlsText, 15, 2);
            int startX = (GetScreenWidth() - textSize.x) / 2;
            DrawTextEx(FONT(resources, FONT_BODY), controlsText, (Vector2) { startX, 560 }, 15, 2, DARKGRAY);
        }

        EndDrawing();

        if (MOVE_LEFT || MOVE_DOWN) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            selection--;
            if (selection < 0) selection = lineCount - 1;
        }
        if (MOVE_RIGHT || MOVE_UP) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            selection++;
            if (selection >= lineCount) selection = 0;
        }
        if (OK_KEY) {
            PlaySound(SOUND(resources, SOUND_SELECT));
            resources->currentState = STATE_PLAY;
            selecting = false;
            resources->settings.mode = selection;
            saveSettings(resources->settings);
            resources->gameLevel = selection;
            countdownPause(resources);
        }
        if (BACK_KEY) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            resources->currentState = STATE_MAIN_MENU;
            selecting = false;
        }
    }
}

bool confirmReset(GameResources* resources) {
    ScaleFactor scale = GetScreenScaleFactor();
    const char* message = "Reset all high scores?";
    const char* options[] = { "Yes", "No" };
    int selection = 1;
    int fontSize = auto_y(20);
    int lineCount = len(options);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
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
    ScaleFactor scale = GetScreenScaleFactor();
    const char* message = "Cannot reset scores during gameplay!";
    const char* subMessage = "Press any key to go back";

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

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

void gameOver(GameResources* resources) {
    ScaleFactor scale = GetScreenScaleFactor();
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
    long long int currentHighScore = scores[resources->gameLevel].score;

    while (inGameOver && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        Vector2 textSize = MeasureTextEx(FONT(resources, FONT_HEADER), message, 30, 2);
        int startX = (GetScreenWidth() - textSize.x) / 2;
        int startY = (GetScreenHeight() - textSize.y) / 2 - 50;
        DrawTextEx(FONT(resources, FONT_HEADER), message, (Vector2) { startX, startY }, 30, 2, DARKGRAY);

        // Tampilkan high score juga
        char highScoreText[50];
        sprintf(highScoreText, "High Score: %lld", currentHighScore);
        Vector2 highScoreSize = MeasureTextEx(FONT(resources, FONT_BODY), highScoreText, fontSize, 2);
        DrawTextEx(FONT(resources, FONT_BODY), highScoreText,
            (Vector2) {
            (GetScreenWidth() - highScoreSize.x) / 2, startY + 40
        },
            fontSize, 2, DARKGRAY);

        startY += textSize.y + auto_y(50);
        for (int i = 0; i < lineCount; i++) {
            textSize = MeasureTextEx(FONT(resources, FONT_HEADER), options[i], fontSize, 2);
            startX = (GetScreenWidth() - textSize.x) / 2;
            DrawTextEx(FONT(resources, FONT_BODY), options[i], (Vector2) { startX, startY }, fontSize, 2,
                selection == i ? BLUE : DARKGRAY);
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
    ScaleFactor scale = GetScreenScaleFactor();
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
            gameOver(resources);
            free(gameContext);
            gameContext = NULL;
            return;
        }
    }


    BeginDrawing();
    ClearBackground(DARKGRAY);

    DrawRectangle(auto_x(0), auto_y(0), auto_x(320), auto_y(640), (Color) { 236, 244, 255, 255 });
    DrawRectangle(auto_x(320), auto_y(0), auto_x(3), auto_y(640), (Color) { 27, 45, 4, 255 });
    DrawRectangle(auto_x(323), auto_y(0), auto_x(7), auto_y(640), (Color) { 65, 71, 71, 255 });
    DrawRectangle(auto_x(330), auto_y(0), auto_x(150), auto_y(640), (Color) { 25, 38, 47, 255 });
    DrawRectangle(auto_x(0), auto_y(512), auto_x(320), auto_y(1), BLACK);


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
        gameOver(resources);
        free(gameContext);
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
    Vector2 playerPos = { P.x, P.y };
    shooter(&P.x, &P.y, resources, scale);
    moveSet(&P.x, resources, scale);

    MoveBullets(gameContext->bullets);
    DrawBullets(gameContext->bullets, resources);

    // Handle input
    if (MOVE_RIGHT) {
        playerPos.x += 5;
        gameContext->playerDirection = 1;
    }
    if (MOVE_LEFT) {
        playerPos.x -= 5;
        gameContext->playerDirection = -1;
    }
    if (SHOOT_KEY) {
        ShootBullets(gameContext->bullets, playerPos, &gameContext->bulletCount,
            &gameContext->canShoot, 0, resources);
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

// Update fungsi isGameOverCheck untuk menerima Game
bool isGameOverCheck(Game* game) {
    for (int j = 0; j < MAX_COLUMNS; j++) {
        if (game->grid[MAX_ROWS - 1][j]) {
            return true;
        }
    }
    return false;
}

// Update fungsi isRowFull
bool isRowFull(Game* game, int row) {
    for (int j = 0; j < MAX_COLUMNS; j++) {
        if (!game->grid[row][j]) return false;
    }
    return true;
}

// Update fungsi hasActiveBlockBelow
bool hasActiveBlockBelow(Game* game, int row) {
    for (int i = row + 1; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLUMNS; j++) {
            if (game->grid[i][j]) return true;
        }
    }
    return false;
}

// Update fungsi shiftRowsUp
void shiftRowsUp(Game* game, int startRow) {
    for (int i = startRow; i < MAX_ROWS - 1; i++) {
        for (int j = 0; j < MAX_COLUMNS; j++) {
            game->grid[i][j] = game->grid[i + 1][j];
        }
    }
    for (int j = 0; j < MAX_COLUMNS; j++) {
        game->grid[MAX_ROWS - 1][j] = false;
    }
}

// Update fungsi handleFullRow
void handleFullRow(Game* game, int row) {
    if (isRowFull(game, row)) {
        if (hasActiveBlockBelow(game, row)) {
            shiftRowsUp(game, row);
            addScore(game, row);
        }
        else {
            for (int j = 0; j < MAX_COLUMNS; j++) {
                game->grid[row][j] = false;
            }
            addScore(game, row);
        }
    }
}

// Handle block movement and generation
void handleBlockMovement(Game* game, int minBlocks, int maxBlocks) {
    // Analyze current column states
    int emptyColLength[MAX_COLUMNS] = { 0 };
    int totalEmptyColumns = 0;

    // Count empty spaces in each column from top
    for (int j = 0; j < MAX_COLUMNS; j++) {
        int emptyCount = 0;
        for (int i = 0; i < MAX_ROWS; i++) {
            if (!game->grid[i][j]) {
                emptyCount++;
            }
            else {
                break;
            }
        }
        emptyColLength[j] = emptyCount;
        if (emptyCount >= 4) {
            totalEmptyColumns++;
        }
    }

    // Move existing blocks down
    moveBlocksDown(game);

    // Generate new blocks
    generateNewBlocks(game, minBlocks, maxBlocks, emptyColLength, totalEmptyColumns);
}

// Move all blocks down one row
void moveBlocksDown(Game* game) {
    for (int i = MAX_ROWS - 1; i > 0; i--) {
        for (int j = 0; j < MAX_COLUMNS; j++) {
            game->grid[i][j] = game->grid[i - 1][j];
        }
    }

    // Clear top row
    for (int j = 0; j < MAX_COLUMNS; j++) {
        game->grid[0][j] = false;
    }
}

// Generate new blocks in the top row
void generateNewBlocks(Game* game, int minBlocks, int maxBlocks, int* emptyColLength, int totalEmptyColumns) {
    int numBlocks = minBlocks + (rand() % (maxBlocks - minBlocks + 1));
    int remainingBlocks = numBlocks;

    // First pass: Fill critical gaps
    if (totalEmptyColumns > 0) {
        remainingBlocks = fillCriticalGaps(game, remainingBlocks, emptyColLength);
    }

    // Second pass: Random placement with balance
    fillRemainingBlocks(game, remainingBlocks);
}

// Fill gaps that are too large
int fillCriticalGaps(Game* game, int remainingBlocks, int* emptyColLength) {
    for (int j = 0; j < MAX_COLUMNS && remainingBlocks > 0; j++) {
        if (emptyColLength[j] >= 4) {
            game->grid[0][j] = true;
            remainingBlocks--;
        }
    }
    return remainingBlocks;
}

// Fill remaining blocks with some randomness but maintaining balance
void fillRemainingBlocks(Game* game, int remainingBlocks) {
    int maxAttemptsPerBlock = 3;
    while (remainingBlocks > 0) {
        for (int attempt = 0; attempt < maxAttemptsPerBlock && remainingBlocks > 0; attempt++) {
            int pos = rand() % MAX_COLUMNS;
            if (!game->grid[0][pos]) {
                game->grid[0][pos] = true;
                remainingBlocks--;
                break;
            }
        }
    }
}

// Handle bullet collisions with blocks
void handleBulletCollisions(Game* game) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (game->bullets[i].active) {
            int gridX = game->bullets[i].position.x / 32;
            int gridY = game->bullets[i].position.y / 32;

            if (isValidGridPosition(gridX, gridY) && game->grid[gridY][gridX]) {
                processBulletHit(game, gridX, gridY, i);
            }
        }
    }
}

// Check if grid position is valid
bool isValidGridPosition(int x, int y) {
    return x >= 0 && x < MAX_COLUMNS && y >= 0 && y < MAX_ROWS;
}

// Process what happens when a bullet hits a block
void processBulletHit(Game* game, int gridX, int gridY, int bulletIndex) {
    // Check if special bullet powerup is active
    bool hasSpecialBullet = false;
    for (int i = 0; i < game->activeEffectsCount; i++) {
        if (game->activePowerups[i].active &&
            game->activePowerups[i].type == POWERUP_SPECIAL_BULLET) {
            hasSpecialBullet = true;
            break;
        }
    }

    if (hasSpecialBullet) {
        // Destroy entire row
        for (int col = 0; col < MAX_COLUMNS; col++) {
            game->grid[gridY][col] = false;
        }
    }
    else {
        // Normal single block behavior
        if (gridY < MAX_ROWS - 1) {
            game->grid[gridY + 1][gridX] = true;
        }
    }

    // Common actions
    for (int row = 0; row < MAX_ROWS; row++) {
        handleFullRow(game, row);
    }
    game->bullets[bulletIndex].active = false;
    game->score += 10;
}

// Updated main updateBlocks function
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

// Update fungsi initBlocks
void initBlocks(Game* game, GameResources* resources) {
    // Clear all blocks
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLUMNS; j++) {
            game->grid[i][j] = false;
        }
    }

    // Get correct initial block range
    int minBlocks, maxBlocks;
    getBlockRangeForMode(resources->gameLevel, &minBlocks, &maxBlocks);

    // Generate blocks within the defined range
    int numBlocks = minBlocks + (rand() % (maxBlocks - minBlocks + 1));

    // Place blocks randomly
    while (numBlocks > 0) {
        int pos = rand() % MAX_COLUMNS;
        if (!game->grid[0][pos]) {
            game->grid[0][pos] = true;
            numBlocks--;
        }
    }

    game->frameCounter = 0;
    game->score = 0;
    game->gameOver = false;
}

// Update fungsi drawBlocks
void drawBlocks(Game* game, GameResources* resources) {
    ScaleFactor scale = GetScreenScaleFactor();
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLUMNS; j++) {
            if (game->grid[i][j]) {
                if (TEXTURE(resources, TEXTURE_BLOCK).id != 0) {
                    DrawTexture(TEXTURE(resources, TEXTURE_BLOCK), j * auto_x(32), i * auto_y(32), WHITE);
                }
                else {
                    DrawRectangle(j * auto_x(32), i * auto_y(32), auto_x(32), auto_y(32), BLUE);
                }
            }
        }
    }
}

void printGrid(Game* game) {
    system("cls");
    printf("\n--- Grid State ---\n");
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLUMNS; j++) {
            printf(game->grid[i][j] ? "[x]" : "   ");
        }
        printf("\n");
    }
    printf("-----------------\n");
}

void drawGameUI(Game* game, GameResources* resources) {
    ScaleFactor scale = GetScreenScaleFactor();

    {
        char obj[32];
        Vector2 textSize;
        int startX, areaWidth;
        const int mid = auto_x(405);
        const int linespacing = auto_y(30);

        // Mode ==========================================================
        int curY = 50;
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
        // High-Score ====================================================
        {
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

                        // Calculate position for current icon
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

        for (int i = 0; i < game->lives; i++) {
            float scale = 30.0f / 640.0f;
            DrawTextureEx(TEXTURE(resources, TEXTURE_HEART),
                (Vector2) {
                345 + (i * 35), 480
            }, // posisi
                0,  // rotation
                scale, // scale factor
                WHITE);
        }

        float local_scale = 80.0f / 640.0f;
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

    }
}