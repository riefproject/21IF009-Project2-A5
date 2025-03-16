#ifndef FALIQ_H
#define FALIQ_H
#define ROW 10
#define COLS 20

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
void musicGameplay();

// Membuat animasi opening game 

typedef struct {
    float progress;
}openingTransition;
extern openingTransition opTrans;

void openingAnimation(float *progress);

#endif