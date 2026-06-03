#ifndef DOUBLE_PENDULUM_UI_H
#define DOUBLE_PENDULUM_UI_H
#include <stdbool.h>

#include "physics.h"

typedef enum AppScreen {
    SCREEN_MENU,
    CORE_SIMULATION,
    SCREEN_BUTTERFLY_EFFECT,
    SCREEN_2D_FRACTAL,
    SCREEN_3D_SPACE,
    SCREEN_SETTINGS
} AppScreen;

typedef struct AppState {
    AppScreen current_screen;
    bool hide_controls;
    int current_key;
    DoublePendulum *lab_pendulum;
} AppState;

AppState *init_state(DoublePendulum *lp);

void destroy_state(AppState *state);

void init_ui_theme(void);

void unload_ui_theme(void);

void update_ui(AppState *state);

void draw_ui(AppState *state);

#endif //DOUBLE_PENDULUM_UI_H
