#pragma once

/**
 * @file Input.hpp
 * @brief Input polling wrappers for keyboard and mouse events.
 * @author Faliq
 *
 * @details Provides inline abstractions over Raylib hardware input functions
 * to provide a consistent, semantic control interface across all game screens.
 */

#include "raylib.h"

namespace Input {

inline bool isOkPressed() {
    return IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_Y);
}

inline bool isShootPressed() {
    return IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

inline bool isShootDown() {
    return IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_ENTER) || IsMouseButtonDown(MOUSE_BUTTON_LEFT);
}

inline bool isBackPressed() {
    return IsKeyPressed(KEY_B) || IsKeyPressed(KEY_BACKSPACE);
}

inline bool isForwardPressed() {
    return IsKeyPressed(KEY_F);
}

inline bool isMoveUp() {
    return IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP);
}

inline bool isMoveDown() {
    return IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN);
}

inline bool isMoveLeft() {
    return IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT);
}

inline bool isMoveRight() {
    return IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT);
}

} // namespace Input

// Global forwarders for backward compatibility
using namespace Input;
