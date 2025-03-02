#include "naira.h"
#include "arief.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <raylib.h>
#include <math.h>

extern GameState currentState;
extern GameState prevState;
// extern int blocks[MAX_ROWS][MAX_COLUMNS];
extern Texture2D blockTexture;

int frameCounter = 0;
int currentRow = 0;

//=====Halo, untuk check display di arief.c go to line 840


//Code in the making + testing
//Output may not be as expected


// void block_falling(int gamestate[NO_BLOCKS_FOR_GAP][NUMBER_OF_BLOCKS_HORIZONTALLY])
// {
//     int temporary_storage;
//     bool is_falling = true;

//     while (is_falling)
//     {
//         is_falling = false; 

//         for (int row_counter = NO_BLOCKS_FOR_GAP - 2; row_counter >= 0; row_counter--) 
//         {
//             for (int column_counter = 0; column_counter < NUMBER_OF_BLOCKS_HORIZONTALLY; column_counter++)
//             {
//                 if ((gamestate[row_counter][column_counter] == 1) || (gamestate[row_counter][column_counter] == 2))
//                 {
//                     if (gamestate[row_counter + 1][column_counter] == 0) 
//                     {
//                         temporary_storage = gamestate[row_counter][column_counter];
//                         gamestate[row_counter][column_counter] = gamestate[row_counter + 1][column_counter];
//                         gamestate[row_counter + 1][column_counter] = temporary_storage;
//                         is_falling = true;
//                     }
//                 }
//             }
//         }
//     }
// }

void printFallingBlocks(int fallingBlocks[FALLING_BLOCKS_ROWS][FALLING_BLOCKS_COLS]) {
    printf("Current Blocks:\n");
    for (int i = 0; i < FALLING_BLOCKS_ROWS; i++) {
        for (int j = 0; j < FALLING_BLOCKS_COLS; j++) {
            printf("%d ", fallingBlocks[i][j]);
        }
        printf("\n");
    }
}

// void updateBlocks(BlockList* list) {
//     // Update block positions using the linked list
//     Block* current = list->head;
//     while (current != NULL) {
//         current->y += BLOCK_SIZE; // Move block down
//         current = current->next;
//     }
// }

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




// void generateNewBlockRow(Block2 blocks[MAX_BLOCKS]) {
//     int count = (rand() % 4) + 2;  // Generate 2-5 blocks
//     bool used[BOARD_COLUMNS] = { 0 };

//     printf("Generating %d new blocks...\n", count);

//     for (int i = 0; i < count; i++) {
//         int column;
//         do {
//             column = rand() % BOARD_COLUMNS;
//         } while (used[column]);

//         used[column] = true;

//         for (int j = 0; j < MAX_BLOCKS; j++) {
//             if (!blocks[j].active) {
//                 blocks[j].x = column * BLOCK_SIZE;
//                 blocks[j].y = -BLOCK_SIZE;
//                 blocks[j].jumlah = (rand() % 9) + 1;
//                 blocks[j].active = true;
//                 printf("Block %d created at column %d (x: %d, y: %d)\n", j, column, blocks[j].x, blocks[j].y);
//                 break;
//             }
//         }
//     }
// }


// void drawFallingBlocks(int fallingBlocks[FALLING_BLOCKS_ROWS][FALLING_BLOCKS_COLS]) {
//     for (int i = 0; i < FALLING_BLOCKS_ROWS; i++) {
//         for (int j = 0; j < FALLING_BLOCKS_COLS; j++) {
//             if (fallingBlocks[i][j] == 1) {  
//                 int x = j * BLOCK_SIZE;  
//                 int y = i * BLOCK_SIZE;  
//                 DrawRectangle(x, y, BLOCK_SIZE, BLOCK_SIZE, DARKGRAY);
//             }
//         }
//     }
// }


void drawFallingBlocks(BlockRow fallingBlocks[FALLING_BLOCKS_ROWS]) {
    for (int row = 0; row < FALLING_BLOCKS_ROWS; row++) {
        for (int i = 0; i < fallingBlocks[row].numBlocks; i++) {
            DrawRectangle(fallingBlocks[row].blocks[i].x, fallingBlocks[row].blocks[i].y, BLOCK_SIZE, BLOCK_SIZE, RED);
        }
    }
}






///////////////////////////////////////////////////////////////////////////////////////////////////////

// void displayGame2() {
//     prevState = STATE_PLAY;
//     int fallingBlocks[FALLING_BLOCKS_ROWS][FALLING_BLOCKS_COLS] = {0};
//     block_falling(fallingBlocks);
//     initFallingBlocks(fallingBlocks);

//     float lastUpdateTime = 0.0f;

//     while (currentState == STATE_PLAY && !WindowShouldClose()) {
//         float currentTime = GetTime();

//         if (currentTime - lastUpdateTime >= FALLING_BLOCKS_UPDATE_INTERVAL) {
//             // updateBlocks(fallingBlocks);
//             lastUpdateTime = currentTime;
//         }

//         BeginDrawing();
//         ClearBackground(DARKGRAY);
//         drawFallingBlocks(fallingBlocks);
//         DrawText("Hi Score", 340, 20, 15, WHITE);
//         DrawText("12345", 340, 40, 20, LIGHTGRAY);
//         DrawText("Dalam Tahap Pengembangan!", 100, 300, 20, LIGHTGRAY);
//         DrawTexture(blockTexture, 0, 0, WHITE);
//         EndDrawing();

//         if (IsKeyPressed(KEY_P)) {
//             while (IsKeyDown(KEY_P) && !WindowShouldClose()) {
//                 BeginDrawing();
//                 ClearBackground(DARKGRAY);
//                 DrawText("Paused", 200, 200, 30, WHITE);
//                 EndDrawing();
//             }
//             pauseMenu(GetFontDefault());
//         }
//     }
// }

///////////////////////////////////////////////////////////////////////////////////// new code testing vvv

// Define speeds for 11 difficulty levels (higher index = faster speed)
int difficultySpeeds[DIFFICULTY_LEVELS] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 15};

void InitDifficultySpeeds() {
    // If you ever need to tweak speeds dynamically, do it here.
}

void GenerateBlockRow(BlockRow *row, int difficulty) {
    row->numBlocks = (rand() % 5) + 5;  // Random number between 5 and 9

    for (int i = 0; i < row->numBlocks; i++) {
        row->blocks[i].x = rand() % GRID_WIDTH;  // Random X position within grid width
        row->blocks[i].y = 0;  // Start at the top
    }

    row->speed = difficultySpeeds[difficulty];  // Assign speed based on difficulty
}

void UpdateBlockRow(BlockRow *row) {
    for (int i = 0; i < row->numBlocks; i++) {
        row->blocks[i].y += row->speed;  // Move blocks downward
    }
}

BlockRow row;
// Inside game update loop:)
//Should I use this? unsure..
void UpdateBlockRow(BlockRow *row);
