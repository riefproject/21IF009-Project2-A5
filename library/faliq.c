#include "defines.h"
#include "all.h"
#define BLOCK_SIZE auto_x(32)
#define SHOOTER_STEP auto_x(32)
#define GAME_WIDTH (BLOCK_SIZE * MAX_COLUMNS)
#define GAME_HEIGHT (BLOCK_SIZE * MAX_ROWS)
position P;
Music soundGameplay;


void shooter(int* x, int* y, GameResources* resources) {
    float blockSize = BLOCK_SIZE;
    float positionX = (float)*x;
    float positionY = (float)*y;

    float imgScale = blockSize / (float)TEXTURE(resources, TEXTURE_SHOOTER_M).width;

    DrawTextureEx(TEXTURE(resources, TEXTURE_SHOOTER_M), (Vector2) { positionX, positionY }, 0, imgScale, WHITE);
    if (positionX >= blockSize) {
        DrawTextureEx(TEXTURE(resources, TEXTURE_SHOOTER_L), (Vector2) { positionX - blockSize, positionY }, 0, imgScale, WHITE);
        DrawTextureEx(TEXTURE(resources, TEXTURE_SHOOTER_T), (Vector2) { positionX, positionY - blockSize }, 0, imgScale, WHITE);

        // Draw bagian kanan jika dalam batas area game
        if (positionX + blockSize < GAME_WIDTH) {
            DrawTextureEx(TEXTURE(resources, TEXTURE_SHOOTER_R), (Vector2) { positionX + blockSize, positionY }, 0, imgScale, WHITE);
        }
    }
}

void moveSet(int* x, GameResources* resources) {
    float blockSize = BLOCK_SIZE;
    float gameWidth = GAME_WIDTH;

    // Movement dengan boundary checking
    if (MOVE_LEFT && *x > 0) {
        *x -= (int)SHOOTER_STEP;
        PlaySound(SOUND(resources, SOUND_MOVE));
    }
    else if (MOVE_RIGHT && *x + blockSize < gameWidth) {
        *x += (int)SHOOTER_STEP;
        PlaySound(SOUND(resources, SOUND_MOVE));
    }

    *x = (*x < 0) ? 0 : *x;
    *x = (*x + (int)blockSize > (int)gameWidth) ? (int)(gameWidth - blockSize) : *x;
}

void musicGameplay(GameResources* resources) {
    soundGameplay = LoadMusicStream("assets/sounds/gameplay.mp3");
    PlayMusicStream(soundGameplay);
    SetMusicVolume(soundGameplay, resources->settings.music ? 0.5f : 0.0f);
}

void handleLaser(Game* game) {
    if (!game->laserActive) return;
    float blockSize = BLOCK_SIZE;

    // Sesuaikan posisi shooter dengan grid
    float shooterX = roundf(P.x / blockSize) * blockSize;
    int gridX = (int)(shooterX / blockSize);
    float intersectionY = P.y;

    DLLNode* currentRowNode = game->grid->tail;
    int currentRowIndex = MAX_ROWS - 1;

    while (currentRowNode != NULL) {
        if (gridX >= 0 && gridX < MAX_COLUMNS) {
            DoublyLinkedList* currentRow = (DoublyLinkedList*)currentRowNode->data;
            DLLNode* blockNode = currentRow->head;

            // Navigate to correct column
            for (int col = 0; col < gridX && blockNode != NULL; col++) {
                blockNode = blockNode->next;
            }

            if (blockNode != NULL) {
                Block* block = (Block*)blockNode->data;
                if (block->active) {
                    // Sesuaikan Y dengan grid
                    intersectionY = currentRowIndex * blockSize;
                    break;
                }
            }
        }
        currentRowNode = currentRowNode->prev;
        currentRowIndex--;
    }

    shooterX += blockSize / 2;

    float laserThickness = auto_x(2.0f);
    float dotRadius = auto_x(3.0f);
    intersectionY += blockSize;

    DrawLineEx((Vector2) { shooterX, P.y }, (Vector2) { shooterX, intersectionY }, laserThickness, (Color) { 255, 0, 0, 128 });
    DrawCircle(shooterX, intersectionY, dotRadius, RED);
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