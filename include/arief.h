#ifndef ARIEF_H
#define ARIEF_H
#include "defines.h"

// Resources Function
#ifndef ASSET_INPUT_HELPERS
#define ASSET_INPUT_HELPERS
void GetAdjustedWindowSize(int width, int height, int* outWidth, int* outHeight);
InputAsset* inputAssets(TypeofAssets type, uint id, const char* path);
Assets* createAssets(void);
void* getAsset(SLLNode* head, uint id);
void unloadAndFree(SLLNode* head, TypeofAssets type);
void destroyAssets(Assets* assets);
#endif


// 
// 
// 
void loadSettings(Settings* settings);

// 
// 
// 
void saveSettings(Settings settings);

// 
// 
// 
DoublyLinkedList* initGameGrid(void);

// 
// 
// 
Game* createGameContext(void);

// 
// 
// 
void destroyGameContext(Game* game);

// 
// 
// 
ScaleFactor GetScreenScaleFactor(void);

// 
// 
// 
void GetAdjustedWindowSize(int width, int height, int* outWidth, int* outHeight);

// 
// 
// 
InputAsset* inputAssets(TypeofAssets type, uint id, const char* path);

// 
// 
// 
Assets* createAssets(void);

// 
// 
// 
void* getAsset(SLLNode* head, uint id);

// 
// 
// 
void unloadAndFree(SLLNode* head, TypeofAssets type);

// 
// 
// 
void destroyAssets(Assets* assets);

// 
// 
// 
void mainWindow(void);

// 
// 
// 
void drawBG(GameResources* resources, uint id);

// 
// 
// 
int loadingScreen(GameResources* resources, float* loadingTime);

// 
// 
// 
void showCredits(GameResources* resources);

// 
// 
// 
void mainMenu(GameResources* resources);

// 
// 
// 
void showControls(GameResources* resources);

// 
// 
// 
void showSettings(GameResources* resources);

// 
// 
// 
void showHiScore(GameResources* resources);

// 
// 
// 
bool confirmExit(GameResources* resources);

// 
// 
// 
void exitGame(GameResources* resources);

// 
// 
// 
bool confirmBack(GameResources* resources);

// 
// 
// 
void pauseMenu(GameResources* resources);

// 
// 
// 
void countdownPause(GameResources* resources);

// 
// 
// 
void selectMode(GameResources* resources);

// 
// 
// 
bool confirmReset(GameResources* resources);

// 
// 
// 
void resetHiScores(GameResources* resources);

// 
// 
// 
void rejectReset(GameResources* resources);

// 
// 
// 
void gameOver(GameResources* resources, ll currentScore);

// 
// 
// 
float getSpeedForMode(Game* game, int mode);

// 
// 
// 
static void getBlockRangeForMode(int mode, int* minBlocks, int* maxBlocks);

// 
// 
// 
void displayGame(GameResources* resources);

// 
// 
// 
bool isGameOverCheck(Game* game);

// 
// 
// 
bool isRowFull(Game* game, int row);

// 
// 
// 
bool hasActiveBlockBelow(Game* game, int row);

// 
// 
// 
void shiftRowsUp(Game* game, int startRow);

// 
// 
// 
void handleFullRow(Game* game, int row);

// 
// 
// 
void handleBlockMovement(Game* game, int minBlocks, int maxBlocks);

// 
// 
// 
void moveBlocksDown(Game* game);

// 
// 
// 
void generateNewBlocks(Game* game, int minBlocks, int maxBlocks, int* emptyColLength, int totalEmptyColumns);

// 
// 
// 
int fillCriticalGaps(Game* game, int remainingBlocks, int* emptyColLength);

// 
// 
// 
void fillRemainingBlocks(Game* game, int remainingBlocks);

// 
// 
// 
void handleBulletCollisions(Game* game);

// 
// 
// 
bool isValidGridPosition(int x, int y);

// 
// 
// 
void processBulletHit(Game* game, int gridX, int gridY, Bullets* bullets);

// 
// 
// 
void updateBlocks(Game* game, GameResources* resources);

// 
// 
// 
void initBlocks(Game* game, GameResources* resources);

// 
// 
// 
void drawBlocks(Game* game, GameResources* resources);

// 
// 
// 
void printGrid(Game* game);

// 
// 
// 
void drawGameUI(Game* game, GameResources* resources);

// 
// 
// 
ll getCurrentModeHighScore(GameResources* resources);

// 
// 
// 
ll getMaxScoreToShow(Game* game, GameResources* rsc);

#endif