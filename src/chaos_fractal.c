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

    LOG_INFO("[SYS] Planting %d double pendulums...", (width * height));

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

ChaosFractal *create_chaos_fractal(void) {
    ChaosFractal *cf = calloc(1, sizeof(ChaosFractal));
    if (!cf) {
        LOG_FATAL("[SYS] Memory allocation failed -> Target: ChaosFractal struct in create_chaos_fractal()");
        return NULL;
    }

    cf->pixel_buffer = GenImageColor(FRACTAL_BUFFER_WIDTH, FRACTAL_BUFFER_HEIGHT, BLACK);
    cf->texture = LoadTextureFromImage(cf->pixel_buffer);
    SetTextureFilter(cf->texture, TEXTURE_FILTER_BILINEAR);

    int total_pixels = FRACTAL_BUFFER_WIDTH * FRACTAL_BUFFER_HEIGHT;
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

static void *chaos_map_worker_live(void *arg) {
    FractalThreadArgs *args = (FractalThreadArgs *) arg;
    ChaosFractal *cf = args->cf;

    int width = cf->pixel_buffer.width;
    int height = cf->pixel_buffer.height;
    Color *pixels = (Color *) cf->pixel_buffer.data;
    const float DT = 0.016f;

    for (int y = args->start_y; y < args->end_y; y++) {
        for (int x = 0; x < width; x++) {
            int idx1 = y * width + x;
            int idx2 = (height - 1 - y) * width + (width - 1 - x); // origin-symmetric pixel

            DoublePendulum *p = &cf->pendulums[idx1];

            for (int step = 0; step < args->steps_per_frame; step++) {
                update_pendulum(p, DT, false);

                float current_speed = fabsf(p->omega1) + fabsf(p->omega2);
                if (current_speed > cf->max_speeds[idx1]) {
                    cf->max_speeds[idx1] = current_speed;
                }
            }

            cf->max_speeds[idx2] = cf->max_speeds[idx1];

            float speed = cf->max_speeds[idx1];
            Color pixel_color = BLACK;

            if (speed < VORTEX_THRESHOLD_DARK) {
                float brightness = (speed / VORTEX_THRESHOLD_DARK);
                pixel_color = ColorFromHSV(VORTEX_HUE, VORTEX_SATURATION_DEEP, brightness * VORTEX_BRIGHTNESS_DEEP);
            } else if (speed < VORTEX_THRESHOLD_GLOW) {
                float transition = (speed - VORTEX_THRESHOLD_DARK) / (VORTEX_THRESHOLD_GLOW - VORTEX_THRESHOLD_DARK);
                pixel_color = ColorFromHSV(VORTEX_HUE, VORTEX_SATURATION_DEEP,
                                           VORTEX_BRIGHTNESS_DEEP + (transition * (1.0f - VORTEX_BRIGHTNESS_DEEP)));
            } else {
                pixel_color = ColorFromHSV(VORTEX_HUE, 0.0f, 1.0f);
            }

            pixels[idx1] = pixel_color;
            pixels[idx2] = pixel_color;
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

    int half_height = cf->pixel_buffer.height / 2;
    int rows_per_thread = half_height / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i].cf = cf;
        thread_args[i].steps_per_frame = steps_per_frame;
        thread_args[i].start_y = i * rows_per_thread;
        thread_args[i].end_y = (i == NUM_THREADS - 1) ? half_height : (i + 1) * rows_per_thread;

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
