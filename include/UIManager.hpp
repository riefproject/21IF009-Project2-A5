#pragma once

/**
 * @file UIManager.hpp
 * @brief User interface layout, text rendering, dialogs, transitions, and HUD drawing.
 * @author Arief
 *
 * @details Handles UI drawing routines: responsive text rendering, menu navigation,
 * setting toggles, sliding transition animations for level select and skin picker,
 * countdown timers, game-over summaries, and in-game HUD displays.
 */

#include "Constants.hpp"
#include "AssetTypes.hpp"
#include "Scale.hpp"

class AssetManager;
class ScoreManager;
class Game;

// Text and menu rendering helpers
void drawCenteredText(Font font, const char* text, float y, int fontSize, float spacing, Color color);
void drawMenuOption(Font font, const char* text, float y, int fontSize, bool selected, Color color);
void drawToggleOption(Font font, const char* line, int startX, int y, int maxLabelWidth, int fontSize, float spacing, bool isSelected, bool isEditing, Color normalColor);
void drawLabelsAndValues(const char* lines[], int lineCount, int startX, int startY, int maxLabelWidth, int fontSize, float spacing, Font font, Color color);
void drawCenteredUIText(const char* text, float y, int fontSize, float uiAreaX, float uiAreaWidth, Color color);

// Dimensions calculation helpers
float calculateTotalTextHeight(Font font, const char* lines[], int lineCount, int fontSize, float spacing);
void calculateMaxWidths(const char* lines[], int lineCount, int fontSize, float spacing, int* maxLabelWidth, int* maxValueWidth, Font font);
int calculateTotalHeight(int lineCount, int fontSize, float spacing);

// Menu navigation and drawing
int handleMenuNavigation(int currentSelection, int maxOptions, const AssetManager& assets, bool sfxEnabled);
void drawMenu(const AssetManager& assets, const char* lines[], int lineCount, int selection, int fontSize, Color highlightColor);

// Dialogs
bool showConfirmationDialog(const AssetManager& assets, bool sfxEnabled, const char* message, const char* options[], int optionCount, Color highlightColor);
void showMessageDialog(const AssetManager& assets, const char* message, const char* subMessage, Color messageColor, Color subMessageColor);

// Slide transitions
void updateTransition(float* transition, int* current, int target, int* direction, float deltaTime, float speed);
void renderModeTransition(const AssetManager& assets, int current, int target, float transition, int direction);
void renderSkinTransition(const AssetManager& assets, int current, int target, float transition, int direction);

// Loading & countdowns
int loadingScreen(const AssetManager& assets, float* loadingTime);
void showCountdown(const AssetManager& assets);
bool drawCountdownTimer(const AssetManager& assets, float* countdown, int y);

// HUD & gameplay UI
void drawBG(const AssetManager& assets, BgTexture id);
void drawGameOverScore(const AssetManager& assets, ll currentScore, ll currentHighScore, int* startY);
void drawPowerUpIcon(Texture2D iconTexture, Vector2 position, float iconSize, float duration, Color timerColor);
void drawLives(const AssetManager& assets, int lives, Vector2 startPos, float spacing);
void drawLaserButton(const AssetManager& assets, float cooldown, float uiAreaX, float uiAreaWidth, float y);
void drawGameUI(const Game& game, const AssetManager& assets, const ScoreManager& scoreMgr);
