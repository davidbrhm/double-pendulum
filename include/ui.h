#ifndef DOUBLE_PENDULUM_UI_H
#define DOUBLE_PENDULUM_UI_H
#include <stdbool.h>
#include <stdint.h>

#include "physics.h"
#include "butterfly_effect.h"
#include "chaos_fractal.h"

typedef enum AppScreen {
    SCREEN_MENU,
    CORE_SIMULATION,
    SCREEN_BUTTERFLY_EFFECT,
    SCREEN_2D_FRACTAL,
    SCREEN_3D_SPACE,
    SCREEN_SETTINGS
} AppScreen;

typedef enum AppFlag {
    APP_FLAG_PAUSED = 1 << 0,
    APP_FLAG_SHOW_TRAIL = 1 << 1,
    APP_FLAG_HIDE_CONTROLS = 1 << 2,
    APP_FLAG_SHOW_ONLY_TRAILS = 1 << 3
} AppFlag;

typedef union ActiveSimulation {
    DoublePendulum *lab_pendulum;
    ButterflyEffect *butterfly_effect;
    ChaosFractal *chaos_fractal;
} ActiveSimulation;

typedef struct AppState {
    AppScreen current_screen;
    int current_key;
    uint32_t flags;
    ActiveSimulation sim;
} AppState;

AppState *init_state(void);

void destroy_state(AppState *state);

void init_ui_theme(void);

void unload_ui_theme(void);


#endif //DOUBLE_PENDULUM_UI_H
