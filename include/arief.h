#ifndef ARIEF_H
#define ARIEF_H

// Struktur Shooter:
// Menyimpan posisi pemain (x, y) dan jumlah amunisi yang dimiliki.
typedef struct {
    int x, y;    // Koordinat posisi pemain
    int ammo;    // Jumlah amunisi yang tersedia
} Shooter;

// Struktur Block:
// Mewakili sebuah blok di dalam game, bisa digunakan untuk rintangan atau elemen game lainnya.
// Disusun secara linked list untuk memudahkan manajemen blok secara dinamis.
typedef struct Block {
    int x, y;        // Koordinat posisi blok
    int jumlah;      // Jumlah atau nilai yang terkait dengan blok (misalnya, kekuatan atau tipe)
    Block* next;     // Pointer ke blok berikutnya dalam list
} Block;

// Struktur BlockQueue:
// Struktur antrian untuk mengelola blok dengan metode FIFO (First In First Out).
typedef struct {
    Block* front;    // Pointer ke blok paling depan dalam antrian
    Block* rear;     // Pointer ke blok paling belakang dalam antrian
} BlockQueue;

// Struktur Bullet:
// Mewakili peluru yang digunakan dalam game. Struktur ini memungkinkan manajemen daftar peluru secara dinamis.
typedef struct Bullet {
    int x, y;        // Koordinat posisi peluru
    Bullet* next;    // Pointer ke peluru berikutnya dalam linked list
} Bullet;

// Struktur Game:
// Menyimpan status permainan seperti skor, level, dan status permainan.
typedef struct {
    int hiscore;     // Skor tertinggi yang pernah dicapai
    int score;       // Skor permainan saat ini
    int level;       // Level permainan saat ini
    int play;        // Status permainan (misalnya, 1 untuk aktif, 0 untuk berhenti atau pause)
} Game;

#endif