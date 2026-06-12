#ifndef DOUBLE_PENDULUM_CHAOS_FRACTAL_H
#define DOUBLE_PENDULUM_CHAOS_FRACTAL_H

#include "raylib.h"
#include "physics.h"

#define FRACTAL_MAX_SPEED_THRESHOLD 15.0f

#define FRACTAL_STEP_PER_FRAME 4 // 4 by default
#define VORTEX_HUE 200.0f
#define VORTEX_SATURATION_DEEP 1.0f
#define VORTEX_BRIGHTNESS_DEEP 0.6f

#define VORTEX_THRESHOLD_DARK 2.0f
#define VORTEX_THRESHOLD_GLOW 12.0f

// fix render puffer
#define FRACTAL_BUFFER_WIDTH  2000
#define FRACTAL_BUFFER_HEIGHT 2000

typedef struct ChaosFractal {
    Image pixel_buffer;
    Texture2D texture;
    DoublePendulum *pendulums;
    float *max_speeds;
    int current_step;
    bool is_evolving;
} ChaosFractal;

ChaosFractal *create_chaos_fractal(void);


void reset_chaos_fractal_state(ChaosFractal *cf);

void evolve_chaos_map_mt(ChaosFractal *cf, int steps_per_frame);

void destroy_chaos_fractal(ChaosFractal *cf);

#endif //DOUBLE_PENDULUM_CHAOS_FRACTAL_H
