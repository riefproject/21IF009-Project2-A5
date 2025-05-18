#ifndef RAFFI_H
#define RAFFI_H
#include "defines.h"

//Inisialisasi Db
void initializeDb(HiScore scores[]);

<<<<<<< HEAD
//Inisialisasi nilai database
void initializeDb(HiScore scores[]);

//Mencari nilai terbesar antara skor player dan HiScores
int getMax(HiScore scores[], Game* game, GameResources* resources);

=======
//Mencari nilai terbesar antara skor player dan HiScores
long long int getMax(HiScore scores[], Game* game, GameResources* resources);

>>>>>>> cbf3f8c5a6734aae2f3fe2c040e10d9982a3f4de
//Mengambil data dari db
void loadHiScores(HiScore scores[]);

//Menyimpan data ke db
void saveHiScores(HiScore scores[]);

//Menyimpan perubahan
void updateHighScore(Game* game, GameResources* resources);

//Aturan skor
void addScore(Game* game, int row);

//Getter score player
long long int playerScore(Game* game);
<<<<<<< HEAD
=======

//Getter mode
char* gameMode(GameResources* resources);
>>>>>>> cbf3f8c5a6734aae2f3fe2c040e10d9982a3f4de
#endif
