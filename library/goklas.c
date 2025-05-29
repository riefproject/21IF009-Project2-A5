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
}


void handleBulletCollisions(Game* game) {
    SLLNode* current = game->bullets->head;
    while (current != NULL) {
        Bullets* bullets = (Bullets*)current->data;
        if (bullets->active) {
            float blockSize = auto_x(32);

            int gridX = (int)(bullets->position.x / blockSize);
            int gridY = (int)(bullets->position.y / blockSize);

            if (isValidGridPosition(gridX, gridY)) {
                DLLNode* rowNode = game->grid->head;
                for (int y = 0; y < gridY && rowNode; y++) {
                    rowNode = rowNode->next;
                }

                if (rowNode) {
                    DoublyLinkedList* row = (DoublyLinkedList*)rowNode->data;

                    DLLNode* blockNode = row->head;
                    for (int x = 0; x < gridX && blockNode; x++) {
                        blockNode = blockNode->next;
                    }

                    if (blockNode) {
                        Block* block = (Block*)blockNode->data;
                        if (block->active) {
                            processBulletHit(game, gridX, gridY, bullets);
                        }
                    }
                }
            }
        }
        current = current->next;
    }
}