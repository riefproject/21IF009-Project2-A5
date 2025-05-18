#ifndef RAFFI_H
#define RAFFI_H

//Inisialisasi Db
SingleLinkedList *initializeDb();

//Mengambil data dari db
SingleLinkedList *loadHiScores();

//Menyimpan data ke db
void saveHiScores(SingleLinkedList *list);

//Menyimpan perubahan
void updateHighScore(Game* game, GameResources* resources);

//Aturan skor
void addScore(Game* game, int row);

//Getter score player
long long int playerScore(Game* game);

//Getter mode
char* gameMode(GameResources* resources);
#endif
