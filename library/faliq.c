#include "defines.h"
#include "all.h"
#define BLOCK_SIZE auto_x(32)
#define SHOOTER_STEP auto_x(32)
#define GAME_WIDTH (BLOCK_SIZE * MAX_COLUMNS)
#define GAME_HEIGHT (BLOCK_SIZE * MAX_ROWS)
// base texture skin. skin dijamin >= 0. dikalikan 4 karena terdapat 4 texture untuk setiap skin 
#define SHOOTER_LEFT(skin) (*(Texture2D*)(getAsset((rsc)->assets->Shooter.head, (SHOOTER_SKIN_1_L + (skin*4)))));
#define SHOOTER_RIGHT(skin) (*(Texture2D*)(getAsset((rsc)->assets->Shooter.head, (SHOOTER_SKIN_1_R + (skin*4)))));
#define SHOOTER_MID(skin) (*(Texture2D*)(getAsset((rsc)->assets->Shooter.head, (SHOOTER_SKIN_1_M + (skin*4)))));
#define SHOOTER_TOP(skin) (*(Texture2D*)(getAsset((rsc)->assets->Shooter.head, (SHOOTER_SKIN_1_T + (skin*4)))));
position P;
Music soundGameplay;

SingleLinkedList* shooterAssets() {
    SingleLinkedList* list = new(SingleLinkedList);
    if (list != NULL) {
        // SKIN 1
        SLL_insertFront(list, inputAssets(TYPE_TEXTURE, SHOOTER_SKIN_1_L, "assets/sprites/shooter1l.png"));
        SLL_insertFront(list, inputAssets(TYPE_TEXTURE, SHOOTER_SKIN_1_M, "assets/sprites/shooter1m.png"));
        SLL_insertFront(list, inputAssets(TYPE_TEXTURE, SHOOTER_SKIN_1_R, "assets/sprites/shooter1r.png"));
        SLL_insertFront(list, inputAssets(TYPE_TEXTURE, SHOOTER_SKIN_1_T, "assets/sprites/shooter1t.png"));
        // SKIN 2
        SLL_insertFront(list, inputAssets(TYPE_TEXTURE, SHOOTER_SKIN_2_L, "assets/sprites/shooter2l.png"));
        SLL_insertFront(list, inputAssets(TYPE_TEXTURE, SHOOTER_SKIN_2_M, "assets/sprites/shooter2m.png"));
        SLL_insertFront(list, inputAssets(TYPE_TEXTURE, SHOOTER_SKIN_2_R, "assets/sprites/shooter2r.png"));
        SLL_insertFront(list, inputAssets(TYPE_TEXTURE, SHOOTER_SKIN_2_T, "assets/sprites/shooter2t.png"));
    }
    return list;
}

Texture2D SLL_Shooter_top(GameResources* rsc) {
    return SHOOTER_TOP(rsc->settings.skin)
}
Texture2D SLL_Shooter_mid(GameResources* rsc) {
    return SHOOTER_MID(rsc->settings.skin)
}
Texture2D SLL_Shooter_left(GameResources* rsc) {
    return SHOOTER_LEFT(rsc->settings.skin)
}
Texture2D SLL_Shooter_right(GameResources* rsc) {
    return SHOOTER_RIGHT(rsc->settings.skin)
}

void shooter(int* x, int* y, GameResources* resources) {
    float blockSize = BLOCK_SIZE;
    float positionX = (float)*x;
    float positionY = (float)*y;
    Texture2D shooterM = SLL_Shooter_mid(resources);
    Texture2D shooterT = SLL_Shooter_top(resources);
    Texture2D shooterR = SLL_Shooter_right(resources);
    Texture2D shooterL = SLL_Shooter_left(resources);

    float imgScale = blockSize / (float)shooterM.width;

    if (positionX + blockSize >= GAME_WIDTH) {
        DrawTextureEx(shooterM, (Vector2) { positionX, positionY }, 0, imgScale, WHITE);
        DrawTextureEx(shooterT, (Vector2) { positionX, positionY - blockSize }, 0, imgScale, WHITE);
        DrawTextureEx(shooterL, (Vector2) { positionX - blockSize, positionY }, 0, imgScale, WHITE);
    }
    else {
        DrawTextureEx(shooterL, (Vector2) { positionX - blockSize, positionY }, 0, imgScale, WHITE);
        DrawTextureEx(shooterM, (Vector2) { positionX, positionY }, 0, imgScale, WHITE);
        DrawTextureEx(shooterT, (Vector2) { positionX, positionY - blockSize }, 0, imgScale, WHITE);
        DrawTextureEx(shooterR, (Vector2) { positionX + blockSize, positionY }, 0, imgScale, WHITE);
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

    Texture2D icon = BG(resources, ICON_LOADING);

    float imgScale = (float)GetScreenHeight() / icon.height;
    float scaledWidth = icon.width * imgScale;
    int iconX = (GetScreenWidth() - scaledWidth) / 2;

    while (*trans < 1.0f) {
        BeginDrawing();
        ClearBackground(fadeInOpeningAnimation(trans));
        DrawTextureEx(icon, (Vector2) { iconX, 0 }, 0.0f, imgScale, (Color) { 255, 255, 255, (unsigned char)(*trans * 255) });
        EndDrawing();
    }

    BeginDrawing();
    ClearBackground(WHITE);
    DrawTextureEx(icon, (Vector2) { iconX, 0 }, 0.0f, imgScale, WHITE);
    EndDrawing();
    WaitTime(1);

    while (*trans > 0.0f) {
        BeginDrawing();
        ClearBackground(fadeOutOpeningAnimation(trans));
        DrawTextureEx(icon, (Vector2) { iconX, 0 }, 0.0f, imgScale, (Color) { 255, 255, 255, (unsigned char)(*trans * 255) });
        EndDrawing();
    }
}