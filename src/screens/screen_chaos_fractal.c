#include "screens/screen_chaos_fractal.h"
#include "logger.h"

extern Font global_font_ui;
extern Font global_font_hud;

void update_screen_chaos_fractal(AppState *state) {
    if (!state || !state->sim.chaos_fractal) return;

    ChaosFractal *cf = state->sim.chaos_fractal;

    if (IsWindowResized()) {
        resize_chaos_fractal(cf, GetScreenWidth(), GetScreenHeight());
    }

    switch (state->current_key) {
        case KEY_SPACE:
            cf->is_evolving = !cf->is_evolving;
            LOG_INFO("[SYS] Chaos Fractal evolving: %d", cf->is_evolving);
            break;

        case KEY_R:
            reset_chaos_fractal_state(cf);
            cf->is_evolving = false;
            break;

        case KEY_ESCAPE:
            state->current_screen = SCREEN_MENU;
            return;

        case KEY_H:
            state->flags ^= APP_FLAG_HIDE_CONTROLS;
            break;

        default:
            break;
    }

    if (cf->is_evolving) {
        evolve_chaos_map_mt(cf, FRACTAL_STEP_PER_FRAME);
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
