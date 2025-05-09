#ifndef FALIQ_H
#define FALIQ_H
#include "defines.h"
// Menentukan lokasi shooter
typedef struct {
    int x;
    int y;
}position;

extern position P;

// Membuat Bentuk Shooter 
void shooter(int* x, int* y, GameResources* resources);

// Membuat Moveset Shooter
void moveSet(int* x, GameResources* resources);

// Mensetting lagu gameplay
extern Music soundGameplay;
void musicGameplay(GameResources* resources);

// Membuat laser
void handleLaser(Game* game);

// Membuat animasi opening game.
Color fadeInOpeningAnimation(float* trans);
void openingAnimation(float* trans, GameResources* resources);

#endif