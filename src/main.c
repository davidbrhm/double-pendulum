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

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Double Pendulum");
    SetTargetFPS(TARGET_FPS);
    SetExitKey(KEY_ENTER); // KEY_NULL - release, KEY_ENTER - dev

    init_ui_theme();

    DoublePendulum *lab_pendulum = create_pendulum();
    AppState *app_state = init_state(lab_pendulum);

    while (!WindowShouldClose()) {
        update_ui(app_state);
        AppScreen current_screen = app_state->current_screen;

        float dt = GetFrameTime();
        if (dt > 0.1f) dt = 1.0f / TARGET_FPS;

        if (current_screen == CORE_SIMULATION && !lab_pendulum->is_paused) {
            update_pendulum(lab_pendulum, dt);
        } else if (app_state->current_screen == SCREEN_BUTTERFLY_EFFECT) {
            update_butterfly_effect(app_state->butterfly_effect, dt);
        }

        // drawing section
        BeginDrawing();

        ClearBackground(BLACK);
        draw_ui(app_state);

        switch (current_screen) {
            case CORE_SIMULATION:
                draw_pendulum(lab_pendulum, GetScreenWidth() / 2, GetScreenHeight() / 2);
                break;
            case SCREEN_BUTTERFLY_EFFECT:
                break;
            default:
                break;
        }

        EndDrawing();
    }

    destroy_pendulum(lab_pendulum);
    unload_ui_theme();
    CloseWindow();

#ifndef NDEBUG
    close_logger();
#endif
    return 0;
}
