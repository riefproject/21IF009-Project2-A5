#ifndef FALIQ_H
#define FALIQ_H

// Menentukan lokasi shooter
typedef struct {
    int x;
    int y;
}position;

extern position P;

// Membuat Bentuk Shooter 
void shooter(int* x, int* y);

// Membuat Moveset Shooter
void moveSet(int* x);

// Mensetting lagu gameplay
extern Music soundGameplay;
void musicGameplay(GameResources* resources);

// Membuat laser
void handleLaser(Game* game);


// Membuat animasi opening game.
typedef struct {
    float progress;
}openingTransition;
extern openingTransition opTrans;

Color fadeInOpeningAnimation(float* trans);
Color fadeOutOpeningAnimation(float* trans);
void openingAnimation(float* trans);

#endif