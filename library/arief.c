/**
 * ARIEF MODULE - ASSET & RESOURCE MANAGEMENT
 * ==========================================
 *
 * Modul yang menangani asset dan resource management, UI rendering, menu system, dan game screens.
 * Berisi implementasi untuk pengelolaan aset, antarmuka pengguna, navigasi menu, dan tampilan layar.
 *
 * Komponen utama:
 * - Asset loading dan resource management
 * - UI rendering dan text layout utilities
 * - Menu system dan navigation logic
 * - Screen transitions dan visual effects
 * - Game screens dan dialog system
 * - Settings management dan configuration
 * - Block management dan game mechanics
 * - Score system dan achievement tracking
 */

#include "defines.h"
#include "all.h"
#define PRIMARY_COLOR (Color){0, 65, 57, 255}

 // =============================================================================
 // ASSET AND RESOURCE MANAGEMENT
 // Fungsi-fungsi untuk mengelola aset game, memuat dan membebaskan resource
 // =============================================================================

 // Menyesuaikan ukuran window berdasarkan resolusi layar
 // Menghitung ukuran optimal untuk mencegah window terlalu besar/kecil
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


// Memuat aset berdasarkan tipe dan path yang diberikan
// Mengembalikan pointer ke InputAsset yang berisi data aset
InputAsset* inputAssets(TypeofAssets type, uint id, const char* path) {
    InputAsset* metadata = malloc(sizeof(InputAsset));
    if (!metadata) {
        printf("[LOG] Failed to allocate memory for InputAsset.\n");
        return NULL;
    }

    metadata->id = id;

    switch (type) {
    case TYPE_SOUND: {
        metadata->data = malloc(sizeof(Sound));
        if (!metadata->data) {
            printf("[LOG] Failed to allocate memory for Sound.\n");
            delete(metadata);
            return NULL;
        }
        *(Sound*)metadata->data = LoadSound(path);
        break;
    }
    case TYPE_FONT: {
        metadata->data = malloc(sizeof(Font));
        if (!metadata->data) {
            printf("[LOG] Failed to allocate memory for Font.\n");
            delete(metadata);
            return NULL;
        }
        *(Font*)metadata->data = LoadFont(path);
        break;
    }
    case TYPE_TEXTURE: {
        metadata->data = malloc(sizeof(Texture2D));
        if (!metadata->data) {
            printf("[LOG] Failed to allocate memory for Texture2D.\n");
            delete(metadata);
            return NULL;
        }
        *(Texture2D*)metadata->data = LoadTexture(path);
        break;
    }
    default: {
        printf("[LOG] Unknown asset type.\n");
        delete(metadata);
        return NULL;
    }
    }

    return metadata;
}


// Membuat struktur Assets kosong untuk menyimpan semua aset game
// Menginisialisasi linked list untuk berbagai tipe aset
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
    SLL_insertFront(&assets->textures, inputAssets(TYPE_TEXTURE, TEXTURE_HEART, "assets/sprites/heart.png"));
    SLL_insertFront(&assets->textures, inputAssets(TYPE_TEXTURE, TEXTURE_LASER_BUTTON, "assets/sprites/laser_button.png"));
    SLL_insertFront(&assets->textures, inputAssets(TYPE_TEXTURE, TEXTURE_RANDOM, "assets/sprites/random.png"));
    SLL_insertFront(&assets->textures, inputAssets(TYPE_TEXTURE, TEXTURE_SPEEDUP, "assets/sprites/speed_up.png"));
    SLL_insertFront(&assets->textures, inputAssets(TYPE_TEXTURE, TEXTURE_SLOWDOWN, "assets/sprites/slow_down.png"));
    SLL_insertFront(&assets->textures, inputAssets(TYPE_TEXTURE, TEXTURE_MIN1_HP, "assets/sprites/minushp.png"));
    SLL_insertFront(&assets->textures, inputAssets(TYPE_TEXTURE, TEXTURE_PLS1_HP, "assets/sprites/heal.png"));
    SLL_insertFront(&assets->textures, inputAssets(TYPE_TEXTURE, TEXTURE_SPECIAL_BULLET, "assets/sprites/bomb.png"));
    SLL_insertFront(&assets->textures, inputAssets(TYPE_TEXTURE, TEXTURE_WHITE_ICON, "assets/icon/icon.png"));

    // skin
    SLL_insertFront(&assets->textures, inputAssets(TYPE_TEXTURE, TEXTURE_SKIN_1, "assets/sprites/skin_1.png"));
    SLL_insertFront(&assets->textures, inputAssets(TYPE_TEXTURE, TEXTURE_SKIN_2, "assets/sprites/skin_2.png"));

    // bg
    SLL_insertFront(&assets->bg, inputAssets(TYPE_TEXTURE, BG_PLAY, "assets/bg/BG_Play.png"));
    SLL_insertFront(&assets->bg, inputAssets(TYPE_TEXTURE, BG_MAIN_MENU, "assets/bg/MainMenu.png"));
    SLL_insertFront(&assets->bg, inputAssets(TYPE_TEXTURE, BG_SETTINGS, "assets/bg/BG_Settings.png"));
    SLL_insertFront(&assets->bg, inputAssets(TYPE_TEXTURE, BG_HIGHSCORES, "assets/bg/BG_HighScores.png"));
    SLL_insertFront(&assets->bg, inputAssets(TYPE_TEXTURE, BG_PAUSED, "assets/bg/BG_Paused.png"));
    SLL_insertFront(&assets->bg, inputAssets(TYPE_TEXTURE, BG_CONTROLS, "assets/bg/BG_Controls.png"));
    SLL_insertFront(&assets->bg, inputAssets(TYPE_TEXTURE, BG_CONFIRM, "assets/bg/BG_Confirm.png"));
    SLL_insertFront(&assets->bg, inputAssets(TYPE_TEXTURE, BG_PLAIN, "assets/bg/BG_Plain.png"));
    SLL_insertFront(&assets->bg, inputAssets(TYPE_TEXTURE, CREDIT_SCENE, "assets/bg/CreditScene.png"));
    SLL_insertFront(&assets->bg, inputAssets(TYPE_TEXTURE, BG_GAMEAREA, "assets/bg/GameArea.png"));
    SLL_insertFront(&assets->bg, inputAssets(TYPE_TEXTURE, BG_UIGAME, "assets/bg/UIArea.png"));
    SLL_insertFront(&assets->bg, inputAssets(TYPE_TEXTURE, BG_LOADING, "assets/bg/BG_Loading.png"));
    SLL_insertFront(&assets->bg, inputAssets(TYPE_TEXTURE, BG_HOWTOPLAY, "assets/bg/BG_HowToPlay.png"));
    SLL_insertFront(&assets->bg, inputAssets(TYPE_TEXTURE, ICON_LOADING, "assets/icon/ICON_Loading.png"));


    assets->Shooter = *shooterAssets();
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
    system("cls");
    printf("\e[4;32m""[ALL ASSETS LOADED SUCCESSFULLY]\n""\e[0m");
    printf("Let the fun begin!\n\n");

    return assets;
}


// Mencari dan mengembalikan aset berdasarkan ID dari linked list
// Melakukan traversal pada linked list untuk menemukan aset yang sesuai
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


// Membebaskan memori aset berdasarkan tipe dan menghapus dari linked list
// Memanggil fungsi unload yang sesuai dengan tipe aset
void unloadAndFree(SLLNode* head, TypeofAssets type) {
    SLLNode* temp = head;
    while (temp) {
        InputAsset* asset = (InputAsset*)temp->data;
        if (asset && asset->data) {
            switch (type) {
            case TYPE_SOUND: {
                UnloadSound(*(Sound*)asset->data);
                break;
            }
            case TYPE_FONT: {
                UnloadFont(*(Font*)asset->data);
                break;
            }
            case TYPE_TEXTURE: {
                UnloadTexture(*(Texture2D*)asset->data);
                break;
            }
            default: {
                printf("[LOG] Unknown asset type.\n");
                break;
            }
            }
        }
        temp = SLL_getNextNode(temp);
    }
}


// Menghancurkan seluruh struktur Assets dan membebaskan semua memori
// Memanggil unloadAndFree untuk semua tipe aset lalu dealokasi struktur utama
void destroyAssets(Assets* assets) {
    if (!assets) return;

    // Unload sounds
    unloadAndFree(assets->sounds.head, TYPE_SOUND);

    // Unload fonts
    unloadAndFree(assets->fonts.head, TYPE_FONT);

    // Unload textures
    unloadAndFree(assets->textures.head, TYPE_TEXTURE);

    // Unload backgrounds
    unloadAndFree(assets->bg.head, TYPE_TEXTURE);

    // Unload background modes
    unloadAndFree(assets->bgMode.head, TYPE_TEXTURE);
    unloadAndFree(assets->txMode.head, TYPE_TEXTURE);

    delete(assets);
}

// =============================================================================
// SETTINGS AND CONFIGURATION
// Fungsi-fungsi untuk mengelola pengaturan game dan konfigurasi
// =============================================================================

// Memuat pengaturan game dari file konfigurasi
// Membaca file settings dan mengisi struktur Settings dengan nilai default jika file tidak ada
void loadSettings(Settings* settings) {
    FILE* file = fopen("db/settings.dat", "r");
    if (!file) {
        printf("[LOG] File tidak ditemukan! Menggunakan default settings.\n");
        settings->music = 1;
        settings->sfx = 1;
        settings->mode = 1;
        settings->skin = 0;
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
        else if (strcmp(key, "skin") == 0) {
            settings->skin = value;
        }
    }
    fclose(file);
}

// Menyimpan pengaturan game ke file konfigurasi
// Menulis struktur Settings ke file untuk persistensi pengaturan
void saveSettings(Settings settings) {
    FILE* file = fopen("db/settings.dat", "w");
    if (!file) {
        printf("[LOG] Gagal menyimpan settings!\n");
        return;
    }

    fprintf(file, "music,%d\n", settings.music);
    fprintf(file, "sfx,%d\n", settings.sfx);
    fprintf(file, "mode,%d\n", settings.mode);
    fprintf(file, "skin,%d\n", settings.skin);

    fclose(file);
}


// =============================================================================
// GAME CONTEXT AND INITIALIZATION
// Fungsi-fungsi untuk inisialisasi dan pengelolaan konteks game utama
// =============================================================================

// Membuat dan menginisialisasi konteks game utama
// Mengalokasi memori untuk struktur Game dan menginisialisasi semua komponennya
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
    game->activePowerups = *createQueue();
    game->bullets = (SingleLinkedList*)malloc(sizeof(SingleLinkedList));
    game->bullets->head = NULL;
    game->bullets->tail = NULL;
    game->bullets->size = 0;

    game->grid = initGameGrid();

    return game;
}

// Menghancurkan konteks game dan membebaskan semua memori
// Dealokasi grid, bullets, dan semua struktur data yang digunakan
void destroyGameContext(Game* game) {
    if (!game) return;
    delete(game);
}


// Menginisialisasi grid permainan sebagai doubly linked list
// Membuat grid kosong dengan ukuran yang ditentukan untuk permainan
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


// Fungsi utama untuk menjalankan window game
// Menginisialisasi Raylib, membuat window, dan menjalankan game loop utama
void mainWindow(void) {
    GameResources* resources = malloc(sizeof(GameResources));
    if (!resources) {
        printf("[LOG] Failed to allocate resources!\n");
        return;
    }

    loadSettings(&resources->settings);
    resources->scores = *loadHiScores();

    float loadingTime = 0.0f;

    int screenWidth = MIN_SCREEN_WIDTH;
    int screenHeight = (screenWidth * ASPECT_RATIO_HEIGHT) / ASPECT_RATIO_WIDTH;

    openingTransition opTrans; // Ditambahkan oleh faliq
    opTrans.progress = 0.0f; // Ditambahkan oleh faliq

    InitWindow(screenWidth, screenHeight, "Block Shooter");
    Image ico = LoadImage("assets/icon/icon.png");
    SetWindowIcon(ico);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(MIN_SCREEN_WIDTH, MIN_SCREEN_HEIGHT);
    InitAudioDevice();

    initializePlayerPosition();

    musicGameplay(resources); // Ditambahkan faliq
    SetTargetFPS(60);
    resources->assets = createAssets();
    if (!resources->assets) {
        delete(resources);
        printf("[LOG] Failed to create assets!\n");
        return;
    }
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

            // Update posisi player setelah resize
            updatePlayerPositionOnResize();
        }
        switch (resources->currentState) {
        case STATE_LOADING:
            if (loadingScreen(resources, &loadingTime)) {
                resources->currentState = STATE_MAIN_MENU;
            }
            break;
        case STATE_MAIN_MENU: mainMenu(resources); break;
        case STATE_HIGH_SCORES: showHiScore(resources); break;
        case STATE_CONTROLS: showControls(resources); break;
        case STATE_SETTINGS: showSettings(resources); break;
        case STATE_PLAY: UpdateMusicStream(soundGameplay); displayGame(resources); break;
        case STATE_QUIT: exitGame(resources); break;
        case STATE_PAUSE: pauseMenu(resources); break;
        case STATE_SELECT_LEVEL: selectMode(resources); break;
        case STATE_SCENE: showCredits(resources); break;
        case STATE_HOWTOPLAY: showHowToPlay(resources); break;
        }
    }

    destroyAssets(resources->assets);
    CloseWindow();
}

// =============================================================================
// SCREEN AND DISPLAY UTILITIES
// Fungsi-fungsi utilitas untuk tampilan layar dan scaling
// =============================================================================

// Menghitung faktor skala berdasarkan resolusi layar
// Mengembalikan ScaleFactor untuk menyesuaikan UI dengan berbagai resolusi
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

// =============================================================================
// TEXT AND UI RENDERING
// Fungsi-fungsi untuk rendering teks dan elemen antarmuka pengguna
// =============================================================================

// Memformat string dengan variadic arguments seperti printf
// Menggunakan vsnprintf untuk format string dengan parameter dinamis
void formatGameText(char* buffer, const char* format, ...) {
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
}

// Menggambar teks terpusat horizontal pada posisi Y tertentu
// Menghitung posisi X agar teks berada di tengah layar
void drawCenteredText(Font font, const char* text, float y, int fontSize, float spacing, Color color) {
    Vector2 textSize = MeasureTextEx(font, text, auto_y(fontSize), auto_y(spacing));
    int startX = (GetScreenWidth() - textSize.x) / 2;
    DrawTextEx(font, text, (Vector2) { startX, y }, auto_y(fontSize), auto_y(spacing), color);
}

// Menggambar opsi menu dengan highlight jika dipilih
// Mengubah warna dan efek visual berdasarkan status selected
void drawMenuOption(Font font, const char* text, float y, int fontSize, bool selected, Color color) {
    drawCenteredText(font, text, y, fontSize, 2.0f, color);
}

// Menggambar opsi toggle dengan indikator editing dan selection
// Menampilkan opsi yang bisa di-toggle dengan visual feedback
void drawToggleOption(Font font, const char* line, int startX, int y, int maxLabelWidth,
    int fontSize, float spacing, bool isSelected, bool isEditing, Color normalColor) {
    char label[50], value[50];
    sscanf(line, "%[^:]:%[^\n]", label, value);

    int colonX = startX + maxLabelWidth + auto_x(10);
    int textX = colonX + MeasureTextEx(font, ":", fontSize, spacing).x + 5;

    // Tentukan warna berdasarkan selection dan editing state
    Color labelColor = normalColor;
    if (isSelected) {
        labelColor = isEditing ? PRIMARY_COLOR : ORANGE;
    }

    DrawTextEx(font, label, (Vector2) { startX, y }, fontSize, spacing, labelColor);
    DrawTextEx(font, ":", (Vector2) { colonX, y }, fontSize, spacing, normalColor);
    DrawTextEx(font, value, (Vector2) { textX, y }, fontSize, spacing, normalColor);
}


// Menggambar array label dan nilai dalam format kolom
// Menampilkan pasangan label-value dengan alignment yang rapi
void drawLabelsAndValues(const char* lines[], int lineCount, int startX, int startY, int maxLabelWidth, int fontSize, float spacing, Font font, Color color) {
    int y = startY;
    for (int i = 0; i < lineCount; i++) {
        char label[50], value[50];
        sscanf(lines[i], "%[^:]:%[^\n]", label, value);
        int colonX = startX + maxLabelWidth + auto_x(10);
        int textX = colonX + MeasureTextEx(font, ":", fontSize, spacing).x + 5;
        DrawTextEx(font, label, (Vector2) { startX, y }, fontSize, spacing, color);
        DrawTextEx(font, ":", (Vector2) { colonX, y }, fontSize, spacing, color);
        DrawTextEx(font, value, (Vector2) { textX, y }, fontSize, spacing, color);
        y += fontSize + spacing;
    }
}


// Menggambar teks terpusat dalam area UI tertentu
// Memposisikan teks di tengah area UI yang ditentukan oleh parameter
void drawCenteredUIText(const char* text, float y, int fontSize, float uiAreaX, float uiAreaWidth, Color color) {
    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, 2);
    float xPos = uiAreaX + (uiAreaWidth - textSize.x) / 2;
    DrawTextEx(GetFontDefault(), text, (Vector2) { xPos, y }, fontSize, 2, color);
}


// =============================================================================
// LAYOUT AND MEASUREMENT UTILITIES
// Fungsi-fungsi untuk mengukur dan menghitung tata letak UI
// =============================================================================

// Menghitung total tinggi yang dibutuhkan untuk array teks
// Menjumlahkan tinggi semua baris teks dengan spacing yang diberikan
float calculateTotalTextHeight(Font font, const char* lines[], int lineCount, int fontSize, float spacing) {
    float totalHeight = 0;
    for (int i = 0; i < lineCount; i++) {
        Vector2 textSize = MeasureTextEx(font, lines[i], fontSize, spacing);
        totalHeight += textSize.y + spacing;
    }
    return totalHeight - spacing; // Kurangi spacing terakhir
}


// Menghitung lebar maksimum untuk label dan value secara terpisah
// Mengukur setiap string untuk menentukan layout terbaik
void calculateMaxWidths(const char* lines[], int lineCount, int fontSize, float spacing, int* maxLabelWidth, int* maxValueWidth, Font font) {
    *maxLabelWidth = 0;
    *maxValueWidth = 0;
    for (int i = 0; i < lineCount; i++) {
        char label[50], value[50];
        sscanf(lines[i], "%[^:]:%[^\n]", label, value);
        int labelWidth = MeasureTextEx(font, label, fontSize, spacing).x;
        int valueWidth = MeasureTextEx(font, value, fontSize, spacing).x;
        if (labelWidth > *maxLabelWidth) { *maxLabelWidth = labelWidth; }
        if (valueWidth > *maxValueWidth) { *maxValueWidth = valueWidth; }
    }
}

// Menghitung total tinggi dalam pixel untuk sejumlah baris
// Menggunakan fontSize dan spacing untuk kalkulasi tinggi total
int calculateTotalHeight(int lineCount, int fontSize, float spacing) {
    return (lineCount * (fontSize + spacing)) - spacing;
}

// =============================================================================
// MENU SYSTEM AND NAVIGATION
// Fungsi-fungsi untuk sistem menu dan navigasi antar halaman
// =============================================================================

// Menangani navigasi menu dengan input keyboard
// Memproses input UP/DOWN dan mengembalikan selection baru
int handleMenuNavigation(int currentSelection, int maxOptions, GameResources* resources) {
    if (MOVE_UP) {
        PlaySound(SOUND(resources, SOUND_MOVE));
        return (currentSelection - 1 + maxOptions) % maxOptions;
    }
    if (MOVE_DOWN) {
        PlaySound(SOUND(resources, SOUND_MOVE));
        return (currentSelection + 1) % maxOptions;
    }
    return currentSelection;
}


// Menggambar menu dengan highlight pada item yang dipilih
// Rendering semua opsi menu dengan visual feedback untuk selection
void drawMenu(GameResources* resources, const char* lines[], int lineCount, int selection, int fontSize, Color highlightColor) {
    int totalHeight = calculateTotalHeight(lineCount, fontSize, auto_y(25));
    int startY = (GetScreenHeight() - totalHeight) / 2;

    for (int i = 0; i < lineCount; i++) {
        drawCenteredText(FONT(resources, FONT_BODY), lines[i], startY, fontSize, 2, selection == i ? highlightColor : RAYWHITE);
        startY += auto_y(fontSize) + auto_y(25);
    }
}

// Menampilkan dan menangani menu utama game
// Loop menu utama dengan opsi Play, Settings, Credits, dll
void mainMenu(GameResources* resources) {
    resources->prevState = STATE_MAIN_MENU;
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
    int lineCount = len(lines);
    int selection = 0;

    while (IsKeyDown(KEY_ENTER) || IsKeyDown(KEY_SPACE)) {
        BeginDrawing();
        ClearBackground(PRIMARY_COLOR);
        EndDrawing();
    }

    while (resources->currentState == STATE_MAIN_MENU && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(PRIMARY_COLOR);
        drawBG(resources, BG_MAIN_MENU);
        float totalHeight = calculateTotalTextHeight(defaultFont, lines, lineCount, fontSize, auto_x(2));
        int startY = auto_y(1023) * MIN_SCREEN_HEIGHT / BG(resources, BG_MAIN_MENU).height;

        for (int i = 0; i < lineCount; i++) {
            drawCenteredText(defaultFont, lines[i], startY, fontSize, auto_x(2), selection == i ? ORANGE : DARKGRAY);
            startY += fontSize + spacing;
        }

        selection = handleMenuNavigation(selection, len(lines), resources);

        if (OK_KEY) {
            PlaySound(SOUND(resources, SOUND_SELECT));
            switch (selection) {
            case 0: resources->currentState = STATE_SELECT_LEVEL; break;
            case 1: resources->currentState = STATE_HIGH_SCORES; break;
            case 2: resources->currentState = STATE_HOWTOPLAY; break;
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

// Menampilkan menu pause saat game sedang berjalan
// Menu overlay dengan opsi Resume, Settings, Exit to Menu
void pauseMenu(GameResources* resources) {
    const char* lines[] = { "RESUME", "HOW TO PLAY", "SETTINGS", "MAIN MENU", "QUIT" };
    int selection = 0;
    int fontSize = auto_y(20);
    int lineCount = len(lines);
    bool paused = true;

    while (resources->currentState == STATE_PAUSE && paused) {
        BeginDrawing();
        ClearBackground(Fade(RAYWHITE, 0.9f));
        drawBG(resources, BG_PAUSED);

        drawMenu(resources, lines, lineCount, selection, fontSize, ORANGE);
        EndDrawing();

        selection = handleMenuNavigation(selection, lineCount, resources);

        if (OK_KEY) {
            PlaySound(SOUND(resources, SOUND_SELECT));
            paused = false;
            switch (selection) {
            case 0: resources->currentState = STATE_PLAY; showCountdown(resources); break;
            case 1: resources->currentState = STATE_HOWTOPLAY; break;
            case 2: resources->currentState = STATE_SETTINGS; break;
            case 3: if (confirmBack(resources)) { resources->currentState = STATE_MAIN_MENU; resources->prevState = STATE_MAIN_MENU; } break;
            case 4: resources->currentState = STATE_PAUSE;resources->currentState = STATE_QUIT; break;
            }
        }
    }
}

// Menampilkan menu pemilihan mode permainan
// Interface untuk memilih difficulty atau mode game
void selectMode(GameResources* resources) {
    resources->prevState = STATE_SELECT_LEVEL;
    int currentSelection = resources->settings.mode;
    int targetSelection = currentSelection;
    int lineCount = 11;
    bool selecting = true;
    float transition = 0.0f;
    float transitionSpeed = 4.0f;
    int transitionDirection = 0;

    int skin = resources->settings.skin + TEXTURE_COUNT;
    int currentSkin = skin;
    int targetSkin = skin;
    float skinTransition = 0.0f;
    int skinTransitionDirection = 0;

    while (selecting && !WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        // Update transitions menggunakan fungsi modular
        updateTransition(&transition, &currentSelection, targetSelection, &transitionDirection, deltaTime, transitionSpeed);
        updateTransition(&skinTransition, &currentSkin, targetSkin, &skinTransitionDirection, deltaTime, transitionSpeed);

        BeginDrawing();
        ClearBackground(PRIMARY_COLOR);

        // Render menggunakan fungsi modular
        renderModeTransition(resources, currentSelection, targetSelection, transition, transitionDirection);
        renderSkinTransition(resources, currentSkin, targetSkin, skinTransition, skinTransitionDirection);

        EndDrawing();

        // Handle input hanya jika tidak ada transisi
        if (transitionDirection == 0 && skinTransitionDirection == 0) {
            if (MOVE_DOWN) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                targetSkin = currentSkin - 1;
                if (targetSkin < TEXTURE_SKIN_1) targetSkin = TEXTURE_SKIN_2;
                skinTransitionDirection = 1;  // Transisi ke bawah
                skinTransition = 0.0f;
            }
            if (MOVE_UP) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                targetSkin = currentSkin + 1;
                if (targetSkin > TEXTURE_SKIN_2) targetSkin = TEXTURE_SKIN_1;
                skinTransitionDirection = -1;  // Transisi ke atas
                skinTransition = 0.0f;
            }
            if (MOVE_LEFT) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                targetSelection = currentSelection - 1;
                if (targetSelection < 0) targetSelection = lineCount - 1;
                transitionDirection = -1;
                transition = 0.0f;
            }
            if (MOVE_RIGHT) {
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
                resources->settings.skin = currentSkin - TEXTURE_SKIN_1; // 15
                saveSettings(resources->settings);
                resources->gameLevel = currentSelection;
                showCountdown(resources);
            }
            if (BACK_KEY) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                resources->currentState = STATE_MAIN_MENU;
                selecting = false;
            }
        }

    }
}


// =============================================================================
// DIALOG AND MESSAGE SYSTEM
// Fungsi-fungsi untuk menampilkan dialog dan pesan kepada pengguna
// =============================================================================

// Menampilkan dialog konfirmasi dengan multiple pilihan
// Modal dialog yang menunggu input user untuk konfirmasi aksi
bool showConfirmationDialog(GameResources* resources, const char* message, const char* options[], int optionCount, Color highlightColor) {
    int selection = 1;
    int fontSize = auto_y(20);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(PRIMARY_COLOR);
        drawBG(resources, BG_CONFIRM);

        // Gambar pesan dengan support multiline
        char messageCopy[1024];
        strcpy(messageCopy, message);

        // Split message by newlines
        char* lines[10];
        int lineCount = 0;
        char* line = strtok(messageCopy, "\n");
        while (line != NULL && lineCount < 10) {
            lines[lineCount] = line;
            lineCount++;
            line = strtok(NULL, "\n");
        }

        // Calculate total height for centering
        float totalTextHeight = 0;
        for (int i = 0; i < lineCount; i++) {
            Vector2 textSize = MeasureTextEx(FONT(resources, FONT_HEADER), lines[i], fontSize, 2);
            totalTextHeight += textSize.y + (i < lineCount - 1 ? auto_y(10) : 0);
        }

        // Draw each line centered
        float startY = (GetScreenHeight() - totalTextHeight) / 2 - 50;
        for (int i = 0; i < lineCount; i++) {
            Vector2 textSize = MeasureTextEx(FONT(resources, FONT_HEADER), lines[i], fontSize, 2);
            int startX = (GetScreenWidth() - textSize.x) / 2;
            DrawTextEx(FONT(resources, FONT_HEADER), lines[i], (Vector2) { startX, startY }, fontSize, 2, RAYWHITE);
            startY += textSize.y + auto_y(10);
        }

        // Gambar opsi
        startY += auto_y(40); // Extra spacing before options
        for (int i = 0; i < optionCount; i++) {
            Vector2 textSize = MeasureTextEx(FONT(resources, FONT_BODY), options[i], fontSize, 2);
            int startX = (GetScreenWidth() - textSize.x) / 2;
            DrawTextEx(FONT(resources, FONT_BODY), options[i], (Vector2) { startX, startY }, fontSize, 2, selection == i ? highlightColor : RAYWHITE);
            startY += textSize.y + auto_y(20);
        }
        EndDrawing();

        // ...existing navigation code...
        if (MOVE_UP || MOVE_LEFT) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            selection = (selection - 1 + optionCount) % optionCount;
        }
        if (MOVE_DOWN || MOVE_RIGHT) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            selection = (selection + 1) % optionCount;
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


// Menampilkan pesan informasi dengan subjudul
// Dialog informasi non-interaktif dengan pesan utama dan sub-pesan
void showMessageDialog(GameResources* resources, const char* message, const char* subMessage, Color messageColor, Color subMessageColor) {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(PRIMARY_COLOR);
        drawBG(resources, BG_CONFIRM);

        // Gunakan fungsi yang sudah ada
        drawCenteredText(FONT(resources, FONT_HEADER), message, (GetScreenHeight() - 40) / 2, 20, 2, messageColor);
        drawCenteredText(FONT(resources, FONT_BODY), subMessage, (GetScreenHeight() + 40) / 2, 15, 2, subMessageColor);

        EndDrawing();

        if (GetKeyPressed() != 0) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            break;
        }
    }
}


// Menampilkan konfirmasi keluar dari game
// Dialog "Are you sure you want to exit?" dengan opsi Yes/No
void exitGame(GameResources* resources) {
    if (confirmExit(resources)) {
        saveHiScores(&resources->scores);
        destroyAssets(resources->assets);
        UnloadMusicStream(soundGameplay);
        CloseWindow();
        exit(0);
    }
    else {
        resources->currentState = resources->prevState;
        showCountdown(resources);
    }
}


// Menampilkan konfirmasi kembali ke menu sebelumnya
// Dialog konfirmasi untuk navigasi mundur yang destructive
bool confirmBack(GameResources* resources) {
    const char* options[] = { "Yes", "No" };
    return showConfirmationDialog(resources, "Are you sure you want\nto go back to the main menu?", options, 2, ORANGE);
}


// Menampilkan konfirmasi reset high scores
// Dialog peringatan sebelum menghapus semua high score
bool confirmReset(GameResources* resources) {
    const char* options[] = { "Yes", "No" };
    return showConfirmationDialog(resources, "Reset all high scores?", options, 2, ORANGE);
}


// Menampilkan pesan penolakan reset high scores
// Feedback visual ketika user membatalkan reset
void rejectReset(GameResources* resources) {
    showMessageDialog(resources,
        "Cannot reset scores\nduring gameplay!",
        "Press any key to go back",
        RED,
        RAYWHITE);
}


// =============================================================================
// SCREEN TRANSITIONS AND EFFECTS
// Fungsi-fungsi untuk transisi antar layar dan efek visual
// =============================================================================

// Mengupdate nilai transisi untuk animasi smooth antar layar
// Menggunakan interpolasi untuk perpindahan yang halus
void updateTransition(float* transition, int* current, int target, int* direction, float deltaTime, float speed) {
    if (*transition < 1.0f && *direction != 0) {
        *transition += deltaTime * speed;
        if (*transition >= 1.0f) {
            *transition = 0.0f;
            *current = target;
            *direction = 0;
        }
    }
}

// Merender transisi antar mode permainan
// Animasi slide atau fade saat berganti mode game
void renderModeTransition(GameResources* resources, int current, int target, float transition, int direction) {
    float imgScale = (float)GetScreenHeight() / BGMODE(resources, current).height;
    float scaledWidth = BGMODE(resources, current).width * imgScale;
    float baseXPos = (GetScreenWidth() - scaledWidth) / 2;

    float currentXPos = baseXPos;
    float nextXPos = baseXPos;

    if (direction != 0) {
        currentXPos += (direction * GetScreenWidth() * -transition);
        nextXPos += (direction * GetScreenWidth() * (1.0f - transition));
        DrawTextureEx(BGMODE(resources, target), (Vector2) { nextXPos, 0 }, 0.0f, imgScale, WHITE);
    }

    DrawTextureEx(BGMODE(resources, current), (Vector2) { currentXPos, 0 }, 0.0f, imgScale, WHITE);

    // Draw text overlay
    imgScale = (float)GetScreenHeight() / TXMODE(resources, current).height;
    scaledWidth = TXMODE(resources, current).width * imgScale;
    baseXPos = (GetScreenWidth() - scaledWidth) / 2;
    DrawTextureEx(TXMODE(resources, current), (Vector2) { baseXPos, 0 }, 0.0f, imgScale, WHITE);
}

// Merender transisi antar skin/tema
// Animasi perpindahan visual theme atau character skin
void renderSkinTransition(GameResources* resources, int current, int target, float transition, int direction) {
    float skin_scaledWidth = (float)120 / TEXTURE(resources, current).width;
    float centerX = (GetScreenWidth() - TEXTURE(resources, current).width * skin_scaledWidth) / 2;
    float centerY = (GetScreenHeight() - TEXTURE(resources, current).height * skin_scaledWidth) / 2;

    float currentSkinY = centerY + auto_y(120);
    float baseSkinX = centerX;

    if (direction != 0) {
        float currentOffset = direction * GetScreenWidth() * transition;
        float targetOffset = direction * GetScreenWidth() * (transition - 1.0f);

        // Draw current skin (fading out)
        DrawTextureEx(TEXTURE(resources, current),
            (Vector2) {
            baseSkinX + currentOffset, currentSkinY
        },
            0.0f, skin_scaledWidth,
            Fade(WHITE, 1.0f - transition));

        // Draw target skin (fading in)
        float targetScaledWidth = (float)120 / TEXTURE(resources, target).width;
        DrawTextureEx(TEXTURE(resources, target),
            (Vector2) {
            baseSkinX + targetOffset, currentSkinY
        },
            0.0f, targetScaledWidth,
            Fade(WHITE, transition));
    }
    else {
        // Normal drawing when not transitioning
        DrawTextureEx(TEXTURE(resources, current),
            (Vector2) {
            baseSkinX, currentSkinY
        },
            0.0f, skin_scaledWidth, WHITE);
    }
}


// Menampilkan layar loading dengan progress bar
// Screen loading dengan animasi dan progress indicator
int loadingScreen(GameResources* resources, float* loadingTime) {

    const int blockSize = auto_x(50);
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
    ClearBackground(PRIMARY_COLOR);
    drawBG(resources, BG_PLAIN);
    for (int i = 0; i < 4; i++) {
        Vector2 currentPos = steps[currentStep][i];
        Vector2 nextPos = steps[nextStep][i];

        Vector2 smoothPos = {
            currentPos.x + (nextPos.x - currentPos.x) * alpha,
            currentPos.y + (nextPos.y - currentPos.y) * alpha
        };
        DrawRectangleV(smoothPos, (Vector2) { blockSize, blockSize }, ORANGE);
    }
    EndDrawing();

    return 0;
}

// Menampilkan countdown sebelum game dimulai
// Countdown 3-2-1-GO sebelum gameplay aktif
void showCountdown(GameResources* resources) {
    if (resources->currentState == STATE_PLAY) {
        float counter = 3.0f;

        while (counter > 0.0f && !WindowShouldClose()) {
            counter -= GetFrameTime();
            int currentNumber = (int)ceilf(counter);

            float timeInSecond = counter - floorf(counter);
            float scale = 2.5f - (2.5f - 1.0f) * (1.0f - timeInSecond);

            BeginDrawing();
            ClearBackground(PRIMARY_COLOR);
            drawBG(resources, BG_PLAIN);
            char text[8];
            sprintf(text, "%d", currentNumber);

            Vector2 textSize = MeasureTextEx(FONT(resources, FONT_BODY), text, auto_y(50), auto_x(2));
            float baseSize = auto_y(50);
            float scaledSize = baseSize * scale;

            Vector2 position = { (GetScreenWidth() - textSize.x * scale) / 2,(GetScreenHeight() - textSize.y * scale) / 2 };

            DrawTextEx(FONT(resources, FONT_BODY), text, position, scaledSize, auto_x(2) * scale, Fade(ORANGE, scale > 1.8f ? 2.0f - (scale / 2.0f) : 1.0f));

            EndDrawing();
        }
    }
}

// Menggambar timer countdown dengan efek visual
// Rendering countdown timer dengan animasi dan warna yang berubah
bool drawCountdownTimer(GameResources* resources, float* countdown, int y) {
    float deltaTime = GetFrameTime();
    *countdown -= deltaTime;

    if (*countdown > 0) {
        char countdownText[20];
        sprintf(countdownText, "%.1f", *countdown);
        drawCenteredText(FONT(resources, FONT_BODY), countdownText, y, auto_y(20), 2, RED);
        return false; // Countdown masih berjalan
    }
    return true; // Countdown selesai
}

// =============================================================================
// GAME SCREENS AND VIEWS
// Fungsi-fungsi untuk menampilkan berbagai layar dalam game
// =============================================================================

// Menggambar background berdasarkan ID yang diberikan
// Rendering background image atau pattern untuk berbagai screen
void drawBG(GameResources* resources, uint id) {
    float imgScale = (float)GetScreenHeight() / BG(resources, id).height;
    float scaledWidth = BG(resources, id).width * imgScale;
    float xPos = (GetScreenWidth() - scaledWidth) / 2;

    DrawTextureEx(BG(resources, id), (Vector2) { xPos, 0 }, 0.0f, imgScale, WHITE);
}


// Menampilkan layar credits dengan informasi pengembang
// Screen credits dengan scrolling text atau static information
void showCredits(GameResources* resources) {
    float creditScale = (float)GetScreenWidth() / BG(resources, CREDIT_SCENE).width;
    float creditWidth = BG(resources, CREDIT_SCENE).width * creditScale;
    float xPos = (GetScreenWidth() - creditWidth) / 2;

    float scrollY = GetScreenHeight() - auto_y(50);
    float scrollSpeed = 30.0f;

    while (resources->currentState == STATE_SCENE && !WindowShouldClose()) {
        scrollY -= scrollSpeed * GetFrameTime();

        BeginDrawing();
        ClearBackground(PRIMARY_COLOR);
        drawBG(resources, BG_PLAIN);

        DrawTextureEx(BG(resources, CREDIT_SCENE), (Vector2) { xPos, scrollY }, 0.0f, creditScale, WHITE);
        EndDrawing();

        if (scrollY <= -(BG(resources, CREDIT_SCENE).height * creditScale) ||
            GetKeyPressed() != 0) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            resources->currentState = STATE_SETTINGS;
        }
    }
}


// Menampilkan layar kontrol dan panduan bermain
// Help screen dengan instruksi control dan gameplay
void showHowToPlay(GameResources* resources) {
    while (resources->currentState == STATE_HOWTOPLAY && !WindowShouldClose()) {
        // Rendering loop yang hilang
        BeginDrawing();
        ClearBackground(PRIMARY_COLOR);
        drawBG(resources, BG_HOWTOPLAY);

        // Info text yang konsisten dengan showControls()
        {
            const char* infoText;
            if (resources->prevState == STATE_PLAY || resources->prevState == STATE_PAUSE) {
                infoText = "[B]: Back    [R]: Resume    [F]: CONTROLS";
            }
            else {
                infoText = "[A]: Main Menu    [F]: CONTROLS";
            }
            int infoFontSize = (resources->prevState == STATE_PLAY || resources->prevState == STATE_PAUSE) ? auto_y(15) : auto_y(20);
            drawCenteredText(FONT(resources, FONT_BODY), infoText, auto_y(560), infoFontSize, 2.0f, RAYWHITE);
        }
        EndDrawing();

        // Input handling - logic kondisional
        if (resources->prevState == STATE_PLAY) {
            // Jika datang dari game yang sedang berjalan
            if (IsKeyPressed(KEY_R) || MOVE_LEFT || BACK_KEY) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                resources->currentState = STATE_PLAY;
                showCountdown(resources);
                break;
            }
            if (IsKeyPressed(KEY_P)) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                resources->currentState = STATE_PAUSE;
                break;
            }
        }
        else if (resources->prevState == STATE_PAUSE) {
            // Jika datang dari pause menu
            if (MOVE_LEFT || BACK_KEY) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                resources->currentState = STATE_PAUSE;
                break;
            }
            if (IsKeyPressed(KEY_P)) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                resources->currentState = STATE_PAUSE;
                break;
            }
        }
        else {
            // Jika datang dari menu lain (bukan dari game)
            if (MOVE_LEFT || BACK_KEY) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                resources->currentState = STATE_MAIN_MENU;
                break;
            }
        }

        // Input yang berlaku untuk semua state
        if (FORWARD_KEY) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            resources->currentState = STATE_CONTROLS;
            break;
        }
    }
}

// Menampilkan layar kontrol dan panduan bermain
// Help screen dengan instruksi control dan gameplay
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
        "Mouse Left Click",
        "E / RIGHT SHIFT : Activate Laser"
    };

    int lineCount = len(lines);

    while (resources->currentState == STATE_CONTROLS && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(PRIMARY_COLOR);
        drawBG(resources, BG_CONTROLS);
        int totalHeight = calculateTotalHeight(lineCount, fontSize, spacing);

        int startY = (GetScreenHeight() - totalHeight) / 2;

        int maxLabelWidth, maxValueWidth;
        calculateMaxWidths(lines, lineCount, fontSize, spacing, &maxLabelWidth, &maxValueWidth, FONT(resources, FONT_BODY));

        int padding = auto_x(20);
        int totalWidth = maxLabelWidth + auto_x(10) + MeasureTextEx(FONT(resources, FONT_BODY), ":", fontSize, spacing).x + 5 + maxValueWidth;
        int startX = (GetScreenWidth() - totalWidth) / 2;
        if (startX < padding) { startX = padding; }

        drawLabelsAndValues(lines, lineCount, startX, startY, maxLabelWidth, fontSize, spacing, FONT(resources, FONT_BODY), RAYWHITE);

        {
            const char* infoText;
            if (resources->prevState == STATE_PLAY || resources->prevState == STATE_PAUSE) {
                infoText = "[B]: Back    [R]: Resume    [F]: GUIDE";
            }
            else {
                infoText = "[A]: Main Menu    [F]: GUIDE";
            }
            int infoFontSize = (resources->prevState == STATE_PLAY) ? 15 : 20;
            drawCenteredText(FONT(resources, FONT_BODY), infoText, 560, infoFontSize, 2.0f, RAYWHITE);
        }
        EndDrawing();

        if (resources->prevState == STATE_PLAY) {
            if (IsKeyPressed(KEY_R) || MOVE_LEFT || BACK_KEY) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                resources->currentState = STATE_PLAY;
                showCountdown(resources);
                break;
            }
            if (IsKeyPressed(KEY_P)) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                resources->currentState = STATE_PAUSE;
                break;
            }
        }
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
        if (FORWARD_KEY) {
            PlaySound(SOUND(resources, SOUND_MOVE));
            resources->currentState = STATE_HOWTOPLAY;
            break;
        }
    }
}

// Menampilkan layar pengaturan game
// Settings screen dengan berbagai opsi konfigurasi
void showSettings(GameResources* resources) {
    int fontSize = auto_y(20);
    int spacing = auto_x(4);
    int menuSpacing = auto_y(25);

    // Pisahkan array toggle dan menu
    char toggleLines[2][100];
    snprintf(toggleLines[0], sizeof(toggleLines[0]), "Music : %s", resources->settings.music ? "On" : "Off");
    snprintf(toggleLines[1], sizeof(toggleLines[1]), "Sfx   : %s", resources->settings.sfx ? "On" : "Off");
    SetSoundVolume(SOUND(resources, SOUND_MOVE), resources->settings.sfx ? 1.0f : 0.0f);
    SetSoundVolume(SOUND(resources, SOUND_SELECT), resources->settings.sfx ? 1.0f : 0.0f);

    const char* menuLines[] = {
        "Controls",
        "Reset High Scores",
        "Credits"
    };

    // Buat array pointer untuk toggle agar kompatibel dengan fungsi modul
    const char* togglePointers[2];
    togglePointers[0] = toggleLines[0];
    togglePointers[1] = toggleLines[1];

    int toggleCount = 2;
    int menuCount = 3;
    int totalCount = toggleCount + menuCount;
    int selection = 0;
    bool editing = false;

    while (resources->currentState == STATE_SETTINGS && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(PRIMARY_COLOR);
        drawBG(resources, BG_SETTINGS);

        int totalHeight = calculateTotalHeight(totalCount, fontSize, menuSpacing);
        int startY = (GetScreenHeight() - totalHeight) / 2;

        // Gambar toggle options (Music/Sfx) menggunakan modul
        int maxLabelWidth, maxValueWidth;
        calculateMaxWidths(togglePointers, toggleCount, fontSize, spacing, &maxLabelWidth, &maxValueWidth, FONT(resources, FONT_BODY));

        int padding = auto_x(20);
        int totalWidth = maxLabelWidth + auto_x(10) + MeasureTextEx(FONT(resources, FONT_BODY), ":", fontSize, spacing).x + auto_x(5) + maxValueWidth;
        int startX = (GetScreenWidth() - totalWidth) / 2;
        if (startX < padding) { startX = padding; }

        // Gambar toggle dengan highlighting untuk selected item
        int y = startY;
        for (int i = 0; i < toggleCount; i++) {
            bool isSelected = (selection == i);
            drawToggleOption(FONT(resources, FONT_BODY), togglePointers[i], startX, y, maxLabelWidth, fontSize, spacing, isSelected, editing, RAYWHITE);
            y += fontSize + spacing;
        }

        // Gambar menu options menggunakan modul
        y += 50;
        for (int i = 0; i < menuCount; i++) {
            int globalIndex = i + toggleCount;
            bool isSelected = (selection == globalIndex);
            drawCenteredText(FONT(resources, FONT_BODY), menuLines[i], y, auto_x(20), 2.0f,
                isSelected ? ORANGE : RAYWHITE);
            y += auto_y(50);
        }

        // Gambar info text menggunakan modul
        const char* infoText;
        if (resources->prevState == STATE_PLAY || resources->prevState == STATE_PAUSE) {
            infoText = "[B]: Back    [R]: Resume";
        }
        else {
            infoText = "[A]: Main Menu";
        }
        drawCenteredText(FONT(resources, FONT_BODY), infoText, 560, auto_y(20), 2.0f, RAYWHITE);

        EndDrawing();

        // Handle input
        if (!editing) {
            selection = handleMenuNavigation(selection, totalCount, resources);

            if (OK_KEY) {
                PlaySound(SOUND(resources, SOUND_SELECT));
                if (selection < toggleCount) {
                    editing = true;
                }
                else {
                    // Menu option selected
                    switch (selection - toggleCount) {
                    case 0: resources->currentState = STATE_CONTROLS; break;
                    case 1: resetHiScores(resources); break;
                    case 2: resources->currentState = STATE_SCENE; break;
                    }
                }
            }
        }
        else {
            // Editing mode - handle toggle changes
            if (MOVE_LEFT || MOVE_RIGHT || MOVE_UP || MOVE_DOWN) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                char label[50], value[50];
                sscanf(togglePointers[selection], "%[^:]:%[^\n]", label, value);

                if (strcmp(value, " On") == 0) {
                    snprintf(toggleLines[selection], sizeof(toggleLines[selection]), "%s: Off", label);
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
                    snprintf(toggleLines[selection], sizeof(toggleLines[selection]), "%s: On", label);
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

                // Update pointer setelah mengubah string
                togglePointers[selection] = toggleLines[selection];
                saveSettings(resources->settings);
            }

            if (OK_KEY) {
                editing = false;
            }
        }

        // Handle special keys for different states
        if (resources->prevState == STATE_PLAY) {
            if (IsKeyPressed(KEY_R)) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                resources->currentState = STATE_PLAY;
                showCountdown(resources);
                break;
            }
            if (IsKeyPressed(KEY_P)) {
                PlaySound(SOUND(resources, SOUND_MOVE));
                resources->currentState = STATE_PAUSE;
                break;
            }
        }
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
}

// Menampilkan layar high scores
// Leaderboard dengan best scores untuk setiap mode
void showHiScore(GameResources* resources) {

    int fontSize = auto_y(20);
    int spacing = auto_x(4);
    resources->scores = *loadHiScores();

    char lines[MAX_LEVELS][65];

    extern char* levelNames[];
    int i = 0;
    while (i < MAX_LEVELS) {
        SLLNode* searchName = resources->scores.head;
        while (searchName != NULL) {
            HiScore* searchScore = (HiScore*)searchName->data;
            if (strcmp(searchScore->mode, levelNames[i]) == 0) {
                snprintf(lines[i], sizeof(lines[i]), "%s: %lld", searchScore->mode, searchScore->score);
                break;
            }
            searchName = searchName->next;
        }
        i++;
    }
    const char* linePtrs[MAX_LEVELS];
    for (int i = 0; i < MAX_LEVELS; i++) {
        linePtrs[i] = lines[i];
    }
    int lineCount = len(linePtrs);

    while (resources->currentState == STATE_HIGH_SCORES && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(PRIMARY_COLOR);
        drawBG(resources, BG_HIGHSCORES);
        int totalHeight = calculateTotalHeight(lineCount, fontSize, spacing);
        int startY = (GetScreenHeight() - totalHeight) / 2;

        int maxLabelWidth, maxValueWidth;
        calculateMaxWidths(linePtrs, lineCount, fontSize, spacing, &maxLabelWidth, &maxValueWidth, FONT(resources, FONT_BODY));

        int padding = auto_x(20);
        int totalWidth = maxLabelWidth + 10 + MeasureTextEx(FONT(resources, FONT_BODY), ":", fontSize, spacing).x + 5 + maxValueWidth;
        int startX = (GetScreenWidth() - totalWidth) / 2;

        if (startX < padding) {
            startX = padding;
        }

        int y = startY;
        drawLabelsAndValues(linePtrs, lineCount, startX, startY, maxLabelWidth, fontSize, spacing, FONT(resources, FONT_BODY), RAYWHITE);
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

// Menampilkan layar game over dengan skor final
// End screen dengan skor, ranking, dan opsi restart/menu
void gameOver(GameResources* resources, ll currentScore) {
    const char* message = "GAME OVER";
    const char* options[] = { "Retry", "Main Menu" };
    int selection = 1;
    int fontSize = auto_y(20);
    int lineCount = len(options);
    bool inGameOver = true;
    PlaySound(SOUND(resources, SOUND_DEATH));

    // Load current high scores untuk tampilan
    SingleLinkedList scores = *loadHiScores();
    ll currentHighScore = getCurrentModeHighScore(resources);

    float countdown = 2.0f;  // 2 second countdown
    bool canSelect = false;

    while (inGameOver && !WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        countdown -= deltaTime;
        if (countdown <= 0) canSelect = true;

        BeginDrawing();
        ClearBackground(PRIMARY_COLOR);
        drawBG(resources, BG_PLAIN);

        // Draw GAME OVER message menggunakan fungsi modular
        drawCenteredText(FONT(resources, FONT_HEADER), message, (GetScreenHeight() / 2) - 350, 30, 2, RAYWHITE);

        // Draw scores menggunakan fungsi modular
        int startY = (GetScreenHeight() / 2) - 100;
        drawGameOverScore(resources, currentScore, currentHighScore, &startY);

        if (!canSelect) {
            // Draw countdown menggunakan fungsi modular
            if (!drawCountdownTimer(resources, &countdown, startY)) {
                // Countdown masih berjalan, tidak perlu draw options
            }
        }
        else {
            // Draw menu options menggunakan fungsi modular
            drawMenu(resources, options, lineCount, selection, fontSize, ORANGE);

            // Handle input menggunakan fungsi modular
            selection = handleMenuNavigation(selection, lineCount, resources);

            if (OK_KEY) {
                PlaySound(SOUND(resources, SOUND_SELECT));
                switch (selection) {
                case 0: // Retry
                    resources->currentState = STATE_PLAY;
                    inGameOver = false;
                    showCountdown(resources);
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

// Menggambar komponen skor pada layar game over
// Rendering skor current, high score, dan ranking
void drawGameOverScore(GameResources* resources, ll currentScore, ll currentHighScore, int* startY) {
    int fontSize = auto_y(20);

    // High Score dengan [NEW] tag jika diperlukan
    char highScoreText[50];
    sprintf(highScoreText, "High Score: %lld", currentHighScore);

    if (currentScore > currentHighScore) {
        const char* newTag = "[NEW]";
        Vector2 newTagSize = MeasureTextEx(FONT(resources, FONT_BODY), newTag, 12, 2);
        Vector2 highScoreSize = MeasureTextEx(FONT(resources, FONT_BODY), highScoreText, fontSize, 2);

        Rectangle newBox = {
            (GetScreenWidth() - highScoreSize.x) / 2,
            *startY + auto_y(3),
            newTagSize.x + 10,
            newTagSize.y + auto_y(6)
        };
        DrawRectangle(newBox.x, newBox.y, newBox.width, newBox.height, ORANGE);
        drawCenteredText(FONT(resources, FONT_BODY), newTag, newBox.y + 3, 12, 2, WHITE);
        *startY += 25;
    }

    // Draw High Score
    drawCenteredText(FONT(resources, FONT_BODY), highScoreText, *startY, fontSize, 2, RAYWHITE);
    *startY += 30;

    // Draw Current Score  
    char scoreText[50];
    sprintf(scoreText, "Score: %lld", currentScore);
    drawCenteredText(FONT(resources, FONT_BODY), scoreText, *startY, fontSize, 2, ORANGE);
    *startY += 60;
}

// =============================================================================
// GAME STATE MANAGEMENT
// Fungsi-fungsi untuk mengelola state dan alur permainan
// =============================================================================

// Keluar dari game dan cleanup semua resource
// Shutdown sequence yang aman dengan pembersihan memori
bool confirmExit(GameResources* resources) {
    const char* options[] = { "Yes", "No" };
    return showConfirmationDialog(resources, "Are you sure you want to exit?", options, 2, ORANGE);
}

// Reset semua high scores ke nilai default
// Menghapus semua record high score dan kembalikan ke 0
void resetHiScores(GameResources* resources) {
    if (resources->prevState == STATE_PAUSE
        || resources->prevState == STATE_PLAY) {
        rejectReset(resources);
        return;
    }
    if (confirmReset(resources)) {
        SingleLinkedList scores = *initializeDb();
        saveHiScores(&scores);
    }
    else return;
}

// Menunggu hingga semua key dilepas
// Mencegah input spam dengan menunggu key release
void waitForKeyRelease(void) {
    while (IsKeyDown(KEY_ENTER) || IsKeyDown(KEY_SPACE)) {
        BeginDrawing();
        ClearBackground(PRIMARY_COLOR);
        EndDrawing();
    }
}


// =============================================================================
// GAME MECHANICS AND LOGIC
// Fungsi-fungsi untuk logika inti permainan dan mekanika gameplay
// =============================================================================

// Menampilkan dan mengupdate gameplay utama
// Main game loop dengan rendering dan update logic
void displayGame(GameResources* resources) {

    if (!resources) return;
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
    resources->prevState = STATE_PLAY;

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
    float imgScale = (float)GetScreenHeight() / BG(resources, BG_GAMEAREA).height;
    DrawTexturePro(BG(resources, BG_GAMEAREA), (Rectangle) { 0, 0, BG(resources, BG_GAMEAREA).width, BG(resources, BG_GAMEAREA).height },
        (Rectangle) {
        gameAreaX, gameAreaY, gameWidth, GetScreenHeight()
    }, (Vector2) { 0, 0 }, 0.0f, WHITE);

    float sidebarX = gameWidth;
    float sidebarWidth = GetScreenWidth() - gameWidth;

    imgScale = (float)GetScreenHeight() / BG(resources, BG_UIGAME).height;
    DrawTexturePro(BG(resources, BG_UIGAME), (Rectangle) { 0, 0, BG(resources, BG_UIGAME).width, BG(resources, BG_UIGAME).height },
        (Rectangle) {
        sidebarX, 0, sidebarWidth, GetScreenHeight()
    }, (Vector2) { 0, 0 }, 0.0f, WHITE);


    // Bottom boundary 
    DrawRectangle(gameAreaX, gameHeight, gameWidth, auto_y(1), RAYWHITE);
    // drawBG(resources, BG_PLAY);

    drawBlocks(gameContext, resources);
    drawPowerUp(gameContext, resources);

    if ((IsKeyPressed(KEY_E) || IsKeyPressed(KEY_RIGHT_SHIFT)) && gameContext->laserCooldown <= 0) {
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
    SLLNode* prev = NULL;
    SLLNode* node = gameContext->activePowerups.Front;
    while (node != NULL) {
        PowerUp* powerup = (PowerUp*)node->data;
        if (powerup && powerup->active) {
            powerup->duration -= deltaTime;

            if (powerup->duration <= 0) {
                // Reverse efek
                switch (powerup->type) {
                case POWERUP_SPEED_UP:
                    gameContext->rowAddDelay *= 4;
                    break;
                case POWERUP_SLOW_DOWN:
                    gameContext->rowAddDelay = (int)(gameContext->rowAddDelay / 2.5f);
                    break;
                }
                // Remove node from queue
                SLLNode* toDelete = node;
                if (prev == NULL) {
                    // Removing front
                    gameContext->activePowerups.Front = node->next;
                    node = node->next;
                }
                else {
                    prev->next = node->next;
                    node = node->next;
                }
                if (gameContext->activePowerups.Rear == toDelete) {
                    gameContext->activePowerups.Rear = prev;
                }
                free(toDelete->data);
                free(toDelete);
                gameContext->activePowerups.size--;
                continue;
            }
        }
        prev = node;
        node = node->next;
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
    // Player and bullets
    shooter(&P.x, &P.y, resources);
    moveSet(&P.x, resources);

    MoveBullets(gameContext->bullets);
    DrawBullets(gameContext->bullets, resources);

    // Handle input
    if (SHOOT_KEY) {
        ShootBullets(gameContext->bullets, (Vector2) { P.x, P.y }, & gameContext->bulletCount,
            & gameContext->canShoot, resources);
    }
    if (SHOOT_RELEASE) {
        gameContext->canShoot = true;
    }

    // Keperluan debug
    // if (IsKeyPressed(KEY_G)) {
    //     gameOver(resources);
    // }

    if (IsKeyPressed(KEY_P)) {
        printf("[DEBUG] P key pressed! Current state: %d\n", resources->currentState);
        printf("[DEBUG] P key pressed! Previous state: %d\n", resources->prevState);
        resources->prevState = STATE_PLAY;
        resources->currentState = STATE_PAUSE;
        return;
    }
    EndDrawing();
    if (IsKeyPressed(KEY_H)) {
        resources->currentState = STATE_HOWTOPLAY;
        return;
    }
    if (IsKeyPressed(KEY_F1)) {
        printGrid(gameContext);
    }
}


// Mendapatkan kecepatan game berdasarkan mode yang dipilih
// Mengembalikan speed multiplier untuk berbagai difficulty
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


// Mendapatkan range jumlah blok untuk mode tertentu
// Menentukan min/max blok yang muncul berdasarkan difficulty
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


// Mengecek apakah kondisi game over tercapai
// Validasi kondisi kalah seperti blok mencapai batas bawah atau lives habis
bool isGameOverCheck(Game* game) {
    if (!game || !game->grid) return false;

    DLLNode* lastRowNode = game->grid->tail;
    if (!lastRowNode) return false;

    // ambil linked list yang merepresentasikan baris terakir
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

// =============================================================================
// BLOCK MANAGEMENT SYSTEM
// Fungsi-fungsi untuk mengelola blok dalam grid permainan
// =============================================================================

// Mendapatkan node row dari grid berdasarkan index
// Mengakses baris tertentu dalam doubly linked list grid
DLLNode* getRowNode(Game* game, int rowIndex) {
    if (!game || !game->grid || rowIndex < 0) return NULL;

    DLLNode* rowNode = game->grid->head;
    for (int i = 0; i < rowIndex && rowNode; i++) {
        rowNode = rowNode->next;
    }
    return rowNode;
}


// Mendapatkan node blok dari row berdasarkan index kolom
// Mengakses blok tertentu dalam linked list row
DLLNode* getBlockNode(DoublyLinkedList* row, int colIndex) {
    if (!row || colIndex < 0) return NULL;

    DLLNode* blockNode = row->head;
    for (int i = 0; i < colIndex && blockNode; i++) {
        blockNode = blockNode->next;
    }
    return blockNode;
}

// Mendapatkan pointer ke blok pada posisi tertentu
// Akses langsung ke data blok pada koordinat grid
Block* getBlockAt(Game* game, int row, int col) {
    if (!game || !game->grid) return NULL;

    DLLNode* rowNode = getRowNode(game, row);
    if (!rowNode) return NULL;

    DoublyLinkedList* currentRow = (DoublyLinkedList*)rowNode->data;
    if (!currentRow) return NULL;

    DLLNode* blockNode = getBlockNode(currentRow, col);
    if (!blockNode) return NULL;

    return (Block*)blockNode->data;
}

// Mengaktifkan blok pada posisi tertentu
// Set status blok menjadi aktif/visible di grid
bool activateBlockAt(Game* game, int row, int col) {
    Block* block = getBlockAt(game, row, col);
    if (block && !block->active) {
        block->active = true;
        return true;
    }
    return false;
}

// Menonaktifkan blok pada posisi tertentu
// Set status blok menjadi non-aktif/destroyed
bool deactivateBlockAt(Game* game, int row, int col) {
    Block* block = getBlockAt(game, row, col);
    if (block && block->active) {
        block->active = false;
        return true;
    }
    return false;
}

// Menghapus semua blok dalam satu baris
// Reset semua blok dalam row menjadi non-aktif
void clearRow(DoublyLinkedList* row) {
    if (!row) return;

    DLLNode* blockNode = row->head;
    while (blockNode) {
        Block* block = (Block*)blockNode->data;
        if (block) {
            block->active = false;
        }
        blockNode = blockNode->next;
    }
}

// Mengecek apakah ada blok aktif dalam baris
// Validasi apakah row masih memiliki blok yang visible
bool hasActiveBlocksInRow(DoublyLinkedList* row) {
    if (!row) return false;

    DLLNode* blockNode = row->head;
    while (blockNode) {
        Block* block = (Block*)blockNode->data;
        if (block && block->active) {
            return true;
        }
        blockNode = blockNode->next;
    }
    return false;
}

// Mengecek apakah ada blok aktif di bawah posisi tertentu
// Deteksi collision untuk blok yang jatuh
bool hasActiveBlockBelow(Game* game, int row) {
    if (!game || !game->grid) return false;

    // Gunakan getRowNode untuk mendapatkan row node
    DLLNode* rowNode = getRowNode(game, row);
    if (!rowNode) return false;

    // cek setiap row di bawahnya
    rowNode = rowNode->next;
    while (rowNode) {
        DoublyLinkedList* currentRow = (DoublyLinkedList*)rowNode->data;
        if (currentRow && hasActiveBlocksInRow(currentRow)) {
            return true;
        }
        rowNode = rowNode->next;
    }

    return false;
}

// =============================================================================
// BLOCK MOVEMENT AND PHYSICS
// Fungsi-fungsi untuk pergerakan blok dan fisika dalam game
// =============================================================================

// Menangani pergerakan blok berdasarkan range yang diberikan
// Logic utama untuk movement dan spawning blok baru
void handleBlockMovement(Game* game, int minBlocks, int maxBlocks) {
    int emptyColLength[MAX_COLUMNS] = { 0 };
    int totalEmptyColumns = 0;

    // hitung empty kolom menggunakan helper
    for (int j = 0; j < MAX_COLUMNS; j++) {
        int emptyCount = 0;

        for (int i = 0; i < MAX_ROWS; i++) {
            Block* block = getBlockAt(game, i, j);
            if (block && !block->active) {
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

    moveBlocksDown(game);
    generateNewBlocks(game, minBlocks, maxBlocks, emptyColLength, totalEmptyColumns);
}

// Memindahkan semua blok ke bawah satu level
// Gravity effect yang menggerakkan blok turun
void moveBlocksDown(Game* game) {
    if (!game || !game->grid) return;

    // Gunakan copyRowBlocks untuk setiap pasangan row
    for (int i = MAX_ROWS - 2; i >= 0; i--) {
        DLLNode* currentRowNode = getRowNode(game, i);
        DLLNode* nextRowNode = getRowNode(game, i + 1);

        if (currentRowNode && nextRowNode) {
            DoublyLinkedList* currentRow = (DoublyLinkedList*)currentRowNode->data;
            DoublyLinkedList* nextRow = (DoublyLinkedList*)nextRowNode->data;

            copyRowBlocks(currentRow, nextRow);
        }
    }

    // Clear first row menggunakan helper
    DLLNode* firstRowNode = getRowNode(game, 0);
    if (firstRowNode) {
        DoublyLinkedList* firstRow = (DoublyLinkedList*)firstRowNode->data;
        clearRow(firstRow);
    }
}

// Menyalin blok dari source row ke destination row
// Utility untuk transfer blok antar baris
void copyRowBlocks(DoublyLinkedList* sourceRow, DoublyLinkedList* destRow) {
    if (!sourceRow || !destRow) return;

    DLLNode* sourceBlock = sourceRow->head;
    DLLNode* destBlock = destRow->head;

    while (sourceBlock && destBlock) {
        Block* source = (Block*)sourceBlock->data;
        Block* dest = (Block*)destBlock->data;
        if (source && dest) {
            dest->active = source->active;
        }
        sourceBlock = sourceBlock->next;
        destBlock = destBlock->next;
    }
}


// Menggeser baris-baris ke atas mulai dari startRow
// Shift rows untuk menghapus baris yang completed
void shiftRowsUp(Game* game, int startRow) {
    if (!game || !game->grid) return;

    DLLNode* rowNode = getRowNode(game, startRow);

    // Shift rows up dari startRow ke second-to-last row
    while (rowNode && rowNode->next) {
        DoublyLinkedList* currentRow = (DoublyLinkedList*)rowNode->data;
        DoublyLinkedList* nextRow = (DoublyLinkedList*)rowNode->next->data;

        copyRowBlocks(nextRow, currentRow);
        rowNode = rowNode->next;
    }

    // Clear last row
    if (rowNode) {
        DoublyLinkedList* lastRow = (DoublyLinkedList*)rowNode->data;
        clearRow(lastRow);
    }
}


// Mengecek apakah satu baris penuh terisi blok
// Deteksi line completion untuk scoring
bool isRowFull(Game* game, int row) {
    if (!game || !game->grid) return false;

    DLLNode* rowNode = getRowNode(game, row);
    if (!rowNode) return false;

    DoublyLinkedList* currentRow = (DoublyLinkedList*)rowNode->data;
    if (!currentRow) return false;

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


// Menangani baris yang terisi penuh
// Logic untuk menghapus line, scoring, dan shift rows
void handleFullRow(Game* game, int row) {
    if (isRowFull(game, row)) {
        if (hasActiveBlockBelow(game, row)) {
            shiftRowsUp(game, row);
            addScore(game, row);
        }
        else {
            DLLNode* rowNode = getRowNode(game, row);
            if (rowNode) {
                DoublyLinkedList* currentRow = (DoublyLinkedList*)rowNode->data;
                clearRow(currentRow);
            }
            addScore(game, row);
        }
    }
}


// =============================================================================
// BLOCK GENERATION SYSTEM
// Fungsi-fungsi untuk menghasilkan dan menempatkan blok baru
// =============================================================================

// Menghasilkan blok baru berdasarkan parameter yang diberikan
// Spawn blok baru dengan algoritma distribusi yang seimbang
void generateNewBlocks(Game* game, int minBlocks, int maxBlocks, int* emptyColLength, int totalEmptyColumns) {
    if (!game || !game->grid) return;

    int numBlocks = minBlocks + (rand() % (maxBlocks - minBlocks + 1));
    int remainingBlocks = numBlocks;

    // isi critical gaps first menggunakan helper
    if (totalEmptyColumns > 0) {
        for (int col = 0; col < MAX_COLUMNS && remainingBlocks > 0; col++) {
            if (emptyColLength[col] >= 4) {
                if (activateBlockAt(game, 0, col)) {
                    remainingBlocks--;
                }
            }
        }
    }

    // distribusi blok dengan gap maksimal 2 kolom
    if (remainingBlocks > 0) {
        int lastPlacedCol = -3;
        int attempts = 0;
        int maxAttempts = MAX_COLUMNS * 2;

        while (remainingBlocks > 0 && attempts < maxAttempts) {
            int pos = rand() % MAX_COLUMNS;

            if (pos - lastPlacedCol >= 2) {
                if (activateBlockAt(game, 0, pos)) {
                    lastPlacedCol = pos;
                    remainingBlocks--;
                }
            }
            attempts++;
        }

        // jika masih ada blok tersisa, tempatkan di posisi random
        while (remainingBlocks > 0) {
            int pos = rand() % MAX_COLUMNS;
            if (activateBlockAt(game, 0, pos)) {
                remainingBlocks--;
            }
        }
    }
}

// Mengisi gap kritis untuk mencegah pola tidak adil
// Prioritas spawn pada kolom yang kosong untuk balance
int fillCriticalGaps(Game* game, int remainingBlocks, int* emptyColLength) {
    if (!game || !game->grid) return remainingBlocks;

    DLLNode* firstRowNode = game->grid->head;
    if (!firstRowNode) return remainingBlocks;

    DoublyLinkedList* firstRow = (DoublyLinkedList*)firstRowNode->data;
    if (!firstRow) return remainingBlocks;

    DLLNode* blockNode = firstRow->head;
    int col = 0;

    // cek setiap kolom yang kritis
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


// Mengisi blok yang tersisa secara random
// Distribusi blok sisa dengan probabilitas yang merata
void fillRemainingBlocks(Game* game, int remainingBlocks) {
    int maxAttemptsPerBlock = 3;

    DLLNode* firstRowNode = game->grid->head;
    if (!firstRowNode) return;

    DoublyLinkedList* firstRow = (DoublyLinkedList*)firstRowNode->data;
    if (!firstRow) return;

    while (remainingBlocks > 0) {
        for (int attempt = 0; attempt < maxAttemptsPerBlock && remainingBlocks > 0; attempt++) {
            int pos = rand() % MAX_COLUMNS;

            // traversal (cari target)
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
        remainingBlocks--; // Mencegah inf loop jika tak meletakkan semua blok
    }
}

// Memproses hit bullet pada grid position tertentu
// Damage application, explosion effect, dan bullet removal
void processBulletHit(Game* game, int gridX, int gridY, Bullets* bullets) {
    bool hasSpecialBullet = false;
    SLLNode* node = game->activePowerups.Front;
    while (node) {
        PowerUp* powerup = (PowerUp*)node->data;
        if (powerup && powerup->active &&
            powerup->type == POWERUP_SPECIAL_BULLET) {
            hasSpecialBullet = true;
            break;
        }
        node = node->next;
    }

    if (hasSpecialBullet) {
        // Spesial bullet: hapus sebaris menggunakan helper
        DLLNode* rowNode = getRowNode(game, gridY);
        if (rowNode) {
            DoublyLinkedList* row = (DoublyLinkedList*)rowNode->data;
            clearRow(row);
        }
        game->score += 40;
    }
    else if (gridY < MAX_ROWS - 1) {
        // Aktifkan block di baris bawah menggunakan helper
        activateBlockAt(game, gridY + 1, gridX);
    }

    // cek baris penuh
    for (int row = 0; row < MAX_ROWS; row++) {
        handleFullRow(game, row);
    }

    if (bullets != NULL) {
        bullets->active = false;
    }
    game->score += 10;
}


// Validasi apakah posisi grid masih dalam bounds
// Boundary checking untuk mencegah out-of-bounds access
bool isValidGridPosition(int x, int y) {
    return x >= 0 && x < MAX_COLUMNS && y >= 0 && y < MAX_ROWS;
}


// =============================================================================
// BLOCK RENDERING AND UPDATES
// Fungsi-fungsi untuk menggambar dan memperbarui tampilan blok
// =============================================================================

// Mengupdate state semua blok (animasi, effects, dll)
// Update logic untuk visual effects dan state transitions
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


// Inisialisasi visual dan data blok
// Setup awal untuk rendering dan collision data
void initBlocks(Game* game, GameResources* resources) {
    DLLNode* firstRowNode = game->grid->head;
    if (!firstRowNode) return;
    DoublyLinkedList* firstRow = (DoublyLinkedList*)firstRowNode->data;
    if (!firstRow) return;

    // Clear all blocks first
    DLLNode* rowNode = game->grid->head;
    while (rowNode) {
        DoublyLinkedList* row = (DoublyLinkedList*)rowNode->data;
        clearRow(row);
        rowNode = rowNode->next;
    }

    int minBlocks, maxBlocks;
    getBlockRangeForMode(resources->gameLevel, &minBlocks, &maxBlocks);
    int numBlocks = minBlocks + (rand() % (maxBlocks - minBlocks + 1));

    while (numBlocks > 0) {
        int pos = rand() % MAX_COLUMNS;
        DLLNode* blockNode = getBlockNode(firstRow, pos);

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

// Menggambar semua blok pada layar
// Rendering loop untuk semua blok dengan textures dan effects
void drawBlocks(Game* game, GameResources* resources) {

    float blockSize = auto_x(32);

    DLLNode* rowNode = game->grid->head;
    int rowIndex = 0;

    while (rowNode && rowIndex < MAX_ROWS) {
        DoublyLinkedList* row = (DoublyLinkedList*)rowNode->data;
        DLLNode* blockNode = row->head;
        int colIndex = 0;

        // iterasi blok per baris
        while (blockNode && colIndex < MAX_COLUMNS) {
            Block* block = (Block*)blockNode->data;
            if (block && block->active) {
                if (TEXTURE(resources, TEXTURE_BLOCK).id != 0) {
                    float texScale = (float)blockSize / (float)TEXTURE(resources, TEXTURE_BLOCK).width;
                    DrawTextureEx(TEXTURE(resources, TEXTURE_BLOCK), (Vector2) { colIndex* blockSize, rowIndex* blockSize }, 0.0f, texScale, WHITE);
                }
                else {
                    DrawRectangle(colIndex * blockSize, rowIndex * blockSize, blockSize, blockSize, BLUE);
                }
            }
            blockNode = blockNode->next;
            colIndex++;
        }
        rowNode = rowNode->next;
        rowIndex++;
    }
}


// Debug function untuk print grid ke console
// Development utility untuk visualisasi grid state
void printGrid(Game* game) {
    system("cls");
    printf("[LOG] \n--- Grid State ---\n");

    DLLNode* rowNode = game->grid->head;
    while (rowNode) {
        DoublyLinkedList* row = (DoublyLinkedList*)rowNode->data;
        DLLNode* blockNode = row->head;

        while (blockNode) {
            Block* block = (Block*)blockNode->data;
            printf(block->active ? "[x]" : "   ");
            blockNode = blockNode->next;
        }
        printf("[LOG] \n");
        rowNode = rowNode->next;
    }

    printf("[LOG] -----------------\n");
}


// =============================================================================
// GAME UI AND HUD SYSTEM
// Fungsi-fungsi untuk antarmuka permainan dan heads-up display
// =============================================================================

// Menggambar ikon power-up dengan timer visual
// Rendering power-up icon dengan countdown indicator
void drawPowerUpIcon(Texture2D iconTexture, Vector2 position, float iconSize, float duration, Color timerColor) {
    float scale = iconSize / iconTexture.width;

    // Draw icon
    DrawTextureEx(iconTexture, position, 0, scale, WHITE);

    // Draw timer below icon
    char timerText[8];
    sprintf(timerText, "%.1fs", duration);
    Vector2 timerSize = MeasureTextEx(GetFontDefault(), timerText, 15, 2);
    float timerX = position.x + (iconSize - timerSize.x) / 2;
    float timerY = position.y + iconSize + 5;

    DrawTextEx(GetFontDefault(), timerText, (Vector2) { timerX, timerY }, 15, 2, timerColor);
}


// Mendapatkan visual data untuk power-up tertentu
// Mengambil texture, color, dan effect data untuk power-up
PowerUpVisuals getPowerUpVisuals(GameResources* resources, PowerUpType type) {
    PowerUpVisuals visuals;

    switch (type) {
    case POWERUP_SPEED_UP:
        visuals.texture = TEXTURE(resources, TEXTURE_SPEEDUP);
        visuals.timerColor = GREEN;
        break;
    case POWERUP_SLOW_DOWN:
        visuals.texture = TEXTURE(resources, TEXTURE_SLOWDOWN);
        visuals.timerColor = RED;
        break;
    case POWERUP_SPECIAL_BULLET:
        visuals.texture = TEXTURE(resources, TEXTURE_SPECIAL_BULLET);
        visuals.timerColor = BLUE;
        break;
    default:
        visuals.texture = TEXTURE(resources, TEXTURE_RANDOM);
        visuals.timerColor = WHITE;
    }

    return visuals;
}


// Menggambar indicator lives/health player
// Rendering heart icons atau health bar
void drawLives(GameResources* resources, int lives, Vector2 startPos, float spacing) {
    float scale = auto_x(30.0f) / 640.0f;

    for (int i = 0; i < lives; i++) {
        Vector2 position = { startPos.x + (i * spacing), startPos.y };
        DrawTextureEx(TEXTURE(resources, TEXTURE_HEART), position, 0, scale, WHITE);
    }
}


// Menggambar tombol laser dengan cooldown indicator
// UI button dengan visual feedback untuk special ability
void drawLaserButton(GameResources* resources, float cooldown, float uiAreaX, float uiAreaWidth, float y) {
    float scale = (float)auto_x(80) / TEXTURE(resources, TEXTURE_LASER_BUTTON).width;
    float imgSize = scale * TEXTURE(resources, TEXTURE_LASER_BUTTON).width;
    float xPos = uiAreaX + (uiAreaWidth - imgSize) / 2;

    if (cooldown > 0) {
        // Draw dimmed button
        DrawTextureEx(TEXTURE(resources, TEXTURE_LASER_BUTTON),
            (Vector2) {
            xPos, y
        }, 0, scale,
            (Color) {
            255, 255, 255, 150
        });

            // Draw cooldown text
            char cooldownText[8];
            sprintf(cooldownText, "%.1f", cooldown);
            Vector2 textSize = MeasureTextEx(GetFontDefault(), cooldownText, auto_x(20), auto_x(2));

            float textX = xPos + (imgSize - textSize.x) / 2;
            float textY = y + (TEXTURE(resources, TEXTURE_LASER_BUTTON).height * scale - textSize.y) / 2;

            DrawTextEx(GetFontDefault(), cooldownText,
                (Vector2) {
                textX - auto_x(7), textY - auto_y(3)
            },
                auto_x(20), auto_x(2), WHITE);
    }
    else {
        // Draw normal button
        DrawTextureEx(TEXTURE(resources, TEXTURE_LASER_BUTTON),
            (Vector2) {
            xPos, y
        }, 0, scale, WHITE);
    }
}


// Menggambar semua elemen UI dalam game
// Main UI rendering function untuk HUD complete
void drawGameUI(Game* game, GameResources* resources) {
    char obj[32];
    const int ICON_SIZE = auto_x(35);
    const int SPACING = auto_x(10);
    const int START_ICON_X = auto_x(345);

    float uiScale = (float)GetScreenHeight() / BG(resources, BG_UIGAME).height;
    float gamescale = (float)GetScreenHeight() / BG(resources, BG_GAMEAREA).height;
    float uiscaledWidth = BG(resources, BG_UIGAME).width * uiScale;
    float gamescaledWidth = BG(resources, BG_GAMEAREA).width * gamescale;

    // Mode
    formatGameText(obj, "%s", gameMode(resources));
    drawCenteredUIText(obj, auto_y(107), auto_x(18), gamescaledWidth, uiscaledWidth, WHITE);

    // High-Score with NEW tag if needed
    if (game->score > getCurrentModeHighScore(resources)) {
        const char* newTag = "[NEW]";
        Vector2 title = MeasureTextEx(GetFontDefault(), "HIGH-SCORE", 20, 2);
        Vector2 tag = MeasureTextEx(GetFontDefault(), newTag, 12, 2);
        Rectangle newBox = {
            auto_x(405) - auto_x(title.x / 2), auto_y(160),
            tag.x + auto_x(10), tag.y + auto_y(6)
        };
        DrawRectangle(newBox.x, newBox.y, newBox.width, newBox.height, ORANGE);
        DrawTextEx(GetFontDefault(), newTag, (Vector2) { newBox.x + 5, newBox.y + auto_y(3) }, auto_x(12), 2, WHITE);
    }

    formatGameText(obj, "%lld", getMaxScoreToShow(game, resources));
    drawCenteredUIText(obj, auto_y(215), auto_x(18), gamescaledWidth, uiscaledWidth, WHITE);

    // Score
    formatGameText(obj, "%lld", playerScore(game));
    drawCenteredUIText(obj, auto_y(305), auto_x(18), gamescaledWidth, uiscaledWidth, WHITE);

    // Power-up icons
    int iconIdx = 0;
    SLLNode* node = game->activePowerups.Front;
    while (node) {
        PowerUp* powerup = (PowerUp*)node->data;
        if (powerup && powerup->active) {
            PowerUpVisuals visuals = getPowerUpVisuals(resources, powerup->type);
            int iconX = START_ICON_X + (iconIdx * (ICON_SIZE + SPACING));

            drawPowerUpIcon(visuals.texture, (Vector2) { iconX, auto_y(380) },
                ICON_SIZE, powerup->duration, visuals.timerColor);
            iconIdx++;
        }
        node = node->next;
    }

    // Lives/Hearts
    drawLives(resources, game->lives, (Vector2) { START_ICON_X, auto_y(482) }, auto_x(35));

    // Laser Button
    drawLaserButton(resources, game->laserCooldown, gamescaledWidth, uiscaledWidth, auto_y(542));
}


// =============================================================================
// SCORE AND ACHIEVEMENT SYSTEM
// Fungsi-fungsi untuk mengelola skor dan pencapaian pemain
// =============================================================================

// Mendapatkan high score untuk mode yang sedang dimainkan
// Mengambil best score dari save data untuk mode current
ll getCurrentModeHighScore(GameResources* resources) {
    extern char* levelNames[];
    char* curMode = levelNames[resources->gameLevel];
    SLLNode* temp = resources->scores.head;
    while (temp != NULL) {
        HiScore* score = (HiScore*)SLL_getNodeData(temp);
        if (score && strcmp(score->mode, curMode) == 0) {
            return score->score;
        }
        temp = SLL_getNextNode(temp);
    }
    return 0;
}


// Mendapatkan skor maksimum yang akan ditampilkan
// Utility untuk display score dengan formatting yang tepat
ll getMaxScoreToShow(Game* game, GameResources* rsc) {
    return game->score > getCurrentModeHighScore(rsc) ? game->score : getCurrentModeHighScore(rsc);
}

// Tambahkan fungsi ini untuk inisialisasi posisi player yang responsif
void initializePlayerPosition(void) {
    // Hitung posisi tengah game area secara responsif
    float blockSize = auto_x(32);
    float gameWidth = blockSize * MAX_COLUMNS;

    // Posisi X: tengah game area dikurangi setengah lebar shooter
    P.x = auto_x(192);

    // Posisi Y: bagian bawah dengan offset yang responsif
    P.y = auto_y(598);

    // Pastikan posisi dalam bounds
    if (P.x < 0) P.x = 0;
    if (P.x + blockSize > gameWidth) P.x = gameWidth - blockSize;
}

// Fungsi untuk update posisi saat window resize
void updatePlayerPositionOnResize(void) {
    float blockSize = auto_x(32);
    float gameWidth = blockSize * MAX_COLUMNS;

    // Pertahankan posisi relatif terhadap game area
    P.x = ((int)(P.x / blockSize)) * blockSize;

    // Pertahankan posisi relatif terhadap game area
    if (P.x + blockSize > gameWidth) {
        P.x = gameWidth - blockSize;
    }

    // Update posisi Y jika diperlukan
    P.y = auto_y(598);
}