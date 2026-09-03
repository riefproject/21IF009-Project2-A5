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
    gridColumn = 6;
    x = static_cast<int>(gridColumn * blockSize);
    y = auto_y(598);
}

void Player::updatePositionOnResize() {
    float blockSize = static_cast<float>(auto_x(32));
    if (gridColumn < 0) gridColumn = 0;
    if (gridColumn >= MAX_COLUMNS) gridColumn = MAX_COLUMNS - 1;

    x = static_cast<int>(gridColumn * blockSize);
    y = auto_y(598);
}

void Player::move(bool left, bool right, const AssetManager& assets, bool sfxEnabled) {
    float blockSize = static_cast<float>(auto_x(32));

    if (left && gridColumn > 0) {
        gridColumn--;
        x = static_cast<int>(gridColumn * blockSize);
        if (sfxEnabled) {
            PlaySound(assets.getSound(SoundAsset::Move));
        }
    } else if (right && (gridColumn < MAX_COLUMNS - 1)) {
        gridColumn++;
        x = static_cast<int>(gridColumn * blockSize);
        if (sfxEnabled) {
            PlaySound(assets.getSound(SoundAsset::Move));
        }
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
    float intersectionY = 0.0f;
    bool hitBlock = false;

    if (gridX >= 0 && gridX < MAX_COLUMNS) {
        for (int r = MAX_ROWS - 1; r >= 0; --r) {
            if (grid.isBlockActive(r, gridX)) {
                intersectionY = static_cast<float>(r) * blockSize + blockSize;
                hitBlock = true;
                break;
            }
        }
    }

    float laserX = static_cast<float>(x) + (blockSize / 2.0f);
    float laserThickness = static_cast<float>(auto_x(2.0f));
    float dotRadius = static_cast<float>(auto_x(3.0f));

    DrawLineEx({ laserX, static_cast<float>(y) }, { laserX, intersectionY }, laserThickness, Color{ 255, 0, 0, 128 });
    if (hitBlock) {
        DrawCircle(static_cast<int>(laserX), static_cast<int>(intersectionY), dotRadius, RED);
    }
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

    float imgScale = static_cast<float>(VIRTUAL_SCREEN_HEIGHT) / static_cast<float>(icon.height);
    float scaledWidth = static_cast<float>(icon.width) * imgScale;
    float iconX = (static_cast<float>(VIRTUAL_SCREEN_WIDTH) - scaledWidth) / 2.0f;

    while (*trans < 1.0f && !WindowShouldClose()) {
        BeginVirtualCanvas();
        ClearBackground(fadeInOpeningAnimation(trans));
        unsigned char alpha = static_cast<unsigned char>(std::clamp(*trans, 0.0f, 1.0f) * 255.0f);
        DrawTextureEx(icon, { iconX, 0.0f }, 0.0f, imgScale, Color{ 255, 255, 255, alpha });
        EndVirtualCanvas();
    }

    BeginVirtualCanvas();
    ClearBackground(WHITE);
    DrawTextureEx(icon, { iconX, 0.0f }, 0.0f, imgScale, WHITE);
    EndVirtualCanvas();

    // Tunggu sejenak di layar penuh
    float holdTimer = 1.0f;
    while (holdTimer > 0.0f && !WindowShouldClose()) {
        holdTimer -= GetFrameTime();
        BeginVirtualCanvas();
        ClearBackground(WHITE);
        DrawTextureEx(icon, { iconX, 0.0f }, 0.0f, imgScale, WHITE);
        EndVirtualCanvas();
    }

    while (*trans > 0.0f && !WindowShouldClose()) {
        BeginVirtualCanvas();
        ClearBackground(fadeOutOpeningAnimation(trans));
        unsigned char alpha = static_cast<unsigned char>(std::clamp(*trans, 0.0f, 1.0f) * 255.0f);
        DrawTextureEx(icon, { iconX, 0.0f }, 0.0f, imgScale, Color{ 255, 255, 255, alpha });
        EndVirtualCanvas();
    }
}
