#include "chaos_fractal.h"
#include "logger.h"
#include "physics.h"

#include <stdlib.h>
#include <pthread.h>
#include <math.h>

typedef struct FractalThreadArgs {
    ChaosFractal *cf;
    int start_y;
    int end_y;
    int step_size;
    int max_steps;
} FractalThreadArgs;

ChaosFractal *create_chaos_fractal(const int width, const int height) {
    ChaosFractal *cf = calloc(1, sizeof(ChaosFractal));
    if (!cf) {
        LOG_FATAL("[SYS] Memory allocation failed -> Target: ChaosFractal struct in create_chaos_fractal()");
        return NULL;
    }

    cf->pixel_buffer = GenImageColor(width, height, BLACK);
    cf->texture = LoadTextureFromImage(cf->pixel_buffer);

    return cf;
}

void resize_chaos_fractal(ChaosFractal *cf, int new_width, int new_height) {
    if (!cf) return;

    UnloadTexture(cf->texture);
    UnloadImage(cf->pixel_buffer);

    cf->pixel_buffer = GenImageColor(new_width, new_height, BLACK);
    cf->texture = LoadTextureFromImage(cf->pixel_buffer);

    // TODO: set flags for multi threading
}

static void *chaos_map_worker_thread(void *arg) {
    FractalThreadArgs *args = (FractalThreadArgs *)arg;
    ChaosFractal *cf = args->cf;

    int width = cf->pixel_buffer.width;
    int height = cf->pixel_buffer.height;
    Color *pixels = (Color *)cf->pixel_buffer.data;

    const float DT = 0.016f * 3;

    for (int y = args->start_y; y < args->end_y; y += args->step_size) {
        for (int x = 0; x < width; x += args->step_size) {

            float theta1 = ((float)x / width) * 2.0f * PI - PI;
            float theta2 = ((float)y / height) * 2.0f * PI - PI;

            DoublePendulum p = {0};
            p.m1 = 10.0f; p.m2 = 10.0f;
            p.l1 = 100.0f; p.l2 = 100.0f;
            p.g = 9.81f;
            p.theta1 = theta1; p.theta2 = theta2;

            float max_speed = 0.0f;

            for (int step = 0; step < args->max_steps; step++) {
                update_pendulum(&p, DT, false);
                float current_speed = fabsf(p.omega1) + fabsf(p.omega2);
                if (current_speed > max_speed) max_speed = current_speed;
            }

            float intensity = fminf(max_speed / 15.0f, 1.0f);
            float brightness = 0.9f;
            if (intensity < 0.05f) {
                brightness = (intensity / 0.05f) * 0.9f;
            }

            Color pixel_color = ColorFromHSV(240.0f - (intensity * 240.0f), 0.9f, brightness);

            for (int dy = 0; dy < args->step_size; dy++) {
                for (int dx = 0; dx < args->step_size; dx++) {
                    if (y + dy < args->end_y && x + dx < width) {
                        pixels[(y + dy) * width + (x + dx)] = pixel_color;
                    }
                }
            }
        }
    }

    pthread_exit(NULL);
}

void generate_chaos_map_mt(ChaosFractal *cf, int step_size, int max_steps) {
    if (!cf) return;

    const int NUM_THREADS = 12;
    pthread_t threads[NUM_THREADS];
    FractalThreadArgs thread_args[NUM_THREADS];

    int height = cf->pixel_buffer.height;

    int rows_per_thread = height / NUM_THREADS;

    double start_time = GetTime();

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i].cf = cf;
        thread_args[i].step_size = step_size;
        thread_args[i].max_steps = max_steps;
        thread_args[i].start_y = i * rows_per_thread;

        if (i == NUM_THREADS - 1) {
            thread_args[i].end_y = height;
        } else {
            thread_args[i].end_y = (i + 1) * rows_per_thread;
        }

        pthread_create(&threads[i], NULL, chaos_map_worker_thread, &thread_args[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    UpdateTexture(cf->texture, cf->pixel_buffer.data);

    double end_time = GetTime();
    LOG_INFO("[SYS] Multi-threaded render (%d steps, step_size: %d) done in: %.4f seconds",
             max_steps, step_size, end_time - start_time);
}

void destroy_chaos_fractal(ChaosFractal *cf) {
    if (!cf) {
        LOG_ERROR("[RENDER] Null pointer exception -> ChaosFractal pointer 'f' is NULL in destroy_chaos_fractal()");
    }

    UnloadTexture(cf->texture);
    UnloadImage(cf->pixel_buffer);
    free(cf);
}


