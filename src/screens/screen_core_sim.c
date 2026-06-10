#include "screens/screen_core_sim.h"
#include "raylib.h"
#include "constants.h"
#include "logger.h"

extern Font global_font_ui;
extern Font global_font_hud;

void update_screen_core_sim(AppState *state) {
    if (!state->sim.lab_pendulum) {
        LOG_ERROR("[SYS] Null pointer exception -> DoublePendulum pointer is NULL in update_screen_core_sim()");
        return;
    }

    switch (state->current_key) {
        case KEY_ESCAPE:
            state->current_screen = SCREEN_MENU;
            LOG_INFO("[SYS] ESC -> Return to Menu from Core Simulation");
            return;
        case KEY_SPACE:
            state->flags ^= APP_FLAG_PAUSED;
            LOG_INFO("[INPUT] Key 'SPACE' pressed -> Paused state: %d", state->flags & APP_FLAG_PAUSED);
            break;
        case KEY_H:
            state->flags ^= APP_FLAG_HIDE_CONTROLS;
            LOG_INFO("[INPUT] Key 'H' pressed -> Hide controls state: %d", state->flags & APP_FLAG_HIDE_CONTROLS);
            break;
        case KEY_T:
            state->flags ^= APP_FLAG_SHOW_TRAIL;
            LOG_INFO("[INPUT] Key 'T' pressed -> Trail state: %d", state->flags & APP_FLAG_SHOW_TRAIL);
            break;
        case KEY_R:
            randomize_pendulum(state->sim.lab_pendulum);
            break;
        case KEY_V:
            state->flags ^= APP_FLAG_SHOW_ONLY_TRAILS;
            LOG_INFO("[INPUT] Key 'V' pressed -> Hide pendulum state: %d", state->flags & APP_FLAG_SHOW_ONLY_TRAILS);
            break;
        default: break;
    }
}

void draw_screen_core_sim(AppState *state) {
    if (state->flags & APP_FLAG_HIDE_CONTROLS) return;

    const int screen_w = GetScreenWidth();
    const int screen_h = GetScreenHeight();

    float base_size = screen_h * 0.02f; // TODO: 1.5%? 2%?...
    const float pad = 40.0f;
    const float line_spacing = base_size * 1.5f;

    // title
    DrawTextEx(global_font_ui, "CORE SIMULATION", (Vector2){pad, pad}, base_size, 2.0f, THEME_FG);

    // performance
    const float frame_ms = GetFrameTime() * 1000.0f;
    const char *fps_text = TextFormat("FPS : %d", GetFPS());
    const char *dt_text = TextFormat("DT  : %.2f ms", frame_ms);

    const float fps_w = MeasureTextEx(global_font_hud, fps_text, base_size * 0.8f, 1.0f).x;
    const float dt_w = MeasureTextEx(global_font_hud, dt_text, base_size * 0.8f, 1.0f).x;

    DrawTextEx(global_font_hud, fps_text, (Vector2){screen_w - fps_w - pad, pad}, base_size * 0.8f, 1.0f,
               THEME_FG);
    DrawTextEx(global_font_hud, dt_text, (Vector2){screen_w - dt_w - pad, pad + line_spacing}, base_size * 0.8f, 1.0f,
               THEME_FG_DIM);

    // if (!state->hide_controls && state->lab_pendulum) {
    DrawTextEx(global_font_ui, "TELEMETRY", (Vector2){pad, screen_h - 160.0f}, base_size * 0.8f, 2.0f, THEME_FG);

    const char *t1_str = TextFormat("THETA 1 : %6.2f rad", state->sim.lab_pendulum->theta1);
    const char *t2_str = TextFormat("THETA 2 : %6.2f rad", state->sim.lab_pendulum->theta2);

    DrawTextEx(global_font_hud, t1_str, (Vector2){pad, screen_h - 130.0f}, base_size * 0.7f, 1.0f, THEME_FG_DIM);
    DrawTextEx(global_font_hud, t2_str, (Vector2){pad, screen_h - 110.0f}, base_size * 0.7f, 1.0f, THEME_FG_DIM);

    // HUD
    const float btn_y = screen_h - pad - base_size;
    const char *help_text =
            "[SPACE] Play/Pause    [R] Randomize    [T] Toggle Trail    [V] Hide Pendulum    [H] Hide Controls    [ESC] Menu";
    const float help_w = MeasureTextEx(global_font_ui, help_text, base_size * 0.8f, 1.0f).x;

    DrawTextEx(global_font_ui, help_text, (Vector2){screen_w - help_w - pad, btn_y}, base_size * 0.8f, 1.0f,
               THEME_FG_DIM);
}
