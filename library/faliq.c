#include "defines.h"
#include "all.h"
#define BLOCK_SIZE auto_x(32)
#define SHOOTER_STEP 32
// 
position P;
Music soundGameplay;


void shooter(int* x, int* y, GameResources* resources, ScaleFactor scale) {
    int positionx = *x;
    int positiony = *y;

    if (*x + BLOCK_SIZE < GAME_SCREEN_WIDTH) {
        DrawTexture(TEXTURE(resources, TEXTURE_SHOOTER_M), positionx, positiony, WHITE);
        DrawTexture(TEXTURE(resources, TEXTURE_SHOOTER_L), positionx - BLOCK_SIZE, positiony, WHITE);
        DrawTexture(TEXTURE(resources, TEXTURE_SHOOTER_R), positionx + BLOCK_SIZE, positiony, WHITE);
        DrawTexture(TEXTURE(resources, TEXTURE_SHOOTER_T), positionx, positiony - BLOCK_SIZE, WHITE);
    }
    else {
        DrawTexture(TEXTURE(resources, TEXTURE_SHOOTER_M), positionx, positiony, WHITE);
        DrawTexture(TEXTURE(resources, TEXTURE_SHOOTER_L), positionx - BLOCK_SIZE, positiony, WHITE);
        DrawTexture(TEXTURE(resources, TEXTURE_SHOOTER_T), positionx, positiony - BLOCK_SIZE, WHITE);
    }
}

void moveSet(int* x, GameResources* resources, ScaleFactor scale) {
    if ((MOVE_LEFT) && (*x > 0)) {
        *x -= SHOOTER_STEP;
        PlaySound(SOUND(resources, SOUND_MOVE));
    }
    else if ((MOVE_RIGHT) && (*x + BLOCK_SIZE < GAME_SCREEN_WIDTH)) {
        *x += SHOOTER_STEP;
        PlaySound(SOUND(resources, SOUND_MOVE));
    }
}

void musicGameplay(GameResources* resources) {
    soundGameplay = LoadMusicStream("assets/sounds/gameplay.mp3");
    PlayMusicStream(soundGameplay);
    SetMusicVolume(soundGameplay, resources->settings.music ? 0.5f : 0.0f);
}

void handleLaser(Game* game) {
    if (!game->laserActive) return;

    int shooterX = P.x;
    int gridX = shooterX / 32;
    float intersectionY = P.y;

    DLLNode* currentRowNode = game->grid->tail;
    int currentRowIndex = MAX_ROWS - 1;

    // Iterasi dari baris terakhir ke baris pertama
    while (currentRowNode != NULL) {
        if (gridX >= 0 && gridX < MAX_COLUMNS) {
            DoublyLinkedList* currentRow = (DoublyLinkedList*)currentRowNode->data;
            DLLNode* blockNode = currentRow->head;

            // Navigasi ke kolom `gridX`
            for (int col = 0; col < gridX && blockNode != NULL; col++) {
                blockNode = blockNode->next;
            }

            if (blockNode != NULL) {
                Block* block = (Block*)blockNode->data;
                if (block->active) {
                    intersectionY = (currentRowIndex + 1) * 32;
                    break;
                }
            }
        }

        currentRowNode = currentRowNode->prev;
        currentRowIndex--;
    }

    shooterX += 16;
    DrawLineEx(
        (Vector2) {
        shooterX, P.y
    },
        (Vector2) {
        shooterX, intersectionY
    },
        2.0f,
        (Color) {
        255, 0, 0, 128
    }
    );

    DrawCircle(shooterX, intersectionY, 3, RED);
}

// Membuat animasi background
Color fadeInOpeningAnimation(float* trans) {
    if (*trans < 1.0f) {
        *trans = *trans + 0.005f;
    }
    int colorValue = (int)(*trans * 255);
    return (Color) { colorValue, colorValue, colorValue, 255 };
}

Color fadeOutOpeningAnimation(float* trans) {
    if (*trans > 0.0f) {
        *trans = *trans - 0.005f;
        if (*trans < 0) {
            *trans = 0.0f;
        }
    }
    int colorValue = (int)(*trans * 255);
    return (Color) { colorValue, colorValue, colorValue, 255 };
}

void openingAnimation(float* trans, GameResources* resources) {
    BeginDrawing();

    Texture2D icon = TEXTURE(resources, TEXTURE_WHITE_ICON);

    float targetWidth = 400;
    float scaleFactor = targetWidth / icon.width;
    float scaledWidth = icon.width * scaleFactor;
    float scaledHeight = icon.height * scaleFactor;

    int iconX = (GetScreenWidth() - scaledWidth) / 2;
    int iconY = (GetScreenHeight() - scaledHeight) / 2;

    while (*trans < 1.0f) {
        BeginDrawing();
        ClearBackground(fadeInOpeningAnimation(trans));
        DrawTextureEx(icon, (Vector2) { iconX, iconY }, 0.0f, scaleFactor, (Color) { 255, 255, 255, (unsigned char)(*trans * 255) });
        EndDrawing();
    }

    BeginDrawing();
    ClearBackground(WHITE);
    DrawTextureEx(icon, (Vector2) { iconX, iconY }, 0.0f, scaleFactor, WHITE);
    EndDrawing();
    WaitTime(1);

    while (*trans > 0.0f) {
        BeginDrawing();
        ClearBackground(fadeOutOpeningAnimation(trans));
        DrawTextureEx(icon, (Vector2) { iconX, iconY }, 0.0f, scaleFactor, (Color) { 255, 255, 255, (unsigned char)(*trans * 255) });
        EndDrawing();
    }
}