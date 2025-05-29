#ifndef FALIQ_H
#define FALIQ_H
#include "defines.h"

// =============================================================================
// POSITION AND COORDINATE SYSTEM
// Sistem koordinat dan posisi untuk elemen game
// =============================================================================

// Struktur posisi untuk menyimpan koordinat X dan Y
// Digunakan untuk tracking posisi shooter dan elemen lainnya
typedef struct {
    int x;  // Koordinat horizontal
    int y;  // Koordinat vertikal
} position;

// Variabel global untuk posisi shooter
// Menyimpan posisi current dari player shooter
extern position P;

// =============================================================================
// SHOOTER ASSET MANAGEMENT
// Manajemen aset visual untuk karakter shooter
// =============================================================================

// Membuat linked list untuk menyimpan semua aset shooter
// Menginisialisasi container untuk texture shooter dalam berbagai pose
SingleLinkedList* shooterAssets();

// Mendapatkan fragment texture shooter yang terletak di atas-tengah
Texture2D SLL_Shooter_top(GameResources* rsc);

//  Mendapatkan fragment texture shooter yang terletak di bawah-tengah
Texture2D SLL_Shooter_mid(GameResources* rsc);

//  Mendapatkan fragment texture shooter yang terletak di bawah-kiri
Texture2D SLL_Shooter_left(GameResources* rsc);

//  Mendapatkan fragment texture shooter yang terletak di bawah-kanan
Texture2D SLL_Shooter_right(GameResources* rsc);

// =============================================================================
// SHOOTER MECHANICS AND CONTROL
// Mekanika dan kontrol untuk karakter shooter
// =============================================================================

// Menggambar dan mengupdate posisi shooter
// Rendering shooter dengan texture yang sesuai berdasarkan state
void shooter(int* x, int* y, GameResources* resources);

// Menangani input pergerakan shooter
// Memproses keyboard input untuk movement kiri-kanan
void moveSet(int* x, GameResources* resources);

// =============================================================================
// AUDIO SYSTEM
// Sistem audio untuk musik dan efek suara
// =============================================================================

// Variabel global untuk musik gameplay
// Music stream yang akan diputar selama permainan berlangsung
extern Music soundGameplay;

// Menginisialisasi dan memutar musik gameplay
// Loading music file dan start playback untuk background music
void musicGameplay(GameResources* resources);

// =============================================================================
// SPECIAL ABILITIES SYSTEM
// Sistem kemampuan khusus seperti laser dan power-up
// =============================================================================

// Menangani mekanika laser shooter
// Memproses input laser, cooldown, dan efek visual laser beam
void handleLaser(Game* game);

// =============================================================================
// VISUAL EFFECTS AND ANIMATIONS
// Efek visual dan animasi untuk transisi dan opening
// =============================================================================

// Menghitung warna fade-in untuk animasi opening
// Menghasilkan efek transisi opacity berdasarkan progress
Color fadeInOpeningAnimation(float* trans);

// Menjalankan animasi pembuka game
// Sequence animasi intro dengan fade effects dan transitions
void openingAnimation(float* trans, GameResources* resources);

#endif