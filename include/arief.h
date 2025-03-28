#ifndef ARIEF_H
#define ARIEF_H
#include "defines.h"

// =====================================================================
//                          GAME MANAGEMENT
// =====================================================================

/* Fungsi-fungsi untuk mengelola konteks game dan aset */
Game* createGameContext(void);              // Membuat dan menginisialisasi konteks game baru
void destroyGameContext(Game* game);        // Membersihkan dan menghapus konteks game
Assets* createAssets(void);                 // Membuat dan memuat aset game
void destroyAssets(Assets* assets);         // Membersihkan dan menghapus aset game

// =====================================================================
//                          BLOCK MANAGEMENT
// =====================================================================

/* Fungsi-fungsi untuk mengelola BlockList */
void addBlock(BlockList* list, int x, int y, int jumlah);               // Menambah blok baru ke list
void updateBlock(BlockList* list, int pos, int x, int y, int jumlah);   // Mengupdate blok pada posisi tertentu
void removeBlock(BlockList* list, int pos);                             // Menghapus blok dari list
void printBlockList(BlockList* list);                                   // Menampilkan isi BlockList
int countBlock(BlockList* list);                                        // Menghitung jumlah blok dalam list

/* Fungsi-fungsi untuk mengelola BlockQueue */
void initQueue(BlockQueue* q);                 // Inisialisasi queue baru
int isEmpty(BlockQueue* q);                    // Cek apakah queue kosong
int isFull(BlockQueue* q);                     // Cek apakah queue penuh
int countQueue(BlockQueue* q);                 // Menghitung jumlah item dalam queue
void enqueue(BlockQueue* q, Block* element);   // Menambah blok ke queue
Block* dequeue(BlockQueue* q);                 // Mengambil blok dari queue
void displayQueue(BlockQueue* q);              // Menampilkan isi queue
void clearQueue(BlockQueue* q);                // Mengosongkan queue

// =====================================================================
//                          USER INTERFACE
// =====================================================================

/* Fungsi-fungsi untuk menangani tampilan dan menu */
int loadingScreen(float* loadingTime);          // Menampilkan layar loading
void mainWindow(void);                          // Menampilkan jendela utama
void mainMenu(GameResources* resources);        // Menampilkan menu utama
void resetHiScores(GameResources* resources);   // Reset skor tertinggi
void rejectReset(GameResources* resources);     // Membatalkan reset skor
void gameOver(GameResources* resources, long long int currentScore);        // Menampilkan layar game over
void selectMode(GameResources* resources);      // Memilih mode permainan
void showControls(GameResources* resources);    // Menampilkan kontrol game
void showHiScore(GameResources* resources);     // Menampilkan skor tertinggi
void showSettings(GameResources* resources);    // Menampilkan pengaturan
bool confirmExit(GameResources* resources);     // Konfirmasi keluar game
void exitGame(GameResources* resources);        // Keluar dari game
void pauseMenu(GameResources* resources);       // Menu pause
void countdownPause(GameResources* resources);  // Hitung mundur pause
bool confirmBack(GameResources* resources);     // Konfirmasi kembali

// =====================================================================
//                          GAME MECHANICS
// =====================================================================

/* Fungsi-fungsi untuk logika dan mekanika game */
float getSpeedForMode(Game* game, int mode);              // Mendapatkan kecepatan berdasarkan mode
void displayGame(GameResources* resources);               // Menampilkan game
bool isGameOverCheck(Game* game);                         // Cek kondisi game over
bool isRowFull(Game* game, int row);                      // Cek apakah baris penuh
bool hasActiveBlockBelow(Game* game, int row);            // Cek blok aktif di bawah
void shiftRowsUp(Game* game, int startRow);               // Geser baris ke atas
void handleFullRow(Game* game, int row);                  // Menangani baris penuh
void updateBlocks(Game* game, GameResources* resources);  // Update status blok
void initBlocks(Game* game, GameResources* resources);    // Inisialisasi blok
void drawBlocks(Game* game, GameResources* resources);    // Menggambar blok
void drawBlockUI(Game* game);                             // Menggambar UI blok
void printGrid(Game* game);                               // Menampilkan grid game

/* Fungsi-fungsi untuk pergerakan dan collision */
void handleBlockMovement(Game* game, int minBlocks, int maxBlocks);    // Menangani pergerakan blok
void moveBlocksDown(Game* game);                                       // Menggerakkan blok ke bawah
void generateNewBlocks(Game* game, int minBlocks, int maxBlocks, int* emptyColLength, int totalEmptyColumns);     // Membuat blok baru
int fillCriticalGaps(Game* game, int remainingBlocks, int* emptyColLength);                                       // Mengisi celah kritis
void fillRemainingBlocks(Game* game, int remainingBlocks);             // Mengisi sisa blok
void handleBulletCollisions(Game* game);                               // Menangani collision peluru
bool isValidGridPosition(int x, int y);                                // Validasi posisi grid
void processBulletHit(Game* game, int gridX, int gridY, int bulletIndex);                                         // Memproses tembakan peluru
void drawGameUI(Game* game, GameResources* resources);         // Menggambar UI game
#endif