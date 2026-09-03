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

#include "Defines.hpp"

/**
 * @struct ScaleFactor
 * @brief Scale multipliers for horizontal and vertical axes.
 */
struct ScaleFactor {
    float x{1.0f};
    float y{1.0f};
};

/**
 * @brief Computes the active scale factor comparing current window size to base resolution.
 * @return ScaleFactor containing uniform scaling components.
 */
ScaleFactor GetScreenScaleFactor();

/**
 * @brief Adjusts target window size to enforce 3:4 aspect ratio constraints.
 * @param width Requested or detected width.
 * @param height Requested or detected height.
 * @param[out] outWidth Calculated adjusted width.
 * @param[out] outHeight Calculated adjusted height.
 */
void GetAdjustedWindowSize(int width, int height, int* outWidth, int* outHeight);

inline int auto_x(float var) {
    return static_cast<int>(var * GetScreenScaleFactor().x);
}

inline int auto_y(float var) {
    return static_cast<int>(var * GetScreenScaleFactor().y);
}
