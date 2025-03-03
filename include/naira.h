#ifndef NAIRA_H
#define NAIRA_H
#include "arief.h"
// Add these definitions to your header file
#define MAX_BLOCKS 100
#define BLOCK_SIZE 32
#define BOARD_COLUMNS 10

typedef struct {
    int x;
    int y;
    int jumlah;  // Value of block
    bool active; // Whether this block is currently in use
} BlockItem;


extern BlockItem blocks[MAX_BLOCKS];

void initBlocks() ;
void generateNewBlockRow() ;
#endif