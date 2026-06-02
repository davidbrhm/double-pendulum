#include "ui.h"
#include "raylib.h"
#include "constants.h"
#include "logger.h"

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

AppScreen update_ui(AppScreen current_screen) {
    const int key = GetKeyPressed();

    if (key == KEY_ESCAPE) return SCREEN_MENU;

    if (current_screen == SCREEN_MENU) {
        switch (key) {
            case KEY_ONE:
                return CORE_SIMULATION;
            /*
        case KEY_TWO:
            return SCREEN_BUTTERFLY_EFFECT;
        case KEY_THREE:
            return SCREEN_2D_FRACTAL;
        case KEY_FOUR:
            return SCREEN_3D_SPACE;
            */
            case KEY_S:
                return SCREEN_SETTINGS;
            default:
                return current_screen;
        }
    }

    return current_screen;
}

void draw_ui(AppScreen current_screen) { // TODO: fix resize crash
    const int screen_w = GetScreenWidth();
    const int screen_h = GetScreenHeight();

    const float inv_phi = 1.0f / GOLDEN_RATIO;
    const float inv_phi_sq = inv_phi * inv_phi;


    float base_size = screen_h * 0.02f; // TODO: 1.5%? 2%?...
    float menu_size = base_size * GOLDEN_RATIO;
    float title_size = menu_size * GOLDEN_RATIO * GOLDEN_RATIO;

    switch (current_screen) {
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
        case CORE_SIMULATION:
            DrawTextEx(font_ui, "ESC TO RETURN", (Vector2){40, screen_h - 40}, base_size, 2.0f, THEME_FG_DIM);
            break;

        default: break;
    }
}
