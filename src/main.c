#include <raylib.h>

int main(void) {
    InitWindow(900, 1200, "Block Shooter");
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        int posX = 190;
        DrawText("SABAR BANG LAGI DIBUAT!", posX, 200, 20, LIGHTGRAY);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

