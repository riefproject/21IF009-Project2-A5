#include <stdio.h>
#include <raylib.h>
#include "Faliq.h"
#include "arief.h"
#include "all.h"
#define SIZE 32
#define SPEED 32

position P;
Music soundGameplay;
extern Sound sfxMove;
void initializeSounds() {
    sfxMove = LoadSound("assets/sounds/click.wav");
}

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
    soundGameplay = LoadMusicStream("assets/sound/gameplay.mp3"); // Ditambahkan oleh faliq
    PlayMusicStream(soundGameplay);  // Ditambahkan oleh faliq
    SetMusicVolume(soundGameplay, 0.5f); // Ditambahkan oleh faliq
}