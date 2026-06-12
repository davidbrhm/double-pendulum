#include "screens/screen_chaos_fractal.h"
#include "logger.h"

#include <math.h>

extern Font global_font_ui;
extern Font global_font_hud;

void update_screen_chaos_fractal(AppState *state) {
    if (!state || !state->sim.chaos_fractal) return;

    ChaosFractal *cf = state->sim.chaos_fractal;

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

    Texture2D tex = state->sim.chaos_fractal->texture;
    Rectangle sourceRec = {0.0f, 0.0f, (float) tex.width, (float) tex.height};
    Rectangle destRec = {0.0f, 0.0f, (float) GetScreenWidth(), (float) GetScreenHeight()};
    Vector2 origin = { 0.0f, 0.0f };
    DrawTexturePro(tex, sourceRec, destRec, origin, 0.0f, WHITE);


    if (!(state->flags & APP_FLAG_HIDE_CONTROLS)) {
        // performance (copy paste from screen_code_sim.c)
        const int screen_w = GetScreenWidth();
        const int screen_h = GetScreenHeight();
        float base_size = screen_h * 0.02f;
        const float pad = 40.0f;

        const float line_spacing = base_size * 1.5f;
        const float frame_ms = GetFrameTime() * 1000.0f;
        const char *fps_text = TextFormat("FPS : %d", GetFPS());
        const char *dt_text = TextFormat("DT  : %.2f ms", frame_ms);

        const float fps_w = MeasureTextEx(global_font_hud, fps_text, base_size * 0.8f, 1.0f).x;
        const float dt_w = MeasureTextEx(global_font_hud, dt_text, base_size * 0.8f, 1.0f).x;

        DrawTextEx(global_font_hud, fps_text, (Vector2){screen_w - fps_w - pad, pad}, base_size * 0.8f, 1.0f,
                   THEME_FG);
        DrawTextEx(global_font_hud, dt_text, (Vector2){screen_w - dt_w - pad, pad + line_spacing}, base_size * 0.8f,
                   1.0f,
                   THEME_FG_DIM);

        // HUD
        float btn_y = screen_h - pad - base_size;
        const char *help_text = "[H] Hide Controls    [ESC] Menu";

        float help_w = MeasureTextEx(global_font_ui, help_text, base_size * 0.8f, 1.0f).x;

        DrawTextEx(global_font_ui, help_text, (Vector2){screen_w - help_w - pad, btn_y}, base_size * 0.8f, 1.0f,
                   THEME_FG_DIM);
    }
}
