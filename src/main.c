#include <stdio.h>

#include "raylib.h"
#include "ui.h"
#include "constants.h"
#include "logger.h"
#include "physics.h"
#include "render.h"

int main(void) {
#ifndef NDEBUG
    init_logger();
#endif

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Double Pendulum");
    SetTargetFPS(TARGET_FPS);
    SetExitKey(KEY_ENTER); // KEY_NULL - release, KEY_ENTER - dev

    init_ui();

    DoublePendulum *lab_pendulum = create_pendulum();
    AppScreen current_screen = SCREEN_MENU;

    while (!WindowShouldClose()) {
        current_screen = update_ui(current_screen);

        if (current_screen == SCREEN_LABORATORY) {
            if (IsKeyPressed(KEY_SPACE)) {
                lab_pendulum->is_paused = !lab_pendulum->is_paused;
                LOG_INFO("[INPUT] Key 'SPACE' pressed -> Paused state: %d", lab_pendulum->is_paused);
            }

            if (IsKeyPressed(KEY_R)) {
                randomize_pendulum(lab_pendulum);
            }

            if (IsKeyPressed(KEY_T)) {
                lab_pendulum->show_trail = !lab_pendulum->show_trail;
                LOG_INFO("[INPUT] Key 'T' pressed -> Trail state: %d", lab_pendulum->show_trail);
            }

            float dt = GetFrameTime();
            update_pendulum(lab_pendulum, dt);
        }


        // drawing section
        BeginDrawing();

        ClearBackground(BLACK);
        draw_ui(current_screen);

        switch (current_screen) {
            case SCREEN_LABORATORY:
                draw_pendulum(lab_pendulum, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
                break;
            default:
                break;
        }

        EndDrawing();
    }

    destroy_pendulum(lab_pendulum);
    CloseWindow();

#ifndef NDEBUG
    close_logger();
#endif
    return 0;
}
