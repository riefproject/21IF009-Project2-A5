#pragma once

/**
 * @file Scale.hpp
 * @brief Dynamic resolution scaling and aspect-ratio helper functions.
 * @author Arief
 *
 * @details Provides mathematical scaling factors to adapt 2D game objects
 * and graphical UI elements dynamically to any window resolution while maintaining
 * the target 3:4 aspect ratio.
 */

#include "Constants.hpp"
#include "raylib.h"

constexpr int VIRTUAL_SCREEN_WIDTH = 480;
constexpr int VIRTUAL_SCREEN_HEIGHT = 640;

/**
 * @struct ScaleFactor
 * @brief Scale multipliers for horizontal and vertical axes.
 */
struct ScaleFactor {
    float x{1.0f};
    float y{1.0f};
};

/**
 * @brief Initializes the virtual 480x640 render target for resolution-independent rendering.
 */
void InitVirtualCanvas(int virtualWidth = VIRTUAL_SCREEN_WIDTH, int virtualHeight = VIRTUAL_SCREEN_HEIGHT);

/**
 * @brief Deallocates the virtual canvas render target on shutdown.
 */
void CloseVirtualCanvas();

/**
 * @brief Begins drawing directly to the virtual 3:4 canvas.
 */
void BeginVirtualCanvas();

/**
 * @brief Finalizes virtual canvas drawing and renders to display monitor with letterboxing/pillarboxing.
 */
void EndVirtualCanvas();

/**
 * @brief Computes scale factor (constant 1.0 inside virtual canvas).
 */
inline ScaleFactor GetScreenScaleFactor() {
    return ScaleFactor{ 1.0f, 1.0f };
}

/**
 * @brief Adjusts target window size to enforce 3:4 aspect ratio constraints.
 */
void GetAdjustedWindowSize(int width, int height, int* outWidth, int* outHeight);

inline int auto_x(float var) {
    return static_cast<int>(var);
}

inline int auto_y(float var) {
    return static_cast<int>(var);
}
