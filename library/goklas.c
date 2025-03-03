#include "goklas.h"
#include <raylib.h>

extern Texture2D blockTexture;

void ShootBullets(Bullets bullets[],Vector2 playerpos, int *BulletCount, bool *CanShoot, int direction)
{
    if (*BulletCount < MAX_BULLETS && *CanShoot) {
        for(int i = 0;i < MAX_BULLETS;i++) {
            if (!bullets[i].active) {
                bullets[i].position = playerpos;
                bullets[i].direction = direction;
                bullets[i].active = true;
                (*BulletCount)++;
                *CanShoot = false;
                break;
            }
        }
    }
    
}

void MoveBullets(Bullets bullets[],int *BulletCount)
{
    for(int i = 0;i < MAX_BULLETS;i++) {
        if (bullets[i].active) {
            bullets[i].position.y -= 10;
            if (bullets[i].position.y < 0) {
                bullets[i].active = false;
            }

        }
    }
}

void DrawBullets(Bullets bullets[])
{
    for(int i = 0;i < MAX_BULLETS;i++) {
        if (bullets[i].active) {
            DrawTexture(blockTexture,bullets->position.x,bullets->position.y,BLUE);
        }
    }
}

void ReloadBullets(Bullets bullets[], int *BulletCount, bool *CanShoot)
{
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = false;
    }
    *BulletCount = 0;
    *CanShoot = true;
}