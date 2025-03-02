#ifndef NAIRA_H
#define NAIRA_H
#include "arief.h"

// Falling blocks mechanism
#define MAX_HEIGHT_BLOCKS 20 // Define the maximum height of blocksks
#define BLOCK_SIZE 32  // Adjust if needed
#define FALLING_BLOCKS_UPDATE_INTERVAL 0.5f // Update every 0.5 seconds
#define FALLING_BLOCKS_ROWS MAX_HEIGHT_BLOCKS // Define the number of rows for falling blocks
#define FALLING_BLOCKS_COLS NUMBER_OF_BLOCKS_HORIZONTALLY
#define MAX_BLOCKS 10
#define BOARD_COLUMNS 10


#define MAX_ROWS MAX_HEIGHT_BLOCKS
#define MAX_COLUMNS FALLING_BLOCKS_COLS 
#define ROW_DELAY 60 // Frames to wait before spawning the next row
#define BLOCK_FALL_SPEED 2 // How fast blocks fall
#define NUMBER_OF_BLOCKS_HORIZONTALLY 10
#define NO_BLOCKS_FOR_GAP 20  

#define MAX_BLOCKS 10

// max blocks, size of block = 32
// column (horizontal) = 10
// max block = row = array of Block2 dengan jumlah element sebanyak barisnya (maybe 16 or more)
// array of Block2 depends on the struct later (more difficult...probably...)

// typedef struct {
//     int x, y;
//     int jumlah;
//     bool active;
// } Block3;

// extern Block2 blocks[MAX_BLOCKS];

// This is the initial blocks
// void pauseMenu(Font font);
// void initFallingBlocks(int fallingBlocks[FALLING_BLOCKS_ROWS][FALLING_BLOCKS_COLS]);
// void updateFallingBlocks(int fallingBlocks[FALLING_BLOCKS_ROWS][FALLING_BLOCKS_COLS]);
// void drawFallingBlocks(BlockRow fallingBlocks[FALLING_BLOCKS_ROWS][FALLING_BLOCKS_COLS]);
// void generateNewBlockRow(Block2 blocks[MAX_BLOCKS]);

//=================================================================================================================== trying new header

#define GRID_WIDTH 10  // Change based on your grid size
#define GRID_HEIGHT 20
#define MAX_BLOCKS_PER_ROW 10  // Max blocks that can appear per row
#define DIFFICULTY_LEVELS 11  // 11 difficulty levels

typedef struct {
    int x;  // X-coordinate
    int y; 
    int jumlah;
    bool active; // Y-coordinate (for falling)
} Block2;

typedef struct {
    Block2 blocks[MAX_BLOCKS_PER_ROW];  // Blocks in a row
    int numBlocks;  // Number of blocks in this row
    int speed;
    bool active;  // Speed of falling
} BlockRow;

// Arrays for block behavior
extern int difficultySpeeds[DIFFICULTY_LEVELS];  // Stores speeds for different difficulties

// Function declarations
void InitDifficultySpeeds();  
void GenerateBlockRow(BlockRow *row, int difficulty);
void UpdateBlockRow(BlockRow *row);

void initFallingBlocks(int fallingBlocks[FALLING_BLOCKS_ROWS][FALLING_BLOCKS_COLS]);
void updateFallingBlocks(int fallingBlocks[FALLING_BLOCKS_ROWS][FALLING_BLOCKS_COLS]);
void drawFallingBlocks(BlockRow fallingBlocks[FALLING_BLOCKS_ROWS]);


#endif