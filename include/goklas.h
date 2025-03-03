#ifndef GOKLAS_H
#define GOKLAS_H

#include <raylib.h>
#include <stdlib.h>

#define MAX_BULLETS 100
#define RELOAD_DURATION 2.0 

extern Texture2D blockTexture;

typedef struct {
    Vector2 position;
    int direction;
    bool active;
} Bullets;


void ShootBullets(Bullets bullets[],Vector2 playerpos, int *BulletCount, bool *CanShoot, int direction);
void MoveBullets(Bullets bullets[], int *BulletCount);
void DrawBullets(Bullets bullets[]);
void ReloadBullets(Bullets bullets[], int *BulletCount, bool *CanShoot);

#endif