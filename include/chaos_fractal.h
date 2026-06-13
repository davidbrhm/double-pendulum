#ifndef DOUBLE_PENDULUM_CHAOS_FRACTAL_H
#define DOUBLE_PENDULUM_CHAOS_FRACTAL_H

#include "raylib.h"
#include "physics.h"

#define FRACTAL_MAX_SPEED_THRESHOLD 15.0f
#define FRACTAL_STEP_PER_FRAME 4 // 4 by default
#define FRACTAL_MAX_EVOLUTION_STEPS 1000

#define VORTEX_HUE 200.0f
#define VORTEX_SATURATION_DEEP 1.0f
#define VORTEX_BRIGHTNESS_DEEP 0.6f

// TODO: dark-glow ratio
#define VORTEX_THRESHOLD_DARK 20.0f
#define VORTEX_THRESHOLD_GLOW 50.0f


typedef struct ChaosFractal {
    Image pixel_buffer;
    Texture2D texture;
    DoublePendulum *pendulums;
    float *max_speeds;
    int current_step;
    bool is_evolving;
    unsigned int frame_counter;
    float offset_x;
    float offset_y;
    float zoom;
} ChaosFractal;

ChaosFractal *create_chaos_fractal(int width, int height);

void resize_chaos_fractal(ChaosFractal *cf, int new_width, int new_height);

void reset_chaos_fractal_state(ChaosFractal *cf);

void evolve_chaos_map_mt(ChaosFractal *cf, int steps_per_frame);

void destroy_chaos_fractal(ChaosFractal *cf);

#endif //DOUBLE_PENDULUM_CHAOS_FRACTAL_H
