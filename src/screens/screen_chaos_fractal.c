#include "screens/screen_chaos_fractal.h"
#include "logger.h"

extern Font global_font_ui;
extern Font global_font_hud;

void update_screen_chaos_fractal(AppState *state) {
    if (!state) {
        LOG_ERROR("[SYS] Null pointer exception -> state pointer is NULL in update_screen_chaos_fractal()");
        return;
    }

    if (IsWindowResized()) {
        resize_chaos_fractal(state->sim.chaos_fractal, GetScreenWidth(), GetScreenHeight());
        LOG_INFO("[SYS] Chaos Fractal buffers resized to: %dx%d", GetScreenWidth(), GetScreenHeight());
    }

    switch (state->current_key) {
        case KEY_ESCAPE:
            state->current_screen = SCREEN_MENU;
            LOG_INFO("[SYS] ESC -> Return to Menu from Chaos Fractal");
            return;
        case KEY_H:
            state->flags ^= APP_FLAG_HIDE_CONTROLS;
            LOG_INFO("[INPUT] H -> Toggle HUD");
            break;
        case KEY_G:
            LOG_INFO("[INPUT] G -> Generate Fractal start");
            generate_chaos_map_mt(state->sim.chaos_fractal, 1, 2000);
            LOG_INFO("[INPUT] G -> Generate Fractal end");
            break;
        case KEY_F:
            LOG_INFO("[INPUT] G -> Generate Fractal start");
            generate_chaos_map_mt(state->sim.chaos_fractal, 10, 400);
            LOG_INFO("[INPUT] G -> Generate Fractal end");
        default:
            break;
    }
}

void draw_screen_chaos_fractal(const AppState *state) {
    if (!state || !state->sim.chaos_fractal) {
        LOG_ERROR(
            "[SYS] Null pointer exception -> state/state.chaos_fractal pointer is NULL in draw_screen_chaos_fractal()");
        return;
    }
    DrawTexture(state->sim.chaos_fractal->texture, 0, 0, WHITE);

    return;
    const char *title = "2D CHAOS MAP";
    const int text_width = MeasureText(title, 20);
    DrawText(title, (GetScreenWidth() - text_width) / 2, GetScreenHeight() / 2, 20, LIGHTGRAY);

    if (!(state->flags & APP_FLAG_HIDE_CONTROLS)) {
        const char *help_text = "[H] Hide Controls    [ESC] Menu";
        DrawText(help_text, 10, GetScreenHeight() - 20, 10, DARKGRAY);
    }
}
