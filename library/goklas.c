#include "all.h"
#include "defines.h"

void InitBullets(Bullets bullets[]) {
    for (int i = 0;i < MAX_BULLETS;i++) {
        bullets[i].active = false;
        bullets[i].direction = 0;
    }
}

void ShootBullets(Bullets bullets[], Vector2 playerpos, int* BulletCount, bool* CanShoot, int direction, GameResources* resources) {
    float blockSize = auto_x(32);
    playerpos.x = roundf(playerpos.x / blockSize) * blockSize;
    playerpos.y = roundf(playerpos.y / blockSize) * blockSize;
    if (*BulletCount < MAX_BULLETS && *CanShoot) {
        for (int i = 0;i < MAX_BULLETS;i++) {
            if (!bullets[i].active) {
                bullets[i].position = (Vector2){ playerpos.x, GetScreenHeight() - 20 };
                bullets[i].direction = direction;
                bullets[i].active = true;
                (*BulletCount)++;
                PlaySound(SOUND(resources, SOUND_SHOOT));
                *CanShoot = false;
                break;
            }
        }
    }

}

void MoveBullets(Bullets bullets[]) {
    float blockSize = auto_x(32);
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            // Sesuaikan kecepatan peluru dengan scaling
            bullets[i].position.y -= blockSize / 3;  // Sesuaikan dengan ukuran block

            // Batasi area pergerakan peluru sesuai area game
            if (bullets[i].position.x < 0 || bullets[i].position.x > auto_x(320)) {
                bullets[i].active = false;
            }

            // Nonaktifkan peluru jika keluar dari layar atas
            if (bullets[i].position.y < 0) {
                bullets[i].active = false;
            }
        }
    }
}

void DrawBullets(Bullets bullets[], GameResources* resource) {
    float blockSize = auto_x(32);
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            // Sesuaikan ukuran peluru dengan grid
            float scale = blockSize / (float)TEXTURE(resource, TEXTURE_BULLET).width;
            DrawTextureEx(TEXTURE(resource, TEXTURE_BULLET), bullets[i].position, 0.0f, scale, WHITE
            );
        }
    }
}
