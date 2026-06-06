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
    ButterflyEffect *bf_effect = create_butterfly_effect();

    AppState *app_state = init_state();
    AppScreen last_screen = SCREEN_MENU;

    while (!WindowShouldClose()) {
        update_ui(app_state);
        AppScreen current_screen = app_state->current_screen;

        float dt = GetFrameTime();
        if (dt > 0.1f) dt = 1.0f / TARGET_FPS;

        if (current_screen != last_screen) {
            LOG_INFO("[SYS] Screen transition: %d -> %d", last_screen, current_screen);

            switch (current_screen) {
                case CORE_SIMULATION:
                    app_state->sim.lab_pendulum = lab_pendulum;
                    break;
                case SCREEN_BUTTERFLY_EFFECT:
                    app_state->sim.butterfly_effect = bf_effect;
                    break;
                default:
                    break;
            }

            app_state->flags |= APP_FLAG_PAUSED;
            last_screen = current_screen;
        }

        switch (current_screen) {
            case CORE_SIMULATION:
                if (!(app_state->flags & APP_FLAG_PAUSED)) {
                    update_pendulum(app_state->sim.lab_pendulum, dt, app_state->flags & APP_FLAG_SHOW_TRAIL);
                }
                break;
            case SCREEN_BUTTERFLY_EFFECT:
                update_butterfly_effect(app_state->sim.butterfly_effect, dt, app_state->flags & APP_FLAG_PAUSED,
                                        app_state->flags & APP_FLAG_SHOW_TRAIL);
                break;
            default:
                break;
        }

        // drawing section
        BeginDrawing();

        ClearBackground(BLACK);

        switch (current_screen) {
            case CORE_SIMULATION:
                draw_pendulum(app_state->sim.lab_pendulum, GetScreenWidth() / 2, GetScreenHeight() / 2,
                              app_state->flags & APP_FLAG_SHOW_TRAIL, app_state->flags & APP_FLAG_SHOW_ONLY_TRAILS);
                break;
            case SCREEN_BUTTERFLY_EFFECT:
                draw_butterfly_effect(app_state->sim.butterfly_effect, app_state->flags & APP_FLAG_SHOW_TRAIL, app_state->flags & APP_FLAG_SHOW_ONLY_TRAILS);
                break;
            default:
                break;
        }
        draw_ui(app_state);

        EndDrawing();
    }

    if (app_state->current_screen == SCREEN_BUTTERFLY_EFFECT) {
        destroy_butterfly_effect(app_state->sim.butterfly_effect);
    }

    destroy_pendulum(lab_pendulum);
    unload_ui_theme();
    CloseWindow();

#ifndef NDEBUG
    close_logger();
#endif
    return 0;
}
