#include "defines.h"
#include "all.h"

void spawnPowerUp(Game* game) {
    game->powerupActive = true;
    game->currentPowerup.type = rand() % POWERUP_COUNT;
    game->powerupPosition = (Vector2){
        (float)(rand() % (320 - 32)), // Using 320
        -32.0f  // Start above screen
    };
}

void activatePowerUp(Game* game, GameResources* resources) {
    if (game->activeEffectsCount >= 3) return; // Maximum 3 active effects

    PowerUpType type = game->currentPowerup.type;
    if (type == POWERUP_RANDOM) {
        // Keep randomizing until we get a non-RANDOM powerup
        do {
            type = rand() % POWERUP_COUNT;
        } while (type == POWERUP_RANDOM);
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
        game->rowAddDelay = (int)(game->rowAddDelay * 0.25f);
        if (game->rowAddDelay < 10) game->rowAddDelay = 10;
        PlaySound(SOUND(resources, SOUND_SPEEDUP));
        break;
    case POWERUP_SLOW_DOWN:
        game->rowAddDelay = (int)(game->rowAddDelay * 2.5f);
        if (game->rowAddDelay > 240) game->rowAddDelay = 240;
        PlaySound(SOUND(resources, SOUND_SLOWDOWN));
        break;
    case POWERUP_SPECIAL_BULLET:
        game->bulletCount = 0;
        PlaySound(SOUND(resources, SOUND_SPECIAL_BULLET));
        break;
    case POWERUP_EXTRA_LIFE:
        if (game->lives < 3) {
            game->lives++;
            PlaySound(SOUND(resources, SOUND_HEAL));
        }
        duration = 0; // Instant effect
        break;
    case POWERUP_BOMB:
        game->lives--;
        PlaySound(SOUND(resources, SOUND_POISON));
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

void drawPowerUp(Game* game, GameResources* resources) {
    if (!game->powerupActive) return;

    Texture2D powerupTexture;
    float local_scale;


    switch (game->currentPowerup.type) {
    case POWERUP_SPEED_UP:
        powerupTexture = TEXTURE(resources, TEXTURE_SPEEDUP);
        local_scale = 40.0f / 640.0f;
        break;
    case POWERUP_SLOW_DOWN:
        powerupTexture = TEXTURE(resources, TEXTURE_SLOWDOWN);
        local_scale = 40.0f / 1024.0f;
        break;
    case POWERUP_SPECIAL_BULLET:  // Efek temporal
        powerupTexture = TEXTURE(resources, TEXTURE_SPECIAL_BULLET);
        game->bulletCount = 0;
        local_scale = 40.0f / 1024.0f;
        break;
    case POWERUP_EXTRA_LIFE:      // Efek instan
        powerupTexture = TEXTURE(resources, TEXTURE_PLS1_HP);
        local_scale = 40.0f / 672.0f;
        break;
    case POWERUP_BOMB:            // Efek instan
        powerupTexture = TEXTURE(resources, TEXTURE_MIN1_HP);
        local_scale = 40.0f / 762.0f;
        break;
    case POWERUP_RANDOM:      // Efek instan
        powerupTexture = TEXTURE(resources, TEXTURE_RANDOM);
        local_scale = 40.0f / 1024.0f;
        break;
    default:
        powerupTexture = TEXTURE(resources, TEXTURE_RANDOM);
        local_scale = 40.0f / 1024.0f;
        break;
    }

    DrawTextureEx(powerupTexture,
        game->powerupPosition,
        0,
        local_scale,
        WHITE);
}

void updatePowerUp(Game* game, GameResources* resources) {
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
    Rectangle powerupRect = { game->powerupPosition.x,game->powerupPosition.y,40,40 };

    Rectangle shooterRect = { P.x - 32,P.y - 32,96,64 };

    if (CheckCollisionRecs(powerupRect, shooterRect)) {
        activatePowerUp(game, resources);
    }
}
