#include "arief.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <raylib.h>
#include <math.h>

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

void loadHiScores(HiScore scores[]) {
    FILE* file = fopen("db/hiscores.dat", "r");
    if (!file) {
        printf("File tidak ditemukan! Menggunakan default skor 0.\n");
        for (int i = 0; i < MAX_LEVELS; i++) {
            if (i < 10) {
                sprintf(scores[i].mode, "Level %d", i + 1);
            }
            else {
                strcpy(scores[i].mode, "Endless");
            }
            scores[i].score = 0;
        }
        return;
    }

    char mode[20];
    int score;
    int index = 0;

    while (fscanf(file, "%[^,],%d\n", mode, &score) != EOF && index < MAX_LEVELS) {
        strcpy(scores[index].mode, mode);
        scores[index].score = score;
        index++;
    }

    fclose(file);
}

void saveHiScores(HiScore scores[]) {
    FILE* file = fopen("db/hiscores.dat", "w");
    if (!file) {
        printf("Gagal menyimpan skor!\n");
        return;
    }

    fprintf(file, "Mode/Level,Score\n");
    for (int i = 0; i < MAX_LEVELS; i++) {
        fprintf(file, "%s,%d\n", scores[i].mode, scores[i].score);
    }

    fclose(file);
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

INIT_GAME_VARIABLES

void mainWindow() {
    Settings settings;
    loadSettings(&settings);
    HiScore scores[MAX_LEVELS];
    loadHiScores(scores);
    const int screenWidth = SCREEN_WIDTH;
    const int screenHeight = SCREEN_HEIGHT;
    float loadingTime = 0.0f;

    InitWindow(screenWidth, screenHeight, "Block Shooter");
    SetTargetFPS(60);

    InitAudioDevice();
    sfxMove = LoadSound("assets/sounds/click.wav");
    sfxSelect = LoadSound("assets/sounds/select.wav");
    SetSoundVolume(sfxMove, 1.0f);

    fontHeader = LoadFont("assets/fonts/Ubuntu-Medium.ttf");
    fontBody = LoadFont("assets/fonts/Ubuntu-Bold.ttf");
    blockTexture = LoadTexture("assets/sprites/block.png");

    currentState = STATE_LOADING;
    while (!WindowShouldClose()) {
        switch (currentState) {
        case STATE_LOADING:
            if (loadingScreen(&loadingTime)) {
                currentState = STATE_MAIN_MENU;
            }
            break;
        case STATE_MAIN_MENU:
            mainMenu();
            break;
        case STATE_HIGH_SCORES:
            showHiScore(scores);
            break;
        case STATE_CONTROLS:
            showControls();
            break;
        case STATE_SETTINGS:
            showSettings(&settings);
            break;
        case STATE_PLAY:
            displayGame();
            break;
        case STATE_QUIT:
            exitGame();
            break;
        case STATE_PAUSE:
            pauseMenu();
            break;
        case STATE_SELECT_LEVEL:
            selectLevel(&settings);
            break;
        }
    }

    UnloadTexture(blockTexture);
    UnloadSound(sfxMove);
    UnloadSound(sfxSelect);
    UnloadFont(fontHeader);
    UnloadFont(fontBody);
    CloseWindow();
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
        { {190,265}, {240,265}, {190,315}, {240,315} },
        // Step 2:
        { {140,265}, {290,265}, {190,315}, {240,315} },
        // Step 3:
        { {140,315}, {290,315}, {190,315}, {240,315} },
        // Step 4:
        { {140,315}, {290,315}, {190,265}, {240,265} },
        // Step 5:
        { {190,315}, {240,315}, {190,265}, {240,265} },
        // Step 6:
        { {190,315}, {240,315}, {140,265}, {290,265} },
        // Step 7:
        { {190,315}, {240,315}, {140,315}, {290,315} },
        // Step 8:
        { {190,265}, {240,265}, {140,315}, {290,315} },
        // Step 9:
        { {190,265}, {240,265}, {190,315}, {240,315} }
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
        DrawRectangleV(smoothPos, (Vector2) { blockSize, blockSize }, ORANGE);
    }
    EndDrawing();

    return 0;
}

void mainMenu() {
    prevState = STATE_MAIN_MENU;
    Font defaultFont = GetFontDefault();
    const char* lines[] = {
        "PLAY",
        "HIGH SCORE",
        "CONTROLS",
        "SETTINGS",
        "QUIT"
    };
    int fontSize = 30;
    int lineCount = sizeof(lines) / sizeof(lines[0]);

    int selection = 0;

    while (currentState == STATE_MAIN_MENU && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        int totalTextHeight = 0;
        for (int i = 0; i < lineCount; i++) {
            Vector2 textSize = MeasureTextEx(defaultFont, lines[i], fontSize, 2);
            totalTextHeight += textSize.y + 30;
        }

        int startY = (640 - totalTextHeight) / 2;
        for (int i = 0; i < lineCount; i++) {
            Vector2 textSize = MeasureTextEx(defaultFont, lines[i], fontSize, 2);
            int startX = (SCREEN_WIDTH - textSize.x) / 2;
            DrawTextEx(defaultFont, lines[i], (Vector2) { startX, startY }, fontSize, 2, selection == i ? BLUE : DARKGRAY);
            startY += textSize.y + 50;
        }

        if (MOVE_UP) {
            PlaySound(sfxMove);
            selection--;
            if (selection < 0) selection = lineCount - 1;
        }
        if (MOVE_DOWN) {
            PlaySound(sfxMove);
            selection++;
            if (selection >= lineCount) selection = 0;
        }

        if (OK_KEY) {
            PlaySound(sfxSelect);
            switch (selection) {
            case 0:
                currentState = STATE_SELECT_LEVEL;
                break;
            case 1:
                currentState = STATE_HIGH_SCORES;
                break;
            case 2:
                currentState = STATE_CONTROLS;
                break;
            case 3:
                currentState = STATE_SETTINGS;
                break;
            case 4:
                prevState = STATE_MAIN_MENU;
                currentState = STATE_QUIT;
                break;
            }
        }

        EndDrawing();
    }
}

void showControls() {
    int fontSize = 20;
    int spacing = 4;
    int startY = 200;

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

    int lineCount = sizeof(lines) / sizeof(lines[0]);

    while (currentState == STATE_CONTROLS && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        {
            Vector2 textSize = MeasureTextEx(fontHeader, "CONTROLS", 30, 0);
            int startX = (SCREEN_WIDTH - textSize.x) / 2;
            DrawTextEx(fontHeader, "CONTROLS", (Vector2) { startX, 100 }, 30, 0, DARKGRAY);
        }

        int maxLabelWidth = 0;
        int maxValueWidth = 0;
        for (int i = 0; i < lineCount; i++) {
            char label[50], value[50];
            sscanf(lines[i], "%[^:]:%[^\n]", label, value);
            int labelWidth = MeasureTextEx(fontBody, label, fontSize, spacing).x;
            int valueWidth = MeasureTextEx(fontBody, value, fontSize, spacing).x;
            if (labelWidth > maxLabelWidth) { maxLabelWidth = labelWidth; }
            if (valueWidth > maxValueWidth) { maxValueWidth = valueWidth; }
        }

        int padding = 20;
        int totalWidth = maxLabelWidth + 10 + MeasureTextEx(fontBody, ":", fontSize, spacing).x + 5 + maxValueWidth;
        int startX = (SCREEN_WIDTH - totalWidth) / 2;
        if (startX < padding) { startX = padding; }

        int y = startY;
        for (int i = 0; i < lineCount; i++) {
            char label[50], value[50];
            sscanf(lines[i], "%[^:]:%[^\n]", label, value);
            int colonX = startX + maxLabelWidth + 10;
            int textX = colonX + MeasureTextEx(fontBody, ":", fontSize, spacing).x + 5;
            DrawTextEx(fontBody, label, (Vector2) { startX, y }, fontSize, spacing, DARKGRAY);
            DrawTextEx(fontBody, ":", (Vector2) { colonX, y }, fontSize, spacing, DARKGRAY);
            DrawTextEx(fontBody, value, (Vector2) { textX, y }, fontSize, spacing, DARKGRAY);
            y += fontSize + spacing;
        }
        {
            const char* infoText;
            // Jika game dijeda (prevState == STATE_SELECT_LEVEL) tampilkan tiga shortcut
            if (prevState == STATE_PLAY)
                infoText = "[R]: Resume    [P]: Pause Menu    [F]: ke Settings";
            else
                infoText = "[A]: Main Menu    [F]: Settings";
            Vector2 textSize = MeasureTextEx(fontBody, infoText, (prevState == STATE_PLAY) ? 15 : 20, 2.0f);
            int startXInfo = (SCREEN_WIDTH - textSize.x) / 2;
            DrawTextEx(fontBody, infoText, (Vector2) { startXInfo, 560 }, (prevState == STATE_PLAY) ? 15 : 20, 2.0f, DARKGRAY);
        }
        EndDrawing();

        if (prevState != STATE_PLAY) {
            if (IsKeyPressed(KEY_A)) {
                PlaySound(sfxMove);
                currentState = STATE_MAIN_MENU;
            }
        }

        if (prevState == STATE_PLAY) {
            if (IsKeyPressed(KEY_R)) {
                PlaySound(sfxMove);
                currentState = STATE_PLAY;
                countdownPause();
                break;
            }
            if (IsKeyPressed(KEY_P)) {
                PlaySound(sfxMove);
                currentState = STATE_PAUSE;
                break;
            }
        }

        if (FORWARD_KEY) {
            PlaySound(sfxMove);
            currentState = STATE_SETTINGS;
        }
    }
}

void showSettings(Settings* settings) {
    int fontSize = 20;
    int spacing = 4;
    int startY = 200;

    char lines[4][100];
    snprintf(lines[0], sizeof(lines[0]), "Music : %s", settings->music ? "On" : "Off");
    snprintf(lines[1], sizeof(lines[1]), "Sfx   : %s", settings->sfx ? "On" : "Off");
    snprintf(lines[2], sizeof(lines[2]), "Controls");
    snprintf(lines[3], sizeof(lines[3]), "Reset");

    int lineCount = sizeof(lines) / sizeof(lines[0]);
    int selection = 0;
    bool editing = false;

    while (currentState == STATE_SETTINGS && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        {
            Vector2 textSize = MeasureTextEx(fontHeader, "SETTINGS", 30, 0);
            int startX = (SCREEN_WIDTH - textSize.x) / 2;
            DrawTextEx(fontHeader, "SETTINGS", (Vector2) { startX, 100 }, 30, 0, DARKGRAY);
        }

        int maxLabelWidth = 0;
        int maxValueWidth = 0;
        for (int i = 0; i < 2; i++) {
            char label[50], value[50];
            sscanf(lines[i], "%[^:]:%[^\n]", label, value);
            int labelWidth = MeasureTextEx(fontBody, label, fontSize, spacing).x;
            int valueWidth = MeasureTextEx(fontBody, value, fontSize, spacing).x;
            if (labelWidth > maxLabelWidth) { maxLabelWidth = labelWidth; }
            if (valueWidth > maxValueWidth) { maxValueWidth = valueWidth; }
        }

        int padding = 20;
        int totalWidth = maxLabelWidth + 10 + MeasureTextEx(fontBody, ":", fontSize, spacing).x + 5 + maxValueWidth;
        int startX = (SCREEN_WIDTH - totalWidth) / 2;
        if (startX < padding) { startX = padding; }

        int y = startY;
        for (int i = 0; i < 2; i++) {
            char label[50], value[50];
            sscanf(lines[i], "%[^:]:%[^\n]", label, value);
            int colonX = startX + maxLabelWidth + 10;
            int textX = colonX + MeasureTextEx(fontBody, ":", fontSize, spacing).x + 5;
            DrawTextEx(fontBody, label, (Vector2) { startX, y }, fontSize, spacing, selection == i ? BLUE : DARKGRAY);
            DrawTextEx(fontBody, ":", (Vector2) { colonX, y }, fontSize, spacing, DARKGRAY);
            DrawTextEx(fontBody, value, (Vector2) { textX, y }, fontSize, spacing, DARKGRAY);
            y += fontSize + spacing;
        }
        y += 50;
        for (int i = 2; i < 4; ++i) {
            const char* optionText = lines[i];
            Vector2 textSize = MeasureTextEx(fontBody, optionText, 20, 2.0f);
            int startXOption = (SCREEN_WIDTH - textSize.x) / 2;
            DrawTextEx(fontBody, optionText, (Vector2) { startXOption, y }, 20, 2.0f, selection == i ? BLUE : DARKGRAY);
            y += 50;
        }
        {
            const char* infoText;
            if (prevState == STATE_PLAY)
                infoText = "[R]: Resume    [P]: Pause Menu    [F]: ke Controls";
            else
                infoText = "[A]: Main Menu    [F]: ke Controls";
            Vector2 textSize = MeasureTextEx(fontBody, infoText, (prevState == STATE_PLAY) ? 15 : 20, 2.0f);
            int startXInfo = (SCREEN_WIDTH - textSize.x) / 2;
            DrawTextEx(fontBody, infoText, (Vector2) { startXInfo, 560 }, (prevState == STATE_PLAY) ? 15 : 20, 2.0f, DARKGRAY);
        }
        EndDrawing();

        if (!editing) {
            if (MOVE_UP) {
                PlaySound(sfxMove);
                selection--;
                if (selection < 0) selection = lineCount - 1;
            }
            if (MOVE_DOWN) {
                PlaySound(sfxMove);
                selection++;
                if (selection >= lineCount) selection = 0;
            }
            if (OK_KEY) {
                PlaySound(sfxSelect);
                if (selection < 2) {
                    editing = true;
                }
                else {
                    switch (selection) {
                    case 2:
                        currentState = STATE_CONTROLS;
                        break;
                    case 3:
                        // Reset logic 
                        break;
                    }
                }
            }
        }
        else {
            if (MOVE_LEFT || MOVE_RIGHT) {
                PlaySound(sfxMove);
                char label[50], value[50];
                sscanf(lines[selection], "%[^:]:%[^\n]", label, value);
                if (strcmp(value, " On") == 0) {
                    snprintf(lines[selection], sizeof(lines[selection]), "%s: Off", label);
                    if (selection == 0) settings->music = 0;
                    if (selection == 1) settings->sfx = 0;
                }
                else {
                    snprintf(lines[selection], sizeof(lines[selection]), "%s: On", label);
                    if (selection == 0) settings->music = 1;
                    if (selection == 1) settings->sfx = 1;
                }
                saveSettings(*settings);
            }
            if (OK_KEY) {
                editing = false;
            }
        }

        if (prevState == STATE_PLAY) {
            if (IsKeyPressed(KEY_R)) {
                PlaySound(sfxMove);
                currentState = STATE_PLAY;
                countdownPause();
                break;
            }
            if (IsKeyPressed(KEY_P)) {
                PlaySound(sfxMove);
                currentState = STATE_PAUSE;
                break;
            }
        }
        else {
            if (MOVE_LEFT || BACK_KEY) {
                PlaySound(sfxMove);
                currentState = STATE_MAIN_MENU;
            }
        }
        if (FORWARD_KEY) {
            PlaySound(sfxMove);
            currentState = STATE_CONTROLS;
        }
    }
}

void showHiScore(HiScore hiscores[]) {
    int fontSize = 20;
    int spacing = 4;
    int startY = 200;

    char lines[MAX_LEVELS][65];
    for (int i = 0; i < MAX_LEVELS; i++) {
        snprintf(lines[i], sizeof(lines[i]), "%s: %d", hiscores[i].mode, hiscores[i].score);
    }

    int lineCount = sizeof(lines) / sizeof(lines[0]);

    while (currentState == STATE_HIGH_SCORES && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        {
            Vector2 textSize = MeasureTextEx(fontHeader, "HIGH SCORE", 30, 0);
            int startX = (SCREEN_WIDTH - textSize.x) / 2;
            DrawTextEx(fontHeader, "HIGH SCORE", (Vector2) { startX, 100 }, 30, 0, DARKGRAY);
        }

        int maxLabelWidth = 0;
        int maxValueWidth = 0;
        for (int i = 0; i < lineCount; i++) {
            char label[50], value[50];
            sscanf(lines[i], "%[^:]:%[^\n]", label, value);

            int labelWidth = MeasureTextEx(fontBody, label, fontSize, spacing).x;
            int valueWidth = MeasureTextEx(fontBody, value, fontSize, spacing).x;
            if (labelWidth > maxLabelWidth) {
                maxLabelWidth = labelWidth;
            }
            if (valueWidth > maxValueWidth) {
                maxValueWidth = valueWidth;
            }
        }

        int padding = 20;
        int totalWidth = maxLabelWidth + 10 + MeasureTextEx(fontBody, ":", fontSize, spacing).x + 5 + maxValueWidth;
        int startX = (SCREEN_WIDTH - totalWidth) / 2;

        if (startX < padding) {
            startX = padding;
        }

        int y = startY;
        for (int i = 0; i < lineCount; i++) {
            char label[50], value[50];
            sscanf(lines[i], "%[^:]:%[^\n]", label, value);

            int colonX = startX + maxLabelWidth + 10;
            int textX = colonX + MeasureTextEx(fontBody, ":", fontSize, spacing).x + 5;

            DrawTextEx(fontBody, label, (Vector2) { startX, y }, fontSize, spacing, DARKGRAY);
            DrawTextEx(fontBody, ":", (Vector2) { colonX, y }, fontSize, spacing, DARKGRAY);
            DrawTextEx(fontBody, value, (Vector2) { textX, y }, fontSize, spacing, DARKGRAY);

            y += fontSize + spacing;
        }
        {
            const char* controlsText = "[A]: Main Menu";
            Vector2 textSize = MeasureTextEx(fontBody, controlsText, 20, 2.0f);
            int startX = (SCREEN_WIDTH - textSize.x) / 2;
            DrawTextEx(fontBody, controlsText, (Vector2) { startX, 560 }, 20, 2.0f, DARKGRAY);
        }

        if (MOVE_LEFT || BACK_KEY) {
            PlaySound(sfxMove);
            currentState = STATE_MAIN_MENU;
        }

        EndDrawing();
    }
}

bool confirmExit() {
    const char* message = "Are you sure you want to exit?";
    const char* options[] = { "Yes", "No" };
    int selection = 1;
    int fontSize = 20;
    int lineCount = sizeof(options) / sizeof(options[0]);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        Vector2 textSize = MeasureTextEx(fontHeader, message, fontSize, 2);
        int startX = (SCREEN_WIDTH - textSize.x) / 2;
        int startY = (640 - textSize.y) / 2 - 50;
        DrawTextEx(fontHeader, message, (Vector2) { startX, startY }, fontSize, 2, DARKGRAY);

        startY += textSize.y + 50;
        for (int i = 0; i < lineCount; i++) {
            textSize = MeasureTextEx(fontHeader, options[i], fontSize, 2);
            startX = (SCREEN_WIDTH - textSize.x) / 2;
            DrawTextEx(fontBody, options[i], (Vector2) { startX, startY }, fontSize, 2, selection == i ? BLUE : DARKGRAY);
            startY += textSize.y + 20;
        }
        EndDrawing();
        if (MOVE_UP) {
            PlaySound(sfxMove);
            selection--;
            if (selection < 0) selection = lineCount - 1;
        }
        if (MOVE_DOWN) {
            PlaySound(sfxMove);
            selection++;
            if (selection >= lineCount) selection = 0;
        }
        if (OK_KEY) {
            PlaySound(sfxSelect);
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
            PlaySound(sfxMove);
            EndDrawing();
            return false;
        }


    }

    return false;
}

void exitGame() {
    if (confirmExit()) {
        UnloadFont(fontBody);
        UnloadFont(fontHeader);
        UnloadSound(sfxMove);
        UnloadSound(sfxSelect);
        CloseWindow();
        exit(0); // Exit program
    }
    else {
        currentState = prevState;
    }
}

void displayGame() {
    prevState = STATE_PLAY;
    BeginDrawing();
    ClearBackground(DARKGRAY);

    DrawRectangle(GAME_SCREEN, Fade(SKYBLUE, 0.3f));

    for (int i = 0; i < 10; ++i) {
        Vector2 position = { GetRandomValue(0, 9) * 32, GetRandomValue(0, 15) * 32 };
        DrawTexture(blockTexture, position.x, position.y, WHITE);
    }

    DrawRectangle(0, 512, 320, 1, BLACK);
    DrawText("Hi Score", 340, 20, 15, WHITE);
    DrawText("12345", 340, 40, 20, LIGHTGRAY);

    DrawText("Dalam Tahap Pengembangan!", 100, 300, 20, LIGHTGRAY);
    EndDrawing();

    if (IsKeyPressed(KEY_P)) {
        while (IsKeyDown(KEY_P) && !WindowShouldClose()) {
            BeginDrawing();
            EndDrawing();
        }
        pauseMenu(GetFontDefault());
    }
}

bool confirmBack() {
    const char* message1 = "Are you sure you want to go back";
    const char* message2 = "to the main menu?";
    const char* options[] = { "Yes", "No" };
    int selection = 1;
    int fontSize = 20;
    int lineCount = sizeof(options) / sizeof(options[0]);
    bool deciding = true;

    while (deciding && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        Vector2 textSize = MeasureTextEx(fontHeader, message1, fontSize, 2);
        int startX = (SCREEN_WIDTH - textSize.x) / 2;
        int startY = (SCREEN_HEIGHT - textSize.y) / 2 - 50;
        DrawTextEx(fontHeader, message1, (Vector2) { startX, startY }, fontSize, 2, DARKGRAY);

        textSize = MeasureTextEx(fontHeader, message2, fontSize, 2);
        startX = (SCREEN_WIDTH - textSize.x) / 2;
        startY += textSize.y + 20;
        DrawTextEx(fontHeader, message2, (Vector2) { startX, startY }, fontSize, 2, DARKGRAY);

        startY += textSize.y + 50;
        for (int i = 0; i < lineCount; i++) {
            textSize = MeasureTextEx(fontBody, options[i], fontSize, 2);
            startX = (SCREEN_WIDTH - textSize.x) / 2;
            DrawTextEx(fontBody, options[i], (Vector2) { startX, startY }, fontSize, 2, selection == i ? BLUE : DARKGRAY);
            startY += textSize.y + 20;
        }
        EndDrawing();

        if (MOVE_UP || MOVE_LEFT) {
            PlaySound(sfxMove);
            selection--;
            if (selection < 0) selection = lineCount - 1;
        }
        if (MOVE_DOWN || MOVE_RIGHT) {
            PlaySound(sfxMove);
            selection++;
            if (selection >= lineCount) selection = 0;
        }
        if (OK_KEY) {
            PlaySound(sfxSelect);
            deciding = false;
            return selection == 0; // Return true jika "Yes", false jika "No"
        }
        if (IsKeyPressed(KEY_N) || BACK_KEY) {
            PlaySound(sfxMove);
            return false;
        }
    }
    return false;
}

void pauseMenu() {
    prevState = STATE_PAUSE;
    bool paused = true;
    const char* lines[] = {
        "RESUME",
        "CONTROLS",
        "SETTINGS",
        "MAIN MENU",
        "QUIT"
    };
    int fontSize = 20;
    int lineCount = sizeof(lines) / sizeof(lines[0]);
    int selection = 0;

    while (paused && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Fade(RAYWHITE, 0.9f));
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(RAYWHITE, 0.8f));
        {
            const char* pausedText = "PAUSED";
            Vector2 textSize = MeasureTextEx(fontHeader, pausedText, 40, 2);
            int startX = (SCREEN_WIDTH - textSize.x) / 2;
            DrawTextEx(fontHeader, pausedText, (Vector2) { startX, 140 }, 40, 2, DARKGRAY);
        }

        int totalHeight = 0;
        for (int i = 0; i < lineCount; i++) {
            Vector2 textSize = MeasureTextEx(fontBody, lines[i], fontSize, 2);
            totalHeight += textSize.y + 25;
        }
        int startY = (SCREEN_HEIGHT - totalHeight) / 2 + 20;
        for (int i = 0; i < lineCount; i++) {
            Vector2 textSize = MeasureTextEx(fontBody, lines[i], fontSize, 2);
            int startX = (SCREEN_WIDTH - textSize.x) / 2;
            if (selection == i) {
                DrawRectangle(startX - 10, startY - 5, textSize.x + 20, textSize.y + 10, Fade(LIGHTGRAY, 0.3f));
            }
            DrawTextEx(fontBody, lines[i], (Vector2) { startX, startY }, fontSize, 2, selection == i ? BLUE : DARKGRAY);
            startY += textSize.y + 25;
        }
        EndDrawing();

        if (MOVE_UP) {
            PlaySound(sfxMove);
            selection--;
            if (selection < 0) selection = lineCount - 1;
        }
        if (MOVE_DOWN) {
            PlaySound(sfxMove);
            selection++;
            if (selection >= lineCount) selection = 0;
        }
        if (OK_KEY) {
            PlaySound(sfxSelect);
            switch (selection) {
            case 0: // RESUME
                currentState = STATE_PLAY;
                paused = false;
                countdownPause();
                break;
            case 1: // CONTROLS
                currentState = STATE_CONTROLS;
                paused = false;
                break;
            case 2: // SETTINGS
                currentState = STATE_SETTINGS;
                paused = false;
                break;
            case 3: // MAIN MENU
                if (confirmBack()) {
                    currentState = STATE_MAIN_MENU;
                    paused = false;
                }
                break;
            case 4: // QUIT
                prevState = STATE_PAUSE;
                currentState = STATE_QUIT;
                paused = false;
                break;
            }
        }
        if (IsKeyPressed(KEY_P)) {
            PlaySound(sfxMove);
            currentState = STATE_PLAY;
            paused = false;
            countdownPause();
        }
    }
}
void countdownPause() {
    if (currentState == STATE_PLAY) {
        float counter = 3.0f;
        while (counter > 0.0f && !WindowShouldClose()) {
            counter -= GetFrameTime();
            BeginDrawing();
            ClearBackground(RAYWHITE);
            char text[8];
            sprintf(text, "%d", (int)ceilf(counter));
            DrawTextEx(GetFontDefault(), text, (Vector2) { 230, 300 }, 50, 2, RED);
            EndDrawing();
        }
    }
}

void selectLevel(Settings* settings) {
    prevState = STATE_SELECT_LEVEL;
    const char* title = "SELECT LEVEL";
    const char* modes[] = {
        "Level 1", "Level 2", "Level 3", "Level 4", "Level 5",
        "Level 6", "Level 7", "Level 8", "Level 9", "Level 10",
        "Endless Mode"
    };
    int fontSize = 20;
    int selection = settings->mode;
    int lineCount = sizeof(modes) / sizeof(modes[0]);
    bool selecting = true;

    while (selecting && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        {
            Vector2 textSize = MeasureTextEx(fontHeader, title, 30, 2);
            int startX = (SCREEN_WIDTH - textSize.x) / 2;
            DrawTextEx(fontHeader, title, (Vector2) { startX, 100 }, 30, 2, DARKGRAY);
        }

        int totalHeight = 0;
        for (int i = 0; i < lineCount; i++) {
            Vector2 textSize = MeasureTextEx(fontBody, modes[i], fontSize, 2);
            totalHeight += textSize.y + 15;
        }

        int startY = (SCREEN_HEIGHT - totalHeight) / 2 + 20;
        for (int i = 0; i < lineCount; i++) {
            Vector2 textSize = MeasureTextEx(fontBody, modes[i], fontSize, 2);
            int startX = (SCREEN_WIDTH - textSize.x) / 2;
            Color textColor = (selection == i) ? BLUE : DARKGRAY;

            if (selection == i) {
                DrawRectangle(startX - 10, startY - 5, textSize.x + 20, textSize.y + 10,
                    Fade(LIGHTGRAY, 0.3f));
            }

            DrawTextEx(fontBody, modes[i], (Vector2) { startX, startY }, fontSize, 2, textColor);
            startY += textSize.y + 15;
        }

        {
            const char* controlsText = "[↑/↓]: Select    [Enter]: Play    [B]: Back";
            Vector2 textSize = MeasureTextEx(fontBody, controlsText, 15, 2);
            int startX = (SCREEN_WIDTH - textSize.x) / 2;
            DrawTextEx(fontBody, controlsText, (Vector2) { startX, 560 }, 15, 2, DARKGRAY);
        }

        EndDrawing();

        if (MOVE_UP) {
            PlaySound(sfxMove);
            selection--;
            if (selection < 0) selection = lineCount - 1;
        }
        if (MOVE_DOWN) {
            PlaySound(sfxMove);
            selection++;
            if (selection >= lineCount) selection = 0;
        }
        if (OK_KEY) {
            PlaySound(sfxSelect);
            currentState = STATE_PLAY;
            selecting = false;
            settings->mode = selection;
            saveSettings(*settings);
            gameLevel = selection;
            countdownPause();
        }
        if (BACK_KEY) {
            PlaySound(sfxMove);
            currentState = STATE_MAIN_MENU;
            selecting = false;
        }
    }
}
