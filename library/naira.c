#include "naira.h"
#include "arief.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <raylib.h>
#include <math.h>

int frameCounter = 0;
int currentRow = 0;

//=====Halo, untuk check display di arief.c go to line 996

void printFallingBlocks(int fallingBlocks[FALLING_BLOCKS_ROWS][FALLING_BLOCKS_COLS]) {
    printf("Current Blocks:\n");
    for (int i = 0; i < FALLING_BLOCKS_ROWS; i++) {
        for (int j = 0; j < FALLING_BLOCKS_COLS; j++) {
            printf("%d ", fallingBlocks[i][j]);
        }
        printf("\n");
    }
}

void initFallingBlocks(int fallingBlocks[FALLING_BLOCKS_ROWS][FALLING_BLOCKS_COLS]) {
    for (int i = 0; i < FALLING_BLOCKS_ROWS; i++) {
        for (int j = 0; j < FALLING_BLOCKS_COLS; j++) {
            fallingBlocks[i][j] = 0;
        }
    }

    for (int j = 0; j < FALLING_BLOCKS_COLS; j++) {
        fallingBlocks[0][j] = 1;
    }

    printf("\n--- START GRID ---\n");
    printFallingBlocks(fallingBlocks);
    printf("------------------\n");
}

void drawFallingBlocks(BlockRow* row) {
    if (row == NULL || row->numBlocks <= 0) return;

    for (int i = 0; i < row->numBlocks; i++) {
        if (i >= MAX_BLOCKS_PER_ROW) break; // Prevent out-of-bounds
        DrawRectangle(row->blocks[i].x, row->blocks[i].y, BLOCK_SIZE, BLOCK_SIZE, RED);
    }
}


// Define speeds for 11 difficulty levels (higher index = faster speed)
int difficultySpeeds[DIFFICULTY_LEVELS] = { 1, 2, 3, 4, 5 };

void InitDifficultySpeeds() {
    // might need to tweak and change the difficulty speed dynamically..
}

void GenerateBlockRow(BlockRow* row, int difficulty) {
    printf("Entering GenerateBlockRow\n");
    printf("row = %p, difficulty = %d\n", (void*)row, difficulty);
    
    // drawFallingBlocks(row);

    if (row == NULL) {
        printf("Error: row is NULL!\n");
        return;
    }

    if (difficulty < 0 || difficulty >= DIFFICULTY_LEVELS) {
        printf("Error: difficulty index %d is out of bounds!\n", difficulty);
        return;
    }

    printf("row->blocks = %p\n", (void*)row->blocks);

    row->numBlocks = (rand() % 5) + 5;  // Between 5-9 blocks
    printf("row->numBlocks = %d\n", row->numBlocks);

    if (row->numBlocks > MAX_BLOCKS_PER_ROW) {  
        printf("Warning: numBlocks exceeded MAX_BLOCKS_PER_ROW, reducing to max.\n");
        row->numBlocks = MAX_BLOCKS_PER_ROW;
    }

    //============================this caused an error vvvv
    for (int i = 0; i < row->numBlocks && i < MAX_BLOCKS_PER_ROW; i++) {
        printf("Accessing block %d at address %p\n", i, (void*)&row->blocks[i]);
        row->blocks[i].x = (rand() % (GRID_WIDTH / BLOCK_SIZE)) * BLOCK_SIZE;  
        row->blocks[i].y = 1;
        printf("Block %d: x = %d, y = %d\n", i, row->blocks[i].x, row->blocks[i].y);
    }

    //==============================

    printf("Accessing difficultySpeeds[%d] at address %p\n", difficulty, (void*)&difficultySpeeds[difficulty]);
    row->speed = difficultySpeeds[difficulty];  
    printf("Generated row with %d blocks at difficulty %d.\n", row->numBlocks, difficulty);
}


void UpdateBlockRow(BlockRow* row) {
    if (row == NULL) {
        printf("Error: row is NULL!\n");
        return;
    }

    if (row->numBlocks <= 0) {
        printf("Error: numBlocks is 0!\n");
        return;
    }

    printf("Updating row with %d blocks...\n", row->numBlocks);
    
    for (int i = 0; i < row->numBlocks; i++) {
        if (i >= MAX_BLOCKS_PER_ROW) {
            printf("Error: Index %d out of bounds!\n", i);
            break;
        }

        row->blocks[i].y += row->speed;
    }
}
