#include <stdio.h>
#include <raylib.h>
#include "Faliq.h"
#include "all.h"
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define SIZE 32
#define SPEED 32

position P;

Music soundGameplay;
extern Sound sfxMove;
void initializeSounds() {
    sfxMove = LoadSound("assets/sounds/click.wav");
}

// Algoritma Shooter
void shooter(int* x, int* y) {
    int positionx = *x;
    int positiony = *y;

    if (*x + SIZE < GAME_SCREEN_WIDTH) {
        DrawRectangle(positionx, positiony, SIZE, SIZE, WHITE);
        DrawRectangle(positionx - SIZE, positiony, SIZE, SIZE, WHITE);
        DrawRectangle(positionx + SIZE, positiony, SIZE, SIZE, WHITE);
        DrawRectangle(positionx, positiony - SIZE, SIZE, SIZE, WHITE);
    }
    else {
        DrawRectangle(positionx, positiony, SIZE, SIZE, WHITE);
        DrawRectangle(positionx - SIZE, positiony, SIZE, SIZE, WHITE);
        DrawRectangle(positionx, positiony - SIZE, SIZE, SIZE, WHITE);
    }
}

void moveSet(int* x) {
    if ((IsKeyPressed(KEY_LEFT)) && (*x > 0)) {
        *x -= SPEED;
        PlaySound(sfxMove);
    }
    else if ((IsKeyPressed(KEY_RIGHT)) && (*x + SIZE < GAME_SCREEN_WIDTH)) {
        *x += SPEED;
        PlaySound(sfxMove);
    }
}

void musicGameplay() {
    soundGameplay = LoadMusicStream("assets/sounds/gameplay.mp3"); // Ditambahkan oleh faliq
    PlayMusicStream(soundGameplay);  // Ditambahkan oleh faliq
    SetMusicVolume(soundGameplay, 0.5f); // Ditambahkan oleh faliq
}

// Membuat animasi background
void openingAnimation(float *trans){
    if(*trans < 1.0f){
        *trans = *trans + 0.01f;
    }
    int colorValue = (int)(*trans * 255);
    Color bgColor = {colorValue, colorValue, colorValue, 255};
    BeginDrawing();
        ClearBackground(bgColor);  
    EndDrawing();
}