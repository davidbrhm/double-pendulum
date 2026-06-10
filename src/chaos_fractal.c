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
    int steps_per_frame;
} FractalThreadArgs;

void reset_chaos_fractal_state(ChaosFractal *cf) {
    if (!cf) {
        LOG_ERROR("[SYS] Null pointer exception -> ChaosFractal pointer 'cf' is NULL in resize_chaos_fractal()");
        return;
    }

    int width = cf->pixel_buffer.width;
    int height = cf->pixel_buffer.height;
    Color *pixels = (Color *) cf->pixel_buffer.data;

    LOG_INFO("[SYS] Planting ~640,000 double pendulums...");

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = y * width + x;

            cf->pendulums[idx].m1 = 10.0f;
            cf->pendulums[idx].m2 = 10.0f;
            cf->pendulums[idx].l1 = 100.0f;
            cf->pendulums[idx].l2 = 100.0f;
            cf->pendulums[idx].g = 9.81f;

            cf->pendulums[idx].theta1 = ((float) x / width) * 2.0f * PI - PI;
            cf->pendulums[idx].theta2 = ((float) y / height) * 2.0f * PI - PI;

            cf->pendulums[idx].omega1 = 0.0f;
            cf->pendulums[idx].omega2 = 0.0f;
            cf->max_speeds[idx] = 0.0f;

            pixels[idx] = BLACK;
        }
    }

    cf->current_step = 0;
    UpdateTexture(cf->texture, cf->pixel_buffer.data);
    LOG_INFO("[SYS] Chaos Fractal state reset ready.");
}

ChaosFractal *create_chaos_fractal(const int width, const int height) {
    ChaosFractal *cf = calloc(1, sizeof(ChaosFractal));
    if (!cf) {
        LOG_FATAL("[SYS] Memory allocation failed -> Target: ChaosFractal struct in create_chaos_fractal()");
        return NULL;
    }

    cf->pixel_buffer = GenImageColor(width, height, BLACK);
    cf->texture = LoadTextureFromImage(cf->pixel_buffer);

    int total_pixels = width * height;
    cf->pendulums = calloc(total_pixels, sizeof(DoublePendulum));
    cf->max_speeds = calloc(total_pixels, sizeof(float));
    if (!cf->pendulums || !cf->max_speeds) {
        LOG_FATAL("[SYS] Memory allocation failed -> Target: DoublePendulum array in create_chaos_fractal()");
        return NULL;
    }

    cf->current_step = 0;
    cf->is_evolving = false;

    reset_chaos_fractal_state(cf);
    return cf;
}

void resize_chaos_fractal(ChaosFractal *cf, int new_width, int new_height) {
    if (!cf) {
        LOG_ERROR("[SYS] Null pointer exception -> ChaosFractal pointer 'cf' is NULL in resize_chaos_fractal()");
        return;
    }

    UnloadTexture(cf->texture);
    UnloadImage(cf->pixel_buffer);
    free(cf->pendulums);
    free(cf->max_speeds);

    cf->pixel_buffer = GenImageColor(new_width, new_height, BLACK);
    cf->texture = LoadTextureFromImage(cf->pixel_buffer);

    int total_pixels = new_width * new_height;
    cf->pendulums = calloc(total_pixels, sizeof(DoublePendulum));
    cf->max_speeds = calloc(total_pixels, sizeof(float));

    cf->current_step = 0;
    cf->is_evolving = false;

    reset_chaos_fractal_state(cf);
}

static void *chaos_map_worker_live(void *arg) {
    FractalThreadArgs *args = (FractalThreadArgs *) arg;
    ChaosFractal *cf = args->cf;

    int width = cf->pixel_buffer.width;
    Color *pixels = (Color *) cf->pixel_buffer.data;
    const float DT = 0.016f;

    for (int y = args->start_y; y < args->end_y; y++) {
        for (int x = 0; x < width; x++) {
            int idx = y * width + x;
            DoublePendulum *p = &cf->pendulums[idx];

            for (int step = 0; step < args->steps_per_frame; step++) {
                update_pendulum(p, DT, false);

                float current_speed = fabsf(p->omega1) + fabsf(p->omega2);
                if (current_speed > cf->max_speeds[idx]) {
                    cf->max_speeds[idx] = current_speed;
                }
            }

            float intensity = fminf(cf->max_speeds[idx] / FRACTAL_MAX_SPEED_THRESHOLD, 1.0f);

            float hue = FRACTAL_HUE_COLD - (intensity * (FRACTAL_HUE_COLD - FRACTAL_HUE_HOT));

            float brightness = FRACTAL_BASE_BRIGHTNESS;
            if (intensity < FRACTAL_DARK_THRESHOLD) {
                brightness = (intensity / FRACTAL_DARK_THRESHOLD) * FRACTAL_BASE_BRIGHTNESS;
            }

            pixels[idx] = ColorFromHSV(hue, FRACTAL_BASE_SATURATION, brightness);
        }
    }
    pthread_exit(NULL);
}

void evolve_chaos_map_mt(ChaosFractal *cf, int steps_per_frame) {
    if (!cf) {
        LOG_ERROR("[SYS] Null pointer exception -> ChaosFractal pointer 'cf' is NULL in evolve_chaos_map_mt()");
        return;
    }

    const int NUM_THREADS = 12;
    pthread_t threads[NUM_THREADS];
    FractalThreadArgs thread_args[NUM_THREADS];

    int height = cf->pixel_buffer.height;
    int rows_per_thread = height / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i].cf = cf;
        thread_args[i].steps_per_frame = steps_per_frame;
        thread_args[i].start_y = i * rows_per_thread;
        thread_args[i].end_y = (i == NUM_THREADS - 1) ? height : (i + 1) * rows_per_thread;

        pthread_create(&threads[i], NULL, chaos_map_worker_live, &thread_args[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    cf->current_step += steps_per_frame;
    UpdateTexture(cf->texture, cf->pixel_buffer.data);
}

void destroy_chaos_fractal(ChaosFractal *cf) {
    if (!cf) {
        LOG_ERROR("[SYS] Null pointer exception -> ChaosFractal pointer 'cf' is NULL in destroy_chaos_fractal()");
        return;
    }

    if (cf->pendulums) free(cf->pendulums);
    if (cf->max_speeds) free(cf->max_speeds);

    UnloadTexture(cf->texture);
    UnloadImage(cf->pixel_buffer);
    free(cf);
}
