#include "raylib.h"
#include "ui.h"
#include "constants.h"

int main(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Double Pendulum");
    SetTargetFPS(TARGET_FPS);

    SetExitKey(KEY_ENTER); // KEY_NULL - release, KEY_ENTER - dev

    init_ui();

    AppScreen current_screen = SCREEN_MENU;

    while (!WindowShouldClose()) {
        current_screen = update_ui(current_screen);

        BeginDrawing();
        ClearBackground(BLACK);

        draw_ui(current_screen);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
