#ifndef RAFFI_H
#define RAFFI_H
#include "arief.h"


//Inisialisasi nilai database
void initializeDb(HiScore scores[]);

//
void playerInitialization(Game *game);

//Menambahkan score player
void playerScore(Game *game, HiScore scores[]);


//Mencari nilai terbesar antara skor player dan HiScore
int getMax(HiScore scores[], Game game);


#endif
