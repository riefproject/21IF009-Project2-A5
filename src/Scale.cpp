/**
 * @file Scale.cpp
 * @brief Implementation of screen resolution scaling and window adjustment.
 * @author Arief
 */

#include "Scale.hpp"
#include <algorithm>

ScaleFactor GetScreenScaleFactor() {
    float screenWidth = static_cast<float>(GetScreenWidth());
    float screenHeight = static_cast<float>(GetScreenHeight());

    float scaleX = screenWidth / static_cast<float>(MIN_SCREEN_WIDTH);
    float scaleY = screenHeight / static_cast<float>(MIN_SCREEN_HEIGHT);

    float finalScale = std::min(scaleX, scaleY);
    return ScaleFactor{ finalScale, finalScale };
}

void GetAdjustedWindowSize(int width, int height, int* outWidth, int* outHeight) {
    float targetRatio = static_cast<float>(ASPECT_RATIO_WIDTH) / static_cast<float>(ASPECT_RATIO_HEIGHT);

    if (static_cast<float>(width) > static_cast<float>(height) * targetRatio) {
        *outHeight = height;
        *outWidth = static_cast<int>(static_cast<float>(height) * targetRatio);
    } else {
        *outWidth = width;
        *outHeight = static_cast<int>(static_cast<float>(width) / targetRatio);
    }

    if (*outWidth < MIN_SCREEN_WIDTH) *outWidth = MIN_SCREEN_WIDTH;
    if (*outHeight < MIN_SCREEN_HEIGHT) *outHeight = MIN_SCREEN_HEIGHT;
}
