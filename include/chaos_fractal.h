#ifndef DOUBLE_PENDULUM_CHAOS_FRACTAL_H
#define DOUBLE_PENDULUM_CHAOS_FRACTAL_H

#include "raylib.h"
#include "physics.h"

#define FRACTAL_MAX_SPEED_THRESHOLD 15.0f

#define FRACTAL_HUE_COLD 240.0f
#define FRACTAL_HUE_HOT  0.0f

#define FRACTAL_BASE_SATURATION 0.9f
#define FRACTAL_BASE_BRIGHTNESS 0.95f
#define FRACTAL_DARK_THRESHOLD 0.05f

#define FRACTAL_STEP_PER_FRAME 4

typedef struct ChaosFractal {
    Image pixel_buffer;
    Texture2D texture;
    DoublePendulum *pendulums;
    float *max_speeds;
    int current_step;
    bool is_evolving;
} ChaosFractal;

ChaosFractal *create_chaos_fractal(int width, int height);


void resize_chaos_fractal(ChaosFractal *cf, int new_width, int new_height);

void reset_chaos_fractal_state(ChaosFractal *cf);

void evolve_chaos_map_mt(ChaosFractal *cf, int steps_per_frame);

void destroy_chaos_fractal(ChaosFractal *cf);

#endif //DOUBLE_PENDULUM_CHAOS_FRACTAL_H
