#include "screens/screen_butterfly_effect.h"
#include "raylib.h"
#include "constants.h"

extern Font global_font_ui;
extern Font global_font_hud;

void update_screen_butterfly_effect(AppState *state) {
    // not implemented yet
}

void draw_screen_butterfly_effect(AppState *state) {
    const int screen_w = GetScreenWidth();
    const int screen_h = GetScreenHeight();
    float base_size = screen_h * 0.02f;
    const float pad = 40.0f;

    // title
    DrawTextEx(global_font_ui, "THE BUTTERFLY EFFECT", (Vector2){pad, pad}, base_size, 2.0f, THEME_FG);

    // HUD
    float btn_y = screen_h - pad - base_size;
    const char *help_text = "[ESC] Menu";
    float help_w = MeasureTextEx(global_font_ui, help_text, base_size * 0.8f, 1.0f).x;

    DrawTextEx(global_font_ui, help_text, (Vector2){screen_w - help_w - pad, btn_y}, base_size * 0.8f, 1.0f,
               THEME_FG_DIM);
}
