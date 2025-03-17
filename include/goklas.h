#ifndef GOKLAS_H
#define GOKLAS_H
#include "defines.h"
// 
void InitBullets(Bullets bullets[]);
void ShootBullets(Bullets bullets[], Vector2 playerpos, int* BulletCount, bool* CanShoot, int direction);
void MoveBullets(Bullets bullets[]);
void DrawBullets(Bullets bullets[],GameResources *resource);

#endif