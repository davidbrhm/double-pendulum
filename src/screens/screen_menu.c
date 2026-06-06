#include "screens/screen_menu.h"
#include "raylib.h"
#include "constants.h"

extern Font global_font_ui;
extern Font global_font_hud;

static void prepare_simulation_flags(AppState *state) {
    state->flags &= ~(APP_FLAG_SHOW_TRAIL | APP_FLAG_SHOW_ONLY_TRAILS);
}

void update_screen_menu(AppState *state) {
    switch (state->current_key) {
        case KEY_ONE:
            prepare_simulation_flags(state);
            state->current_screen = CORE_SIMULATION;
            return;
        case KEY_TWO:
            prepare_simulation_flags(state);
            state->current_screen = SCREEN_BUTTERFLY_EFFECT;
            return;
        /*
case KEY_THREE:
    state->current_screen = SCREEN_2D_FRACTAL;
    return;
case KEY_FOUR:
    state->current_screen = SCREEN_3D_SPACE;
    return;
    */
        case KEY_S:
            state->current_screen = SCREEN_SETTINGS;
            return;
        default:
            return;
    }
}

void draw_screen_menu(AppState *state) {
    const int screen_w = GetScreenWidth();
    const int screen_h = GetScreenHeight();

    const float inv_phi = 1.0f / GOLDEN_RATIO;
    const float inv_phi_sq = inv_phi * inv_phi;

    float base_size = screen_h * 0.02f; // TODO: 1.5%? 2%?...
    float menu_size = base_size * GOLDEN_RATIO;
    float title_size = menu_size * GOLDEN_RATIO * GOLDEN_RATIO;

    ClearBackground(THEME_BG);

    const char *title = "DOUBLE PENDULUM";
    Vector2 title_bounds = MeasureTextEx(global_font_ui, title, title_size, 2.0f);
    Vector2 title_pos = {
        (screen_w - title_bounds.x) / 2.0f,
        screen_h * inv_phi_sq - title_bounds.y
    };
    DrawTextEx(global_font_ui, title, title_pos, title_size, 2.0f, THEME_FG);

    float start_x = screen_w * inv_phi_sq;
    float start_y = screen_h * 0.5f;
    float spacing = menu_size * GOLDEN_RATIO;

    // 01 is the only one active
    DrawTextEx(global_font_ui, "01", (Vector2){start_x, start_y}, menu_size, 2.0f, THEME_ACCENT);
    DrawTextEx(global_font_ui, "Core Simulation", (Vector2){start_x + menu_size * 2, start_y}, menu_size, 2.0f,
               THEME_FG);

    DrawTextEx(global_font_ui, "02", (Vector2){start_x, start_y + spacing}, menu_size, 2.0f, THEME_FG_DIM);
    DrawTextEx(global_font_ui, "The Butterfly Effect", (Vector2){start_x + menu_size * 2, start_y + spacing},
               menu_size, 2.0f, THEME_FG_DIM);

    DrawTextEx(global_font_ui, "03", (Vector2){start_x, start_y + spacing * 2}, menu_size, 2.0f, THEME_FG_DIM);
    DrawTextEx(global_font_ui, "2D Chaos Fractal Explorer", (Vector2){start_x + menu_size * 2, start_y + spacing * 2},
               menu_size, 2.0f, THEME_FG_DIM);

    DrawTextEx(global_font_ui, "04", (Vector2){start_x, start_y + spacing * 3}, menu_size, 2.0f, THEME_FG_DIM);
    DrawTextEx(global_font_ui, "3D Spacetime Cloud", (Vector2){start_x + menu_size * 2, start_y + spacing * 3},
               menu_size, 2.0f, THEME_FG_DIM);

    DrawTextEx(global_font_ui, " S", (Vector2){start_x, start_y + spacing * 4.5f}, menu_size, 2.0f, THEME_FG_DIM);
    DrawTextEx(global_font_ui, "Settings", (Vector2){start_x + menu_size * 2, start_y + spacing * 4.5f}, menu_size,
               2.0f, THEME_FG_DIM);

    DrawTextEx(global_font_hud, "v0.1.0-alpha | CPU Physics Engine", (Vector2){20, screen_h - 30}, base_size, 1.0f,
               THEME_FG_DIM);
}
