#ifndef GOKLAS_H
#define GOKLAS_H
#include "defines.h"

// =============================================================================
// BULLET SYSTEM MANAGEMENT
// Sistem manajemen peluru untuk gameplay shooting
// =============================================================================

// Menembakkan peluru dari posisi pemain
// Membuat bullet baru dan menambahkannya ke linked list dengan cooldown control
void ShootBullets(SingleLinkedList* P, Vector2 playerpos, int* BulletCount, bool* CanShoot, GameResources* resources);

// Menggerakkan semua peluru yang aktif
// Update posisi semua bullets dalam linked list dan hapus yang keluar bounds
void MoveBullets(SingleLinkedList* P);

// Menggambar semua peluru pada layar
// Rendering visual untuk semua bullets yang aktif dalam game
void DrawBullets(SingleLinkedList* P, GameResources* resource);

// Menangani collision antara bullet dan blok
// Deteksi hit, damage calculation, dan cleanup bullet
void handleBulletCollisions(Game* game);

#endif