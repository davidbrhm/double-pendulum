#ifndef DOUBLE_PENDULUM_UI_H
#define DOUBLE_PENDULUM_UI_H
#include <stdbool.h>

#include "physics.h"
#include "butterfly_effect.h"

typedef enum AppScreen {
    SCREEN_MENU,
    CORE_SIMULATION,
    SCREEN_BUTTERFLY_EFFECT,
    SCREEN_2D_FRACTAL,
    SCREEN_3D_SPACE,
    SCREEN_SETTINGS
} AppScreen;

typedef union ActiveSimulation {
    DoublePendulum *lab_pendulum;
    ButterflyEffect *butterfly_effect;
} ActiveSimulation;

typedef struct AppState {
    AppScreen current_screen;
    int current_key;
    bool hide_controls;
    bool is_paused;
    bool show_trail;
    ActiveSimulation sim;
} AppState;

AppState *init_state(DoublePendulum *lp);

void destroy_state(AppState *state);

void init_ui_theme(void);

void unload_ui_theme(void);

void update_ui(AppState *state);

void draw_ui(AppState *state);

#endif //DOUBLE_PENDULUM_UI_H
