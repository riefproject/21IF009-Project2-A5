/**
 * @file UIManager.cpp
 * @brief Implementation of UI text layout, dialogs, transitions, and HUD drawing.
 * @author Arief
 */

#include "UIManager.hpp"
#include "Input.hpp"
#include "AssetManager.hpp"
#include "ScoreManager.hpp"
#include "Game.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <algorithm>

void drawCenteredText(Font font, const char* text, float y, int fontSize, float spacing, Color color) {
    Vector2 textSize = MeasureTextEx(font, text, static_cast<float>(fontSize), spacing);
    float x = (static_cast<float>(VIRTUAL_SCREEN_WIDTH) - textSize.x) / 2.0f;
    DrawTextEx(font, text, { x, y }, static_cast<float>(fontSize), spacing, color);
}

void drawMenuOption(Font font, const char* text, float y, int fontSize, bool selected, Color color) {
    drawCenteredText(font, text, y, fontSize, 2.0f, selected ? color : RAYWHITE);
}

void drawToggleOption(Font font, const char* line, int startX, int y, int maxLabelWidth, int fontSize, float spacing, bool isSelected, bool isEditing, Color normalColor) {
    if (!line) return;
    const char* colon = std::strchr(line, ':');
    if (!colon) return;

    size_t colonOffset = colon - line;
    char label[128];
    size_t labelLen = std::min(colonOffset, sizeof(label) - 1);
    std::memcpy(label, line, labelLen);
    label[labelLen] = '\0';

    const char* val = colon + 1;

    Color labelColor = isSelected ? (isEditing ? GREEN : ORANGE) : normalColor;
    Color valColor   = isSelected ? (isEditing ? GREEN : ORANGE) : normalColor;

    DrawTextEx(font, label, { static_cast<float>(startX), static_cast<float>(y) }, static_cast<float>(fontSize), spacing, labelColor);

    float colonX = static_cast<float>(startX + maxLabelWidth + auto_x(10));
    DrawTextEx(font, ":", { colonX, static_cast<float>(y) }, static_cast<float>(fontSize), spacing, normalColor);

    float valX = colonX + MeasureTextEx(font, ":", static_cast<float>(fontSize), spacing).x + static_cast<float>(auto_x(15));
    DrawTextEx(font, val, { valX, static_cast<float>(y) }, static_cast<float>(fontSize), spacing, valColor);
}

void drawLabelsAndValues(const char* lines[], int lineCount, int startX, int startY, int maxLabelWidth, int fontSize, float spacing, Font font, Color color) {
    int y = startY;
    for (int i = 0; i < lineCount; ++i) {
        if (!lines[i]) continue;
        const char* colon = std::strchr(lines[i], ':');
        if (colon) {
            size_t colonOffset = colon - lines[i];
            char label[128];
            size_t labelLen = std::min(colonOffset, sizeof(label) - 1);
            std::memcpy(label, lines[i], labelLen);
            label[labelLen] = '\0';

            const char* val = colon + 1;

            DrawTextEx(font, label, { static_cast<float>(startX), static_cast<float>(y) }, static_cast<float>(fontSize), spacing, color);

            float colonX = static_cast<float>(startX + maxLabelWidth + auto_x(10));
            DrawTextEx(font, ":", { colonX, static_cast<float>(y) }, static_cast<float>(fontSize), spacing, color);

            float valX = colonX + MeasureTextEx(font, ":", static_cast<float>(fontSize), spacing).x + static_cast<float>(auto_x(10));
            DrawTextEx(font, val, { valX, static_cast<float>(y) }, static_cast<float>(fontSize), spacing, color);
        } else {
            DrawTextEx(font, lines[i], { static_cast<float>(startX), static_cast<float>(y) }, static_cast<float>(fontSize), spacing, color);
        }
        y += fontSize + auto_y(8);
    }
}

void drawCenteredUIText(const char* text, float y, int fontSize, float uiAreaX, float uiAreaWidth, Color color) {
    Font defaultFont = GetFontDefault();
    Vector2 textSize = MeasureTextEx(defaultFont, text, static_cast<float>(fontSize), 2.0f);
    float x = uiAreaX + (uiAreaWidth - textSize.x) / 2.0f;
    DrawTextEx(defaultFont, text, { x, y }, static_cast<float>(fontSize), 2.0f, color);
}

float calculateTotalTextHeight(Font font, const char* lines[], int lineCount, int fontSize, float spacing) {
    float total = 0.0f;
    for (int i = 0; i < lineCount; ++i) {
        Vector2 size = MeasureTextEx(font, lines[i], static_cast<float>(fontSize), spacing);
        total += size.y;
    }
    return total;
}

void calculateMaxWidths(const char* lines[], int lineCount, int fontSize, float spacing, int* maxLabelWidth, int* maxValueWidth, Font font) {
    *maxLabelWidth = 0;
    *maxValueWidth = 0;

    for (int i = 0; i < lineCount; ++i) {
        if (!lines[i]) continue;
        const char* colon = std::strchr(lines[i], ':');
        if (colon) {
            size_t colonOffset = colon - lines[i];
            char label[128];
            size_t labelLen = std::min(colonOffset, sizeof(label) - 1);
            std::memcpy(label, lines[i], labelLen);
            label[labelLen] = '\0';

            const char* val = colon + 1;

            Vector2 lSize = MeasureTextEx(font, label, static_cast<float>(fontSize), spacing);
            Vector2 vSize = MeasureTextEx(font, val, static_cast<float>(fontSize), spacing);

            if (static_cast<int>(lSize.x) > *maxLabelWidth) *maxLabelWidth = static_cast<int>(lSize.x);
            if (static_cast<int>(vSize.x) > *maxValueWidth) *maxValueWidth = static_cast<int>(vSize.x);
        } else {
            Vector2 size = MeasureTextEx(font, lines[i], static_cast<float>(fontSize), spacing);
            if (static_cast<int>(size.x) > *maxLabelWidth) *maxLabelWidth = static_cast<int>(size.x);
        }
    }
}

int calculateTotalHeight(int lineCount, int fontSize, float spacing) {
    return static_cast<int>(lineCount * fontSize + (lineCount - 1) * spacing);
}

int handleMenuNavigation(int currentSelection, int maxOptions, const AssetManager& assets, bool sfxEnabled) {
    if (isMoveUp()) {
        if (sfxEnabled) PlaySound(assets.getSound(SoundAsset::Move));
        currentSelection--;
        if (currentSelection < 0) currentSelection = maxOptions - 1;
    }
    if (isMoveDown()) {
        if (sfxEnabled) PlaySound(assets.getSound(SoundAsset::Move));
        currentSelection++;
        if (currentSelection >= maxOptions) currentSelection = 0;
    }
    return currentSelection;
}

void drawMenu(const AssetManager& assets, const char* lines[], int lineCount, int selection, int fontSize, Color highlightColor) {
    int totalHeight = calculateTotalHeight(lineCount, fontSize, static_cast<float>(auto_y(25)));
    int startY = (VIRTUAL_SCREEN_HEIGHT - totalHeight) / 2;

    Font bodyFont = assets.getFont(FontAsset::Body);
    for (int i = 0; i < lineCount; ++i) {
        drawCenteredText(bodyFont, lines[i], static_cast<float>(startY), fontSize, 2.0f, (selection == i) ? highlightColor : RAYWHITE);
        startY += fontSize + auto_y(25);
    }
}

bool showConfirmationDialog(const AssetManager& assets, bool sfxEnabled, const char* message, const char* options[], int optionCount, Color highlightColor) {
    int selection = 1; // Default 'No'
    int fontSize = auto_y(20);

    while (!WindowShouldClose()) {
        BeginVirtualCanvas();
        ClearBackground(Fade(PRIMARY_COLOR, 0.95f));
        drawBG(assets, BgTexture::Confirm);

        drawCenteredText(assets.getFont(FontAsset::Header), message, static_cast<float>(VIRTUAL_SCREEN_HEIGHT / 2 - auto_y(80)), auto_y(24), 2.0f, RAYWHITE);
        drawMenu(assets, options, optionCount, selection, fontSize, highlightColor);
        EndVirtualCanvas();

        selection = handleMenuNavigation(selection, optionCount, assets, sfxEnabled);

        if (isOkPressed()) {
            if (sfxEnabled) PlaySound(assets.getSound(SoundAsset::Select));
            return (selection == 0);
        }
        if (isBackPressed()) {
            if (sfxEnabled) PlaySound(assets.getSound(SoundAsset::Move));
            return false;
        }
    }
    return false;
}

void showMessageDialog(const AssetManager& assets, const char* message, const char* subMessage, Color messageColor, Color subMessageColor) {
    float holdTime = 1.5f;
    while (holdTime > 0.0f && !WindowShouldClose()) {
        holdTime -= GetFrameTime();
        BeginVirtualCanvas();
        ClearBackground(PRIMARY_COLOR);
        drawBG(assets, BgTexture::Confirm);

        drawCenteredText(assets.getFont(FontAsset::Header), message, static_cast<float>(VIRTUAL_SCREEN_HEIGHT / 2 - auto_y(30)), auto_y(24), 2.0f, messageColor);
        if (subMessage && subMessage[0] != '\0') {
            drawCenteredText(assets.getFont(FontAsset::Body), subMessage, static_cast<float>(VIRTUAL_SCREEN_HEIGHT / 2 + auto_y(20)), auto_y(18), 2.0f, subMessageColor);
        }
        EndVirtualCanvas();
    }
}

void updateTransition(float* transition, int* current, int target, int* direction, float deltaTime, float speed) {
    if (*direction != 0) {
        *transition += deltaTime * speed;
        if (*transition >= 1.0f) {
            *current = target;
            *transition = 0.0f;
            *direction = 0;
        }
    }
}

void renderModeTransition(const AssetManager& assets, int current, int target, float transition, int direction) {
    auto curMode = static_cast<BgModeTexture>(current);
    auto tgtMode = static_cast<BgModeTexture>(target);

    const Texture2D& curBg = assets.getBgMode(curMode);
    if (curBg.height == 0) return;

    float imgScale = static_cast<float>(VIRTUAL_SCREEN_HEIGHT) / static_cast<float>(curBg.height);
    float scaledWidth = static_cast<float>(curBg.width) * imgScale;
    float baseXPos = (static_cast<float>(VIRTUAL_SCREEN_WIDTH) - scaledWidth) / 2.0f;

    float currentXPos = baseXPos;
    float nextXPos = baseXPos;

    if (direction != 0) {
        currentXPos += (static_cast<float>(direction) * static_cast<float>(VIRTUAL_SCREEN_WIDTH) * -transition);
        nextXPos += (static_cast<float>(direction) * static_cast<float>(VIRTUAL_SCREEN_WIDTH) * (1.0f - transition));

        const Texture2D& tgtBg = assets.getBgMode(tgtMode);
        DrawTextureEx(tgtBg, { nextXPos, 0.0f }, 0.0f, imgScale, WHITE);
    }

    DrawTextureEx(curBg, { currentXPos, 0.0f }, 0.0f, imgScale, WHITE);

    // Overlay teks mode
    const Texture2D& txMode = assets.getTxMode(curMode);
    if (txMode.height > 0) {
        float txScale = static_cast<float>(VIRTUAL_SCREEN_HEIGHT) / static_cast<float>(txMode.height);
        float txWidth = static_cast<float>(txMode.width) * txScale;
        float txBaseX = (static_cast<float>(VIRTUAL_SCREEN_WIDTH) - txWidth) / 2.0f;
        DrawTextureEx(txMode, { txBaseX, 0.0f }, 0.0f, txScale, WHITE);
    }
}

void renderSkinTransition(const AssetManager& assets, int current, int target, float transition, int direction) {
    auto curTexId = (current == 0) ? TextureAsset::Skin1 : TextureAsset::Skin2;
    auto tgtTexId = (target == 0) ? TextureAsset::Skin1 : TextureAsset::Skin2;

    const Texture2D& curSkin = assets.getTexture(curTexId);
    if (curSkin.width == 0) return;

    float skinScale = 120.0f / static_cast<float>(curSkin.width);
    float centerX = (static_cast<float>(VIRTUAL_SCREEN_WIDTH) - static_cast<float>(curSkin.width) * skinScale) / 2.0f;
    float centerY = (static_cast<float>(VIRTUAL_SCREEN_HEIGHT) - static_cast<float>(curSkin.height) * skinScale) / 2.0f;

    float currentSkinY = centerY + static_cast<float>(auto_y(120));
    float baseSkinX = centerX;

    if (direction != 0) {
        float curOffset = static_cast<float>(direction) * static_cast<float>(VIRTUAL_SCREEN_WIDTH) * transition;
        float tgtOffset = static_cast<float>(direction) * static_cast<float>(VIRTUAL_SCREEN_WIDTH) * (transition - 1.0f);

        DrawTextureEx(curSkin, { baseSkinX + curOffset, currentSkinY }, 0.0f, skinScale, Fade(WHITE, 1.0f - transition));

        const Texture2D& tgtSkin = assets.getTexture(tgtTexId);
        float tgtScale = 120.0f / static_cast<float>(tgtSkin.width);
        DrawTextureEx(tgtSkin, { baseSkinX + tgtOffset, currentSkinY }, 0.0f, tgtScale, Fade(WHITE, transition));
    } else {
        DrawTextureEx(curSkin, { baseSkinX, currentSkinY }, 0.0f, skinScale, WHITE);
    }
}

int loadingScreen(const AssetManager& assets, float* loadingTime) {
    const int blockSize = auto_x(50);
    const float stepDuration = 0.25f;
    constexpr int totalSteps = 9;
    const float cycleDuration = stepDuration * totalSteps;

    *loadingTime += GetFrameTime();
    if (*loadingTime >= LOADING_TIME) return 1;

    Vector2 steps[9][4] = {
        { {static_cast<float>(auto_x(190)), static_cast<float>(auto_y(265))}, {static_cast<float>(auto_x(240)), static_cast<float>(auto_y(265))}, {static_cast<float>(auto_x(190)), static_cast<float>(auto_y(315))}, {static_cast<float>(auto_x(240)), static_cast<float>(auto_y(315))} },
        { {static_cast<float>(auto_x(140)), static_cast<float>(auto_y(265))}, {static_cast<float>(auto_x(290)), static_cast<float>(auto_y(265))}, {static_cast<float>(auto_x(190)), static_cast<float>(auto_y(315))}, {static_cast<float>(auto_x(240)), static_cast<float>(auto_y(315))} },
        { {static_cast<float>(auto_x(140)), static_cast<float>(auto_y(315))}, {static_cast<float>(auto_x(290)), static_cast<float>(auto_y(315))}, {static_cast<float>(auto_x(190)), static_cast<float>(auto_y(315))}, {static_cast<float>(auto_x(240)), static_cast<float>(auto_y(315))} },
        { {static_cast<float>(auto_x(140)), static_cast<float>(auto_y(315))}, {static_cast<float>(auto_x(290)), static_cast<float>(auto_y(315))}, {static_cast<float>(auto_x(190)), static_cast<float>(auto_y(265))}, {static_cast<float>(auto_x(240)), static_cast<float>(auto_y(265))} },
        { {static_cast<float>(auto_x(190)), static_cast<float>(auto_y(315))}, {static_cast<float>(auto_x(240)), static_cast<float>(auto_y(315))}, {static_cast<float>(auto_x(190)), static_cast<float>(auto_y(265))}, {static_cast<float>(auto_x(240)), static_cast<float>(auto_y(265))} },
        { {static_cast<float>(auto_x(190)), static_cast<float>(auto_y(315))}, {static_cast<float>(auto_x(240)), static_cast<float>(auto_y(315))}, {static_cast<float>(auto_x(140)), static_cast<float>(auto_y(265))}, {static_cast<float>(auto_x(290)), static_cast<float>(auto_y(265))} },
        { {static_cast<float>(auto_x(190)), static_cast<float>(auto_y(315))}, {static_cast<float>(auto_x(240)), static_cast<float>(auto_y(315))}, {static_cast<float>(auto_x(140)), static_cast<float>(auto_y(315))}, {static_cast<float>(auto_x(290)), static_cast<float>(auto_y(315))} },
        { {static_cast<float>(auto_x(190)), static_cast<float>(auto_y(265))}, {static_cast<float>(auto_x(240)), static_cast<float>(auto_y(265))}, {static_cast<float>(auto_x(140)), static_cast<float>(auto_y(315))}, {static_cast<float>(auto_x(290)), static_cast<float>(auto_y(315))} },
        { {static_cast<float>(auto_x(190)), static_cast<float>(auto_y(265))}, {static_cast<float>(auto_x(240)), static_cast<float>(auto_y(265))}, {static_cast<float>(auto_x(190)), static_cast<float>(auto_y(315))}, {static_cast<float>(auto_x(240)), static_cast<float>(auto_y(315))} }
    };

    float elapsed = std::fmod(static_cast<float>(GetTime()), cycleDuration);
    int currentStep = static_cast<int>(elapsed / stepDuration);
    int nextStep = (currentStep + 1) % totalSteps;
    float alpha = (elapsed - (static_cast<float>(currentStep) * stepDuration)) / stepDuration;

    BeginVirtualCanvas();
    ClearBackground(PRIMARY_COLOR);
    drawBG(assets, BgTexture::Plain);

    for (int i = 0; i < 4; ++i) {
        Vector2 curPos = steps[currentStep][i];
        Vector2 nxtPos = steps[nextStep][i];
        Vector2 smoothPos = {
            curPos.x + (nxtPos.x - curPos.x) * alpha,
            curPos.y + (nxtPos.y - curPos.y) * alpha
        };
        DrawRectangleV(smoothPos, { static_cast<float>(blockSize), static_cast<float>(blockSize) }, ORANGE);
    }
    EndVirtualCanvas();

    return 0;
}

void showCountdown(const AssetManager& assets) {
    float counter = 3.0f;
    Font bodyFont = assets.getFont(FontAsset::Body);

    while (counter > 0.0f && !WindowShouldClose()) {
        counter -= GetFrameTime();
        int currentNumber = static_cast<int>(std::ceil(counter));
        if (currentNumber <= 0) currentNumber = 1;

        float timeInSecond = counter - std::floor(counter);
        float scale = 2.5f - (2.5f - 1.0f) * (1.0f - timeInSecond);

        BeginVirtualCanvas();
        ClearBackground(PRIMARY_COLOR);
        drawBG(assets, BgTexture::Plain);

        std::string text = std::to_string(currentNumber);
        Vector2 textSize = MeasureTextEx(bodyFont, text.c_str(), static_cast<float>(auto_y(50)), static_cast<float>(auto_x(2)));
        float baseSize = static_cast<float>(auto_y(50));
        float scaledSize = baseSize * scale;

        Vector2 position = {
            (static_cast<float>(VIRTUAL_SCREEN_WIDTH) - textSize.x * scale) / 2.0f,
            (static_cast<float>(VIRTUAL_SCREEN_HEIGHT) - textSize.y * scale) / 2.0f
        };

        DrawTextEx(bodyFont, text.c_str(), position, scaledSize, static_cast<float>(auto_x(2)) * scale,
            Fade(ORANGE, scale > 1.8f ? 2.0f - (scale / 2.0f) : 1.0f));

        EndVirtualCanvas();
    }
}

bool drawCountdownTimer(const AssetManager& assets, float* countdown, int y) {
    float dt = GetFrameTime();
    *countdown -= dt;

    if (*countdown > 0.0f) {
        char buf[32];
        std::snprintf(buf, sizeof(buf), "%.1f", *countdown);
        drawCenteredText(assets.getFont(FontAsset::Body), buf, static_cast<float>(y), auto_y(20), 2.0f, RED);
        return false;
    }
    return true;
}

void drawBG(const AssetManager& assets, BgTexture id) {
    const Texture2D& bgTex = assets.getBg(id);
    if (bgTex.height == 0) return;

    float imgScale = static_cast<float>(VIRTUAL_SCREEN_HEIGHT) / static_cast<float>(bgTex.height);
    float scaledWidth = static_cast<float>(bgTex.width) * imgScale;
    float xPos = (static_cast<float>(VIRTUAL_SCREEN_WIDTH) - scaledWidth) / 2.0f;

    DrawTextureEx(bgTex, { xPos, 0.0f }, 0.0f, imgScale, WHITE);
}

void drawGameOverScore(const AssetManager& assets, ll currentScore, ll currentHighScore, int* startY) {
    int fontSize = auto_y(20);
    Font bodyFont = assets.getFont(FontAsset::Body);

    std::string highScoreText = "High Score: " + std::to_string(currentHighScore);

    if (currentScore > currentHighScore) {
        const char* newTag = "[NEW]";
        Vector2 newTagSize = MeasureTextEx(bodyFont, newTag, 12.0f, 2.0f);
        Vector2 highScoreSize = MeasureTextEx(bodyFont, highScoreText.c_str(), static_cast<float>(fontSize), 2.0f);

        Rectangle newBox = {
            (static_cast<float>(VIRTUAL_SCREEN_WIDTH) - highScoreSize.x) / 2.0f,
            static_cast<float>(*startY + auto_y(3)),
            newTagSize.x + 10.0f,
            newTagSize.y + static_cast<float>(auto_y(6))
        };
        DrawRectangleRec(newBox, ORANGE);
        drawCenteredText(bodyFont, newTag, newBox.y + 3.0f, 12, 2.0f, WHITE);
        *startY += 25;
    }

    drawCenteredText(bodyFont, highScoreText.c_str(), static_cast<float>(*startY), fontSize, 2.0f, RAYWHITE);
    *startY += 30;

    std::string scoreText = "Score: " + std::to_string(currentScore);
    drawCenteredText(bodyFont, scoreText.c_str(), static_cast<float>(*startY), fontSize, 2.0f, ORANGE);
    *startY += 60;
}

void drawPowerUpIcon(Texture2D iconTexture, Vector2 position, float iconSize, float duration, Color timerColor) {
    if (iconTexture.width == 0) return;

    float scale = iconSize / static_cast<float>(iconTexture.width);
    DrawTextureEx(iconTexture, position, 0.0f, scale, WHITE);

    char timerText[16];
    std::snprintf(timerText, sizeof(timerText), "%.1fs", duration);
    Vector2 timerSize = MeasureTextEx(GetFontDefault(), timerText, 15.0f, 2.0f);
    float timerX = position.x + (iconSize - timerSize.x) / 2.0f;
    float timerY = position.y + iconSize + 5.0f;

    DrawTextEx(GetFontDefault(), timerText, { timerX, timerY }, 15.0f, 2.0f, timerColor);
}

void drawLives(const AssetManager& assets, int lives, Vector2 startPos, float spacing) {
    const Texture2D& heartTex = assets.getTexture(TextureAsset::Heart);
    if (heartTex.width == 0) return;

    float scale = static_cast<float>(auto_x(30)) / static_cast<float>(heartTex.width);

    for (int i = 0; i < lives; ++i) {
        Vector2 pos = { startPos.x + (static_cast<float>(i) * spacing), startPos.y };
        DrawTextureEx(heartTex, pos, 0.0f, scale, WHITE);
    }
}

void drawLaserButton(const AssetManager& assets, float cooldown, float uiAreaX, float uiAreaWidth, float y) {
    const Texture2D& laserTex = assets.getTexture(TextureAsset::LaserButton);
    if (laserTex.width == 0) return;

    float scale = static_cast<float>(auto_x(80)) / static_cast<float>(laserTex.width);
    float imgSize = scale * static_cast<float>(laserTex.width);
    float xPos = uiAreaX + (uiAreaWidth - imgSize) / 2.0f;

    if (cooldown > 0.0f) {
        DrawTextureEx(laserTex, { xPos, y }, 0.0f, scale, Color{ 255, 255, 255, 150 });

        char cooldownText[16];
        std::snprintf(cooldownText, sizeof(cooldownText), "%.1f", cooldown);
        Vector2 textSize = MeasureTextEx(GetFontDefault(), cooldownText, static_cast<float>(auto_x(20)), static_cast<float>(auto_x(2)));

        float textX = xPos + (imgSize - textSize.x) / 2.0f;
        float textY = y + (static_cast<float>(laserTex.height) * scale - textSize.y) / 2.0f;

        DrawTextEx(GetFontDefault(), cooldownText, { textX - static_cast<float>(auto_x(7)), textY - static_cast<float>(auto_y(3)) },
            static_cast<float>(auto_x(20)), static_cast<float>(auto_x(2)), WHITE);
    } else {
        DrawTextureEx(laserTex, { xPos, y }, 0.0f, scale, WHITE);
    }
}

void drawGameUI(const Game& game, const AssetManager& assets, const ScoreManager& scoreMgr) {
    const int ICON_SIZE = auto_x(35);
    const int SPACING = auto_x(10);
    const int START_ICON_X = auto_x(345);

    const Texture2D& uiBg = assets.getBg(BgTexture::UiGame);
    const Texture2D& gameBg = assets.getBg(BgTexture::GameArea);

    float uiScale = (uiBg.height > 0) ? (static_cast<float>(VIRTUAL_SCREEN_HEIGHT) / static_cast<float>(uiBg.height)) : 1.0f;
    float gameScale = (gameBg.height > 0) ? (static_cast<float>(VIRTUAL_SCREEN_HEIGHT) / static_cast<float>(gameBg.height)) : 1.0f;

    float uiscaledWidth = static_cast<float>(uiBg.width) * uiScale;
    float gamescaledWidth = static_cast<float>(gameBg.width) * gameScale;

    // Mode name
    const char* modeName = ScoreManager::getModeName(game.gameLevel);
    drawCenteredUIText(modeName, static_cast<float>(auto_y(107)), auto_x(18), gamescaledWidth, uiscaledWidth, WHITE);

    // High Score
    ll curHighScore = scoreMgr.getHighScoreForMode(game.gameLevel);
    if (game.score > curHighScore) {
        const char* newTag = "[NEW]";
        Vector2 title = MeasureTextEx(GetFontDefault(), "HIGH-SCORE", 20.0f, 2.0f);
        Vector2 tag = MeasureTextEx(GetFontDefault(), newTag, 12.0f, 2.0f);
        Rectangle newBox = {
            static_cast<float>(auto_x(405)) - static_cast<float>(auto_x(title.x / 2.0f)),
            static_cast<float>(auto_y(160)),
            tag.x + static_cast<float>(auto_x(10)),
            tag.y + static_cast<float>(auto_y(6))
        };
        DrawRectangleRec(newBox, ORANGE);
        DrawTextEx(GetFontDefault(), newTag, { newBox.x + 5.0f, newBox.y + static_cast<float>(auto_y(3)) }, static_cast<float>(auto_x(12)), 2.0f, WHITE);
    }

    ll displayedHighScore = std::max(game.score, curHighScore);
    std::string hiStr = std::to_string(displayedHighScore);
    drawCenteredUIText(hiStr.c_str(), static_cast<float>(auto_y(215)), auto_x(18), gamescaledWidth, uiscaledWidth, WHITE);

    // Current score
    std::string scoreStr = std::to_string(game.score);
    drawCenteredUIText(scoreStr.c_str(), static_cast<float>(auto_y(305)), auto_x(18), gamescaledWidth, uiscaledWidth, WHITE);

    // Active power-up icons
    int iconIdx = 0;
    for (const auto& effect : game.powerups.getActiveEffects()) {
        if (effect.active) {
            PowerUpVisuals visuals = PowerUpManager::getVisuals(assets, effect.type);
            int iconX = START_ICON_X + (iconIdx * (ICON_SIZE + SPACING));
            drawPowerUpIcon(visuals.texture, { static_cast<float>(iconX), static_cast<float>(auto_y(380)) },
                static_cast<float>(ICON_SIZE), effect.duration, visuals.timerColor);
            iconIdx++;
        }
    }

    // Lives
    drawLives(assets, game.lives, { static_cast<float>(START_ICON_X), static_cast<float>(auto_y(482)) }, static_cast<float>(auto_x(35)));

    // Laser button
    drawLaserButton(assets, game.player.laserCooldown, gamescaledWidth, uiscaledWidth, static_cast<float>(auto_y(542)));
}
