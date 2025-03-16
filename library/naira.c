#include "all.h"

void spawnPowerUp(Game* game) {
    game->powerupActive = true;
    game->currentPowerup.type = rand() % POWERUP_COUNT;
    game->powerupPosition = (Vector2){
        (float)(rand() % (320 - 32)), // Using 320
        -32.0f  // Start above screen
    };
}

void activatePowerUp(Game* game) {
    if (game->activeEffectsCount >= 3) return; // Maximum 3 active effects

    PowerUpType type = game->currentPowerup.type;
    if (type == POWERUP_RANDOM) {
        type = rand() % (POWERUP_COUNT - 1);
    }

    // Dont add temporary effects if same type is already active
    for (int i = 0; i < game->activeEffectsCount; i++) {
        if (game->activePowerups[i].type == type) {
            game->activePowerups[i].duration = 10.0f; // Reset duration
            game->powerupActive = false;
            game->powerupTimer = 7.0f + (rand() % 8);
            return;
        }
    }

    float duration = 10.0f; // Default duration for temporary effects

    switch (type) {
    case POWERUP_SPEED_UP:
        game->rowAddDelay /= 2;
        break;
    case POWERUP_SLOW_DOWN:
        game->rowAddDelay *= 2;
        break;
    case POWERUP_UNLIMITED_AMMO:
        game->bulletCount = 0;
        break;
    case POWERUP_EXTRA_LIFE:
        if (game->lives < 3) game->lives++;
        duration = 0; // Instant effect
        break;
    case POWERUP_BOMB:
        game->lives--;
        duration = 0; // Instant effect
        break;
    default:
        break;
    }

    // Only add to active effects if it's a temporary powerup
    if (duration > 0) {
        game->activePowerups[game->activeEffectsCount].type = type;
        game->activePowerups[game->activeEffectsCount].duration = duration;
        game->activePowerups[game->activeEffectsCount].active = true;
        game->activeEffectsCount++;
    }

    game->powerupActive = false;
    game->powerupTimer = 7.0f + (rand() % 8);
}

void drawPowerUp(Game* game) {
    if (!game->powerupActive) return;

    Color powerupColor;
    switch (game->currentPowerup.type) {
    case POWERUP_SPEED_UP:
    case POWERUP_UNLIMITED_AMMO:
    case POWERUP_EXTRA_LIFE:
        powerupColor = GREEN;  // Beneficial powerups
        break;
    case POWERUP_RANDOM:
        powerupColor = YELLOW;  // Random effect
        break;
    case POWERUP_SLOW_DOWN:
    case POWERUP_BOMB:
        powerupColor = RED;  // Harmful powerups
        break;
    default:
        powerupColor = WHITE;
        break;
    }

    // Draw powerup as a circle
    DrawCircle((int)game->powerupPosition.x + 16, (int)game->powerupPosition.y + 16, 12, powerupColor
    );
}

void updatePowerUp(Game* game) {
    if (!game->powerupActive) return;

    // Move powerup down slower
    game->powerupPosition.y += 2.0f;  // Reduced from 2.0f

    // Gentler wavy motion
    game->powerupPosition.x += sinf(GetTime() * 2) * 1.0f;  // Reduced amplitude and frequency

    // Keep within screen bounds
    if (game->powerupPosition.x < 0) game->powerupPosition.x = 0;
    if (game->powerupPosition.x > GAME_SCREEN_WIDTH - 32)
        game->powerupPosition.x = GAME_SCREEN_WIDTH - 32;

    // Check if powerup went off screen
    if (game->powerupPosition.y > GAME_SCREEN_HEIGHT) {
        game->powerupActive = false;
        return;
    }

    // Check collision with shooter
    Rectangle powerupRect = { game->powerupPosition.x,game->powerupPosition.y,32,32 };

    Rectangle shooterRect = { P.x - 32,P.y - 32,96,64 };

    if (CheckCollisionRecs(powerupRect, shooterRect)) {
        activatePowerUp(game);
    }
}