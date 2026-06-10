#ifndef DOUBLE_PENDULUM_CHAOS_FRACTAL_H
#define DOUBLE_PENDULUM_CHAOS_FRACTAL_H

#include "raylib.h"

typedef struct ChaosFractal {
    Image pixel_buffer;
    Texture2D texture;
} ChaosFractal;

ChaosFractal *create_chaos_fractal(int width, int height);

void generate_chaos_map_mt(ChaosFractal *cf, int step_size, int max_steps);

void resize_chaos_fractal(ChaosFractal *cf, int new_width, int new_height);

void destroy_chaos_fractal(ChaosFractal *cf);

#endif //DOUBLE_PENDULUM_CHAOS_FRACTAL_H
