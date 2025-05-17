#ifndef GOKLAS_H
#define GOKLAS_H
#include "defines.h"


//Menembakan peluru dari posisi pemain
void ShootBullets(SingleLinkedList* P, Vector2 playerpos, int* BulletCount, bool* CanShoot, GameResources* resources);

//Menggerakan peluru yang aktif
void MoveBullets(SingleLinkedList* P);

//Memunculkan atau menggambar peluru
void DrawBullets(SingleLinkedList* P, GameResources* resource);

#endif