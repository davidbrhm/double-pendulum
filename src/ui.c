#include "ui.h"
#include "raylib.h"
#include "constants.h"

void init_ui(void) {
    // TODO: font style
}

AppScreen update_ui(AppScreen current_screen) {
    const int key = GetKeyPressed();

    if (key == KEY_ESCAPE) return SCREEN_MENU;

    if (current_screen == SCREEN_MENU) {
        switch (key) {
            case KEY_ONE:
                return SCREEN_LABORATORY;
            case KEY_TWO:
                return SCREEN_BUTTERFLY_EFFECT;
            case KEY_THREE:
                return SCREEN_2D_FRACTAL;
            case KEY_FOUR:
                return SCREEN_3D_SPACE;
            case KEY_S:
                return SCREEN_SETTINGS;
            default:
                return current_screen;
        }
    }

    return current_screen;
}

void draw_ui(AppScreen current_screen) { // TODO: responsivity, resize
    const int screen_w = GetScreenWidth();
    const int screen_h = GetScreenHeight();

    switch (current_screen) {
        case SCREEN_MENU: {
            const char *title = "D O U B L E   P E N D U L U M";
            int title_w = MeasureText(title, FONT_SIZE_TITLE);
            DrawText(title, (screen_w - title_w) / 2, screen_h * UI_TITLE_Y_RATIO, FONT_SIZE_TITLE, LIGHTGRAY);

            int start_x = screen_w * UI_MENU_X_RATIO;
            DrawText("[1] Pendulum Laboratory", start_x, screen_h * 0.4f, FONT_SIZE_MENU, RAYWHITE);
            DrawText("[2] The Butterfly Effect", start_x, screen_h * 0.46f, FONT_SIZE_MENU, RAYWHITE);
            DrawText("[3] 2D Chaos Fractal Explorer", start_x, screen_h * 0.52f, FONT_SIZE_MENU, RAYWHITE);
            DrawText("[4] 3D Spacetime Cloud", start_x, screen_h * 0.58f, FONT_SIZE_MENU, RAYWHITE);
            DrawText("[S] Settings", start_x, screen_h * 0.66f, FONT_SIZE_MENU, GRAY);

            break;
        }
        case SCREEN_LABORATORY:
            DrawText("SCREEN: PENDULUM LABORATORY", 40, 40, FONT_SIZE_MENU, SKYBLUE);
            DrawText("[ESC] Return to Menu", 40, screen_h - 40, FONT_SIZE_FOOTER, RAYWHITE);
            break;
        case SCREEN_BUTTERFLY_EFFECT:
            DrawText("THE BUTTERFLY EFFECT: NOT IMPLEMENTED YET", 40, 40, FONT_SIZE_MENU, ORANGE);
            DrawText("[ESC] Return to Menu", 40, screen_h - 40, FONT_SIZE_FOOTER, RAYWHITE);
            break;
        case SCREEN_2D_FRACTAL:
            DrawText("2D FRACTAL EXPLORER: NOT IMPLEMENTED YET", 40, 40, FONT_SIZE_MENU, ORANGE);
            DrawText("[ESC] Return to Menu", 40, screen_h - 40, FONT_SIZE_FOOTER, RAYWHITE);
            break;
        case SCREEN_3D_SPACE:
            DrawText("3D SPACETIME CLOUD: NOT IMPLEMENTED YET", 40, 40, FONT_SIZE_MENU, ORANGE);
            DrawText("[ESC] Return to Menu", 40, screen_h - 40, FONT_SIZE_FOOTER, RAYWHITE);
            break;
        case SCREEN_SETTINGS:
            DrawText("SETTINGS: NOT IMPLEMENTED YET", 40, 40, FONT_SIZE_MENU, ORANGE);
            DrawText("[ESC] Return to Menu", 40, screen_h - 40, FONT_SIZE_FOOTER, RAYWHITE);
            break;
        default: break;
    }
}
