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

    for (int i = MAX_ROWS - 1; i >= 0; i--) {
        if (gridX >= 0 && gridX < MAX_COLUMNS && game->grid[i][gridX]) {
            intersectionY = (i + 1) * 32;
            break;
        }
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

void openingAnimation(float* trans) {
    BeginDrawing();
    while (*trans < 1.0f) {  // Fade-in
        BeginDrawing();
        ClearBackground(fadeInOpeningAnimation(trans));
        if(*trans >= 1.0f){
            BeginDrawing();
            ClearBackground(WHITE);
            EndDrawing();
            WaitTime(2); 
        }
        EndDrawing();
    }
    ClearBackground(WHITE);
    EndDrawing();
}