#ifndef RAFFI_H
#define RAFFI_H

//Inisialisasi Db
void initializeDb(HiScore scores[]);

//Mencari nilai terbesar antara skor player dan HiScores
long long int getMax(HiScore scores[], Game* game, GameResources* resources);

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

//Getter mode
char* gameMode(GameResources* resources);
#endif
