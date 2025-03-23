#ifndef NAIRA_H
#define NAIRA_H
#include "defines.h"

// Spawn a new power-up
void spawnPowerUp(Game* game);
// Activate collected power-up
void activatePowerUp(Game* game, GameResources* resources);
// Render power-up on screen
void drawPowerUp(Game* game, GameResources* resources);
// Update power-up states
void updatePowerUp(Game* game, GameResources* resources);


#endif