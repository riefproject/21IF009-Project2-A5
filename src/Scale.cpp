/**
 * @file Scale.cpp
 * @brief Implementation of screen resolution scaling and window adjustment.
 * @author Arief
 */

#include "Scale.hpp"
#include <algorithm>

static RenderTexture2D s_virtualCanvas{};
static bool s_hasCanvas = false;

void InitVirtualCanvas(int virtualWidth, int virtualHeight) {
    if (!s_hasCanvas) {
        s_virtualCanvas = LoadRenderTexture(virtualWidth, virtualHeight);
        SetTextureFilter(s_virtualCanvas.texture, TEXTURE_FILTER_BILINEAR);
        s_hasCanvas = true;
    }
}

void CloseVirtualCanvas() {
    if (s_hasCanvas) {
        UnloadRenderTexture(s_virtualCanvas);
        s_hasCanvas = false;
    }
}

void BeginVirtualCanvas() {
    if (!s_hasCanvas) {
        InitVirtualCanvas();
    }
    BeginTextureMode(s_virtualCanvas);
    ClearBackground(BLACK);
}

void EndVirtualCanvas() {
    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK); // Pillarbox & Letterbox black bars

    float scale = std::min(
        static_cast<float>(GetScreenWidth()) / static_cast<float>(VIRTUAL_SCREEN_WIDTH),
        static_cast<float>(GetScreenHeight()) / static_cast<float>(VIRTUAL_SCREEN_HEIGHT)
    );

    float viewWidth = static_cast<float>(VIRTUAL_SCREEN_WIDTH) * scale;
    float viewHeight = static_cast<float>(VIRTUAL_SCREEN_HEIGHT) * scale;
    float offsetX = (static_cast<float>(GetScreenWidth()) - viewWidth) * 0.5f;
    float offsetY = (static_cast<float>(GetScreenHeight()) - viewHeight) * 0.5f;

    // In Raylib, OpenGL framebuffers have inverted Y coordinate
    Rectangle sourceRec = {
        0.0f, 0.0f,
        static_cast<float>(s_virtualCanvas.texture.width),
        -static_cast<float>(s_virtualCanvas.texture.height)
    };
    Rectangle destRec = { offsetX, offsetY, viewWidth, viewHeight };

    DrawTexturePro(s_virtualCanvas.texture, sourceRec, destRec, { 0.0f, 0.0f }, 0.0f, WHITE);
    EndDrawing();
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
