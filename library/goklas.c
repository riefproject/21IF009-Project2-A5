#include "all.h"
#include "defines.h"

void ShootBullets(SingleLinkedList* P, Vector2 playerpos, int* BulletCount, bool* CanShoot, GameResources* resources) {
    if (P == NULL) return;
    float blockSize = auto_x(32);
    playerpos.x = roundf(playerpos.x / blockSize) * blockSize;
    playerpos.y = roundf(playerpos.y / blockSize) * blockSize;

    if (*CanShoot) {
        Bullets* bullets = (Bullets*)malloc(sizeof(Bullets));
        bullets->position = (Vector2) {playerpos.x, GetScreenHeight() - 20};
        bullets->active = true;
        (*BulletCount)++;

        SLL_insertBack(P,bullets);
        PlaySound(SOUND(resources, SOUND_SHOOT));
        *CanShoot = false;
    }

    // if (*BulletCount < MAX_BULLETS && *CanShoot) {
    //     for (int i = 0;i < MAX_BULLETS;i++) {
    //         if (!bullets[i].active) {
    //             bullets[i].position = (Vector2){ playerpos.x, GetScreenHeight() - 20 };
    //             bullets[i].direction = direction;
    //             bullets[i].active = true;
    //             (*BulletCount)++;
    //             PlaySound(SOUND(resources, SOUND_SHOOT));
    //             *CanShoot = false;
    //             break;
    //         }
    //     }
    // }

}

void MoveBullets(SingleLinkedList* P) {
    if (P == NULL) return;
    float blockSize = auto_x(32);
    SLLNode* current = P->head;
    SLLNode* prev = NULL;

    while (current != NULL) {
        Bullets* bullet = (Bullets*)current->data;
        SLLNode* next = current->next; 

        if (bullet->active) {
            bullet->position.y -= blockSize / 3;

            if (bullet->position.y < 0) {
                bullet->active = false;
                next = current->next;
                SLL_removeNode(P, current, -1);
                current = next;
                continue; 
            }
        }

        prev = current;
        current = next;
    }
}


    // for (int i = 0; i < MAX_BULLETS; i++) {
    //     if (bullets[i].active) {
    //         // Sesuaikan kecepatan peluru dengan scaling
    //         bullets[i].position.y -= blockSize / 3;  // Sesuaikan dengan ukuran block

    //         // Batasi area pergerakan peluru sesuai area game
    //         if (bullets[i].position.x < 0 || bullets[i].position.x > auto_x(320)) {
    //             bullets[i].active = false;
    //         }

    //         // Nonaktifkan peluru jika keluar dari layar atas
    //         if (bullets[i].position.y < 0) {
    //             bullets[i].active = false;
    //         }
    //     }
    // }


void DrawBullets(SingleLinkedList* P, GameResources* resource) {
    if (P == NULL) return;
    float blockSize = auto_x(32);
    if (P != NULL) {
        SLLNode* head = P->head;
        while (head != NULL) {
            Bullets* bullets = (Bullets*)head->data;
            if (bullets->active) {
                float scale = blockSize / (float)TEXTURE(resource, TEXTURE_BULLET).width;
                DrawTextureEx(TEXTURE(resource, TEXTURE_BULLET), bullets->position, 0.0f, scale, WHITE);
            } 
            head = head->next;
        } 
    }

    // for (int i = 0; i < MAX_BULLETS; i++) {
    //     if (bullets[i].active) {
    //         // Sesuaikan ukuran peluru dengan grid
    //         float scale = blockSize / (float)TEXTURE(resource, TEXTURE_BULLET).width;
    //         DrawTextureEx(TEXTURE(resource, TEXTURE_BULLET), bullets[i].position, 0.0f, scale, WHITE
    //         );
    //     }
    // }
}
