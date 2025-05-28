#ifndef RAFFI_H
#define RAFFI_H

//Inisialisasi Db
SingleLinkedList* initializeDb();

//Mencari nilai terbesar antara skor player dan HiScores
long long int getMax(HiScore scores[], Game* game, GameResources* resources);

//Mengambil data dari db
SingleLinkedList* loadHiScores();

//Menyimpan data ke db
void saveHiScores(SingleLinkedList* scores);

//Menyimpan perubahan
void updateHighScore(Game* game, GameResources* resources);

//Aturan skor
void addScore(Game* game, int row);

//Getter score player
long long int playerScore(Game* game);

//Getter mode
char* gameMode(GameResources* resources);
#endif
