#include "all.h"
#include <raylib.h>
#include <stdbool.h>

// Add these helper functions
void initBlocks() {
    for (int i = 0; i < MAX_BLOCKS; i++) {
        blocks[i].active = false;
    }
}
#define SNAP_DISTANCE 2  // Jarak untuk menempel
float gameSpeed = 1.0f;  // Speed multiplier (1.0 = normal, 2.0 = 2x faster, etc.)
BlockItem blocks[MAX_BLOCKS];

// Modifikasi generateNewBlockRow()
void generateNewBlockRow() {
    // Generate 2-5 random blocks across the top
    int count = (rand() % 4) + 2;
    bool used[BOARD_COLUMNS] = { 0 };

    for (int i = 0; i < count; i++) {
        int column;
        do {
            column = rand() % BOARD_COLUMNS;
        } while (used[column]);

        used[column] = true;

        for (int j = 0; j < MAX_BLOCKS; j++) {
            if (!blocks[j].active) {
                blocks[j].x = column * BLOCK_SIZE;
                blocks[j].y = -BLOCK_SIZE;
                blocks[j].jumlah = (rand() % 9) + 1;
                blocks[j].active = true;
                break;
            }
        }
    }
}
// Main game function