#include "screens/screen_butterfly_effect.h"
#include "butterfly_effect.h"
#include "raylib.h"
#include "constants.h"
#include "logger.h"

#include <math.h>

extern Font global_font_ui;
extern Font global_font_hud;

void update_screen_butterfly_effect(AppState *state) {
    if (!state->sim.butterfly_effect) return;

    ButterflyEffect *bf = state->sim.butterfly_effect;

    if (IsKeyDown(KEY_D)) bf->camera_angle += BF_CAM_ROT_SPEED;
    if (IsKeyDown(KEY_A)) bf->camera_angle -= BF_CAM_ROT_SPEED;


    switch (state->current_key) {
        case KEY_SPACE:
            state->is_paused = !state->is_paused;
            LOG_INFO("space -> pause: %d", state->is_paused);
            break;
        case KEY_R:
            destroy_butterfly_effect(bf);
            state->sim.butterfly_effect = create_butterfly_effect();
            LOG_INFO("R -> reset bf e");
            break;
        case KEY_H:
            state->hide_controls = !state->hide_controls;
            LOG_INFO("H -> hide HUD");
            break;
        case KEY_C:
            bf->camera_angle = 0;
            LOG_INFO("C -> camera center");
            break;
        case KEY_T:
            state->show_trail = !state->show_trail;
            LOG_INFO("T -> show trail: %d", state->show_trail);
            break;
        default: break;
    }
}


void draw_screen_butterfly_effect(AppState *state) {
    if (state->hide_controls) return;

    const int screen_w = GetScreenWidth();
    const int screen_h = GetScreenHeight();
    float base_size = screen_h * 0.02f;
    const float pad = 40.0f;

    // title
    DrawTextEx(global_font_ui, "THE BUTTERFLY EFFECT", (Vector2){pad, pad}, base_size, 2.0f, THEME_FG);

    // performance (copy paste from screen_code_sim.c)
    const float line_spacing = base_size * 1.5f;
    const float frame_ms = GetFrameTime() * 1000.0f;
    const char *fps_text = TextFormat("FPS : %d", GetFPS());
    const char *dt_text = TextFormat("DT  : %.2f ms", frame_ms);

    const float fps_w = MeasureTextEx(global_font_hud, fps_text, base_size * 0.8f, 1.0f).x;
    const float dt_w = MeasureTextEx(global_font_hud, dt_text, base_size * 0.8f, 1.0f).x;

    DrawTextEx(global_font_hud, fps_text, (Vector2){screen_w - fps_w - pad, pad}, base_size * 0.8f, 1.0f,
               THEME_FG);
    DrawTextEx(global_font_hud, dt_text, (Vector2){screen_w - dt_w - pad, pad + line_spacing}, base_size * 0.8f, 1.0f,
               THEME_FG_DIM);

    // HUD
    float btn_y = screen_h - pad - base_size;
    const char *help_text =
            "[A/D] Rotate Camera    [SPACE] Play/Pause    [R] Reset Swarm    [H] Hide Controls    [ESC] Menu";
    float help_w = MeasureTextEx(global_font_ui, help_text, base_size * 0.8f, 1.0f).x;

    DrawTextEx(global_font_ui, help_text, (Vector2){screen_w - help_w - pad, btn_y}, base_size * 0.8f, 1.0f,
               THEME_FG_DIM);
}
