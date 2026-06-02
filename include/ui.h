#ifndef DOUBLE_PENDULUM_UI_H
#define DOUBLE_PENDULUM_UI_H

typedef enum AppScreen {
    SCREEN_MENU,
    SCREEN_LABORATORY,
    SCREEN_BUTTERFLY_EFFECT,
    SCREEN_2D_FRACTAL,
    SCREEN_3D_SPACE,
    SCREEN_SETTINGS
} AppScreen;

void init_ui(void);

AppScreen update_ui(AppScreen current_screen);

void draw_ui(AppScreen current_screen);

#endif //DOUBLE_PENDULUM_UI_H
