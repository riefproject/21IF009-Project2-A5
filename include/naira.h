#ifndef NAIRA_H
#define NAIRA_H
#include "defines.h"

// =============================================================================
// POWER-UP SPAWN SYSTEM
// System for generating and placing power-ups in the game
// =============================================================================

// Spawn a new power-up at random location in the game
// Creates and places a new power-up with random type on the game grid
void spawnPowerUp(Game* game);

// =============================================================================
// POWER-UP ACTIVATION SYSTEM
// System for collecting and activating power-up effects
// =============================================================================

// Activate collected power-up and apply its effects
// Processes power-up collection and applies corresponding game effects to player
void activatePowerUp(Game* game, GameResources* resources);

// Check if a specific power-up type is currently active
// Returns true if the power-up is active, and optionally returns its index
bool isPowerupTypeActive(Queue* q, PowerUpType type, int* index_out);
// =============================================================================
// POWER-UP RENDERING SYSTEM
// System for visual representation of power-ups
// =============================================================================

// Render all active power-ups on screen
// Draws power-up sprites with their visual effects and animations
void drawPowerUp(Game* game, GameResources* resources);

// =============================================================================
// POWER-UP STATE MANAGEMENT
// System for managing power-up states and timers
// =============================================================================

// Update all power-up states and timers
// Processes power-up lifetime, animations, collision detection, and cleanup
void updatePowerUp(Game* game, GameResources* resources);

#endif