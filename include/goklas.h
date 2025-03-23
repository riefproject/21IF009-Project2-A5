#ifndef GOKLAS_H
#define GOKLAS_H
#include "defines.h"

//Menyiapkan peluru  
void InitBullets(Bullets bullets[]);

//Menembakan peluru dari posisi pemain
void ShootBullets(Bullets bullets[], Vector2 playerpos, int* BulletCount, bool* CanShoot, int direction,GameResources *resources);

//Menggerakan peluru yang aktif
void MoveBullets(Bullets bullets[]);

//Memunculkan atau menggambar peluru
void DrawBullets(Bullets bullets[],GameResources *resource);

#endif