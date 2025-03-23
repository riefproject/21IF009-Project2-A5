#include "all.h"
#include "defines.h"

void InitBullets(Bullets bullets[]) {
    for (int i = 0;i < MAX_BULLETS;i++) {
        bullets[i].active = false;
        bullets[i].direction = 0;
    }
}

void ShootBullets(Bullets bullets[], Vector2 playerpos, int* BulletCount, bool* CanShoot, int direction,GameResources *resources) {
    if (*BulletCount < MAX_BULLETS && *CanShoot) {
        for (int i = 0;i < MAX_BULLETS;i++) {
            if (!bullets[i].active) {
                bullets[i].position = (Vector2){ playerpos.x, GetScreenHeight() - 20 };
                bullets[i].direction = direction;
                bullets[i].active = true;
                (*BulletCount)++;
                PlaySound(SOUND(resources,SOUND_SHOOT));
                *CanShoot = false;
                break;
            }
        }
    }

}

void MoveBullets(Bullets bullets[]) {
    for (int i = 0;i < MAX_BULLETS;i++) {
        if (bullets[i].active) {
            bullets[i].position.x += bullets[i].direction * 5;
            bullets[i].position.y -= 10;
            if (bullets[i].position.x < 0 || bullets[i].position.x > GetScreenWidth()) {
                bullets[i].active = false;
            }

        }
    }
}

void DrawBullets(Bullets bullets[],GameResources *resource) {
    for (int i = 0;i < MAX_BULLETS;i++) {
        if (bullets[i].active) {
            DrawTexture(TEXTURE(resource, TEXTURE_BULLET),bullets[i].position.x,bullets[i].position.y,WHITE);
        }
    }
}
