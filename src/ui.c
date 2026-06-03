#include "ui.h"
#include "raylib.h"
#include "constants.h"
#include "logger.h"

#include <stdlib.h>

static Font font_ui;
static Font font_hud;

void init_ui_theme(void) {
    font_ui = LoadFontEx("../assets/fonts/Inter/static/Inter_28pt-Regular.ttf", 150, 0, 0);
    font_hud = LoadFontEx("../assets/fonts/JetBrains_Mono/static/JetBrainsMono-Regular.ttf", 64, 0, 0);

    // TODO: keep trying
    // GenTextureMipmaps(&font_ui.texture);
    // GenTextureMipmaps(&font_hud.texture);
    // SetTextureFilter(font_ui.texture, TEXTURE_FILTER_TRILINEAR);
    // SetTextureFilter(font_hud.texture, TEXTURE_FILTER_TRILINEAR);

    SetTextureFilter(font_ui.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(font_hud.texture, TEXTURE_FILTER_BILINEAR);

    if (font_ui.texture.id == 0 || font_hud.texture.id == 0) {
        LOG_WARN("[SYS] Custom fonts failed to load. Using default Raylib font. Check 'assets/fonts/' directory.");
    } else {
        LOG_INFO("[SYS] UI Theme and custom fonts loaded successfully.");
    }
}

void unload_ui_theme(void) {
    UnloadFont(font_ui);
    UnloadFont(font_hud);
    LOG_INFO("[SYS] UI Theme unloaded.");
}

AppState *init_state(DoublePendulum *lp) {
    AppState *s = calloc(1, sizeof(AppState));
    if (!s) {
        LOG_FATAL("[SYS] Memory allocation failed -> Target: AppState struct in init_state()");
        return NULL;
    }

    // s->current_screen = (screen == NULL) ? SCREEN_MENU : screen;
    s->current_screen = SCREEN_MENU;
    s->hide_controls = false;
    s->lab_pendulum = lp;

    return s;
}

void destroy_state(AppState *state) {
    if (state) free(state);
}

void update_ui(AppState *state) {
    if (!state) exit(44); // TODO
    const int key = GetKeyPressed();

    switch (key) {
        case 0:
            return;
        case KEY_ESCAPE: state->current_screen = SCREEN_MENU;
            return;
    }

    switch (state->current_screen) {
        case SCREEN_MENU: {
            switch (key) {
                case KEY_ONE:
                    state->current_screen = CORE_SIMULATION;
                    return;
                /*
            case KEY_TWO:
                state->current_screen = SCREEN_BUTTERFLY_EFFECT;
                return;
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
        case CORE_SIMULATION: {
            if (!state->lab_pendulum) break;

            switch (key) {
                // kell break?
                case KEY_SPACE:
                    state->lab_pendulum->is_paused = !state->lab_pendulum->is_paused;
                    LOG_INFO("[INPUT] Key 'SPACE' pressed -> Paused state: %d", state->lab_pendulum->is_paused);
                    break;
                case KEY_R:
                    randomize_pendulum(state->lab_pendulum);
                    break;
                case KEY_T:
                    state->lab_pendulum->show_trail = !state->lab_pendulum->show_trail;
                    LOG_INFO("[INPUT] Key 'T' pressed -> Trail state: %d", state->lab_pendulum->show_trail);
                    break;
                case KEY_H:
                    state->hide_controls = !state->hide_controls;
                    break;
                default: break;
            }
        }
    }
}


void draw_ui(AppState *state) {
    const int screen_w = GetScreenWidth();
    const int screen_h = GetScreenHeight();

    const float inv_phi = 1.0f / GOLDEN_RATIO;
    const float inv_phi_sq = inv_phi * inv_phi;


    float base_size = screen_h * 0.02f; // TODO: 1.5%? 2%?...
    float menu_size = base_size * GOLDEN_RATIO;
    float title_size = menu_size * GOLDEN_RATIO * GOLDEN_RATIO;

    switch (state->current_screen) {
        case SCREEN_MENU: {
            ClearBackground(THEME_BG);

            const char *title = "DOUBLE PENDULUM";
            Vector2 title_bounds = MeasureTextEx(font_ui, title, title_size, 2.0f);
            Vector2 title_pos = {
                (screen_w - title_bounds.x) / 2.0f,
                screen_h * inv_phi_sq - title_bounds.y
            };
            DrawTextEx(font_ui, title, title_pos, title_size, 2.0f, THEME_FG);

            float start_x = screen_w * inv_phi_sq;
            float start_y = screen_h * 0.5f;
            float spacing = menu_size * GOLDEN_RATIO;

            // 01 is the only one active
            DrawTextEx(font_ui, "01", (Vector2){start_x, start_y}, menu_size, 2.0f, THEME_ACCENT);
            DrawTextEx(font_ui, "Core Simulation", (Vector2){start_x + menu_size * 2, start_y}, menu_size, 2.0f,
                       THEME_FG);

            DrawTextEx(font_ui, "02", (Vector2){start_x, start_y + spacing}, menu_size, 2.0f, THEME_FG_DIM);
            DrawTextEx(font_ui, "The Butterfly Effect", (Vector2){start_x + menu_size * 2, start_y + spacing},
                       menu_size, 2.0f, THEME_FG_DIM);

            DrawTextEx(font_ui, "03", (Vector2){start_x, start_y + spacing * 2}, menu_size, 2.0f, THEME_FG_DIM);
            DrawTextEx(font_ui, "2D Chaos Fractal Explorer", (Vector2){start_x + menu_size * 2, start_y + spacing * 2},
                       menu_size, 2.0f, THEME_FG_DIM);

            DrawTextEx(font_ui, "04", (Vector2){start_x, start_y + spacing * 3}, menu_size, 2.0f, THEME_FG_DIM);
            DrawTextEx(font_ui, "3D Spacetime Cloud", (Vector2){start_x + menu_size * 2, start_y + spacing * 3},
                       menu_size, 2.0f, THEME_FG_DIM);

            DrawTextEx(font_ui, " S", (Vector2){start_x, start_y + spacing * 4.5f}, menu_size, 2.0f, THEME_FG_DIM);
            DrawTextEx(font_ui, "Settings", (Vector2){start_x + menu_size * 2, start_y + spacing * 4.5f}, menu_size,
                       2.0f, THEME_FG_DIM);

            DrawTextEx(font_hud, "v0.1.0-alpha | CPU Physics Engine", (Vector2){20, screen_h - 30}, base_size, 1.0f,
                       THEME_FG_DIM);
            break;
        }
        case CORE_SIMULATION: {
            if (state->hide_controls) break;

            const float pad = 40.0f;
            const float line_spacing = base_size * 1.5f;

            // title
            // DrawTextEx(font_ui, "CORE SIMULATION", (Vector2){pad, pad}, base_size, 2.0f, THEME_FG);

            // performance
            const float frame_ms = GetFrameTime() * 1000.0f;
            const char *fps_text = TextFormat("FPS : %d", GetFPS());
            const char *dt_text = TextFormat("DT  : %.2f ms", frame_ms);

            const float fps_w = MeasureTextEx(font_hud, fps_text, base_size * 0.8f, 1.0f).x;
            const float dt_w = MeasureTextEx(font_hud, dt_text, base_size * 0.8f, 1.0f).x;

            DrawTextEx(font_hud, fps_text, (Vector2){screen_w - fps_w - pad, pad}, base_size * 0.8f, 1.0f,
                       THEME_ACCENT);
            DrawTextEx(font_hud, dt_text, (Vector2){screen_w - dt_w - pad, pad + line_spacing}, base_size * 0.8f, 1.0f,
                       THEME_FG_DIM);

            // if (!state->hide_controls && state->lab_pendulum) {
            DrawTextEx(font_ui, "TELEMETRY", (Vector2){pad, screen_h - 160.0f}, base_size * 0.8f, 2.0f, THEME_FG);

            const char *t1_str = TextFormat("THETA 1 : %6.2f rad", state->lab_pendulum->theta1);
            const char *t2_str = TextFormat("THETA 2 : %6.2f rad", state->lab_pendulum->theta2);

            DrawTextEx(font_hud, t1_str, (Vector2){pad, screen_h - 130.0f}, base_size * 0.7f, 1.0f, THEME_FG_DIM);
            DrawTextEx(font_hud, t2_str, (Vector2){pad, screen_h - 110.0f}, base_size * 0.7f, 1.0f, THEME_FG_DIM);


            // HUD
            const float btn_y = screen_h - pad - base_size;
            const char *help_text =
                    "[SPACE] Play/Pause    [R] Randomize    [T] Toggle trail    [H] Hide controls    [ESC] Menu";
            const float help_w = MeasureTextEx(font_ui, help_text, base_size * 0.8f, 1.0f).x;

            DrawTextEx(font_ui, help_text, (Vector2){screen_w - help_w - pad, btn_y}, base_size * 0.8f, 1.0f,
                       THEME_FG_DIM);

            break;
        }

        default: break;
    }
}
