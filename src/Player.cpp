/**
 * @file Player.cpp
 * @brief Implementation of Player movement, skin rendering, and intro animation.
 * @author Faliq
 */

#include "Player.hpp"
#include "AssetManager.hpp"
#include "Grid.hpp"
#include <algorithm>

Player::Player() {
    initializePosition();
}

void Player::initializePosition() {
    float blockSize = static_cast<float>(auto_x(32));
    float gameWidth = blockSize * MAX_COLUMNS;

    x = auto_x(192);
    y = auto_y(598);

    if (x < 0) x = 0;
    if (static_cast<float>(x) + blockSize > gameWidth) {
        x = static_cast<int>(gameWidth - blockSize);
    }
}

void Player::updatePositionOnResize() {
    float blockSize = static_cast<float>(auto_x(32));
    float gameWidth = blockSize * MAX_COLUMNS;

    if (blockSize > 0.0f) {
        x = static_cast<int>(static_cast<float>(x) / blockSize) * static_cast<int>(blockSize);
    }

    if (static_cast<float>(x) + blockSize > gameWidth) {
        x = static_cast<int>(gameWidth - blockSize);
    }

    y = auto_y(598);
}

void Player::move(bool left, bool right, const AssetManager& assets, bool sfxEnabled) {
    float blockSize = static_cast<float>(auto_x(32));
    float gameWidth = blockSize * MAX_COLUMNS;
    int step = auto_x(32);

    if (left && x > 0) {
        x -= step;
        if (sfxEnabled) {
            PlaySound(assets.getSound(SoundAsset::Move));
        }
    } else if (right && (static_cast<float>(x) + blockSize < gameWidth)) {
        x += step;
        if (sfxEnabled) {
            PlaySound(assets.getSound(SoundAsset::Move));
        }
    }

    if (x < 0) x = 0;
    if (static_cast<float>(x) + blockSize > gameWidth) {
        x = static_cast<int>(gameWidth - blockSize);
    }
}

void Player::triggerLaser() {
    if (laserCooldown <= 0.0f) {
        laserActive = true;
        laserDuration = 5.0f;
        laserCooldown = 15.0f;
    }
}

void Player::updateLaser(float dt) {
    if (laserActive) {
        laserDuration -= dt;
        if (laserDuration <= 0.0f) {
            laserActive = false;
        }
    }

    if (laserCooldown > 0.0f) {
        laserCooldown -= dt;
        if (laserCooldown < 0.0f) laserCooldown = 0.0f;
    }
}

void Player::draw(const AssetManager& assets, uint skin) const {
    float blockSize = static_cast<float>(auto_x(32));
    float posX = static_cast<float>(x);
    float posY = static_cast<float>(y);
    float gameWidth = blockSize * MAX_COLUMNS;

    const Texture2D& shooterM = assets.getShooterPart(skin, ShooterSkinPart::Mid);
    const Texture2D& shooterT = assets.getShooterPart(skin, ShooterSkinPart::Top);
    const Texture2D& shooterR = assets.getShooterPart(skin, ShooterSkinPart::Right);
    const Texture2D& shooterL = assets.getShooterPart(skin, ShooterSkinPart::Left);

    float imgScale = (shooterM.width > 0) ? (blockSize / static_cast<float>(shooterM.width)) : 1.0f;

    if (posX + blockSize >= gameWidth) {
        DrawTextureEx(shooterM, { posX, posY }, 0.0f, imgScale, WHITE);
        DrawTextureEx(shooterT, { posX, posY - blockSize }, 0.0f, imgScale, WHITE);
        DrawTextureEx(shooterL, { posX - blockSize, posY }, 0.0f, imgScale, WHITE);
    } else {
        DrawTextureEx(shooterL, { posX - blockSize, posY }, 0.0f, imgScale, WHITE);
        DrawTextureEx(shooterM, { posX, posY }, 0.0f, imgScale, WHITE);
        DrawTextureEx(shooterT, { posX, posY - blockSize }, 0.0f, imgScale, WHITE);
        DrawTextureEx(shooterR, { posX + blockSize, posY }, 0.0f, imgScale, WHITE);
    }
}

void Player::drawLaser(const Grid& grid) const {
    if (!laserActive) return;

    float blockSize = static_cast<float>(auto_x(32));
    if (blockSize <= 0.0f) return;

    int gridX = static_cast<int>(static_cast<float>(x) / blockSize);
    float intersectionY = static_cast<float>(y);

    if (gridX >= 0 && gridX < MAX_COLUMNS) {
        for (int r = MAX_ROWS - 1; r >= 0; --r) {
            const Block* b = grid.getBlockAt(r, gridX);
            if (b && b->active) {
                intersectionY = static_cast<float>(r) * blockSize;
                break;
            }
        }
    }

    float laserX = static_cast<float>(x) + (blockSize / 2.0f);
    float laserThickness = static_cast<float>(auto_x(2.0f));
    float dotRadius = static_cast<float>(auto_x(3.0f));
    intersectionY += blockSize;

    DrawLineEx({ laserX, static_cast<float>(y) }, { laserX, intersectionY }, laserThickness, Color{ 255, 0, 0, 128 });
    DrawCircle(static_cast<int>(laserX), static_cast<int>(intersectionY), dotRadius, RED);
}

Color fadeInOpeningAnimation(float* trans) {
    if (*trans < 1.0f) {
        *trans += 0.005f;
    }
    int val = static_cast<int>(std::clamp(*trans, 0.0f, 1.0f) * 255.0f);
    return Color{ static_cast<unsigned char>(val), static_cast<unsigned char>(val), static_cast<unsigned char>(val), 255 };
}

Color fadeOutOpeningAnimation(float* trans) {
    if (*trans > 0.0f) {
        *trans -= 0.005f;
        if (*trans < 0.0f) *trans = 0.0f;
    }
    int val = static_cast<int>(std::clamp(*trans, 0.0f, 1.0f) * 255.0f);
    return Color{ static_cast<unsigned char>(val), static_cast<unsigned char>(val), static_cast<unsigned char>(val), 255 };
}

void openingAnimation(float* trans, const AssetManager& assets) {
    const Texture2D& icon = assets.getBg(BgTexture::IconLoading);
    if (icon.height == 0) return;

    float imgScale = static_cast<float>(GetScreenHeight()) / static_cast<float>(icon.height);
    float scaledWidth = static_cast<float>(icon.width) * imgScale;
    float iconX = (static_cast<float>(GetScreenWidth()) - scaledWidth) / 2.0f;

    while (*trans < 1.0f && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(fadeInOpeningAnimation(trans));
        unsigned char alpha = static_cast<unsigned char>(std::clamp(*trans, 0.0f, 1.0f) * 255.0f);
        DrawTextureEx(icon, { iconX, 0.0f }, 0.0f, imgScale, Color{ 255, 255, 255, alpha });
        EndDrawing();
    }

    BeginDrawing();
    ClearBackground(WHITE);
    DrawTextureEx(icon, { iconX, 0.0f }, 0.0f, imgScale, WHITE);
    EndDrawing();

    // Tunggu sejenak di layar penuh
    float holdTimer = 1.0f;
    while (holdTimer > 0.0f && !WindowShouldClose()) {
        holdTimer -= GetFrameTime();
        BeginDrawing();
        ClearBackground(WHITE);
        DrawTextureEx(icon, { iconX, 0.0f }, 0.0f, imgScale, WHITE);
        EndDrawing();
    }

    while (*trans > 0.0f && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(fadeOutOpeningAnimation(trans));
        unsigned char alpha = static_cast<unsigned char>(std::clamp(*trans, 0.0f, 1.0f) * 255.0f);
        DrawTextureEx(icon, { iconX, 0.0f }, 0.0f, imgScale, Color{ 255, 255, 255, alpha });
        EndDrawing();
    }
}
