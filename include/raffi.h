#ifndef RAFFI_H
#define RAFFI_H
#include "defines.h"

// =============================================================================
// DATABASE INITIALIZATION AND MANAGEMENT
// Inisialisasi dan manajemen database untuk high scores
// =============================================================================

// Menginisialisasi struktur database untuk high scores
// Membuat linked list kosong sebagai container untuk menyimpan data scores
SingleLinkedList* initializeDb();

// Memuat data high scores dari file database
// Membaca file save dan mengisi linked list dengan data high scores yang tersimpan
SingleLinkedList* loadHiScores();

// Menyimpan data high scores ke file database
// Menulis linked list scores ke file untuk persistensi data
void saveHiScores(SingleLinkedList* scores);

// =============================================================================
// SCORE MANAGEMENT SYSTEM
// Sistem manajemen skor dan pencapaian pemain
// =============================================================================

// Memperbarui high score jika skor current lebih tinggi
// Membandingkan skor current dengan high score dan update jika perlu
void updateHighScore(Game* game, GameResources* resources);

// Menambahkan skor berdasarkan baris yang dihancurkan
// Menghitung dan menambahkan poin berdasarkan jumlah baris yang cleared
void addScore(Game* game, int row);

// Mendapatkan skor current pemain
// Mengambil nilai skor yang sedang berjalan dalam game session
long long int playerScore(Game* game);

// =============================================================================
// GAME STATE UTILITIES
// Utilitas untuk mengambil informasi state game
// =============================================================================

// Mendapatkan nama mode permainan yang sedang dimainkan
// Mengembalikan string identifier untuk mode game current
char* gameMode(GameResources* resources);
#endif