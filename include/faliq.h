#ifndef FALIQ_H
#define FALIQ_H

// Menentukan lokasi shooter
typedef struct
{
    int x;
    int y;
}position;

extern position P;

// Membuat Bentuk Shooter 
void shooter(int *x , int *y);

// Membuat Moveset Shooter
void moveSet(int *x);

// Mensetting lagu gameplay
extern Music soundGameplay;

#endif