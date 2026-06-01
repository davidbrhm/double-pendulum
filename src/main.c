#include "raylib.h"
#include "ui.h"
#include "constants.h"
#include "physics.h"
#include "render.h"

int main(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Double Pendulum");
    SetTargetFPS(TARGET_FPS);

    SetExitKey(KEY_ENTER); // KEY_NULL - release, KEY_ENTER - dev

    init_ui();
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI);

    DoublePendulum *lab_pendulum = create_pendulum();

    AppScreen current_screen = SCREEN_MENU;

    while (!WindowShouldClose()) {
        current_screen = update_ui(current_screen);

        BeginDrawing();
        ClearBackground(BLACK);

        draw_ui(current_screen);

        if (current_screen == SCREEN_LABORATORY) {
            draw_pendulum(lab_pendulum, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        }

        EndDrawing();
    }

    destroy_pendulum(lab_pendulum);
    CloseWindow();
    return 0;
}
