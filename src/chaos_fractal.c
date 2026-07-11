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

ChaosFractal *create_chaos_fractal(int width, int height) {
    ChaosFractal *cf = calloc(1, sizeof(ChaosFractal));
    if (!cf) {
        LOG_FATAL("[SYS] Memory allocation failed -> Target: ChaosFractal struct in create_chaos_fractal()");
        return NULL;
    }

    cf->pixel_buffer = GenImageColor(width, height, BLACK);
    cf->texture = LoadTextureFromImage(cf->pixel_buffer);

    SetTextureFilter(cf->texture, TEXTURE_FILTER_TRILINEAR);
    SetTextureWrap(cf->texture, TEXTURE_WRAP_REPEAT);

    cf->zoom = 1.0f;
    cf->offset_x = 0.0f;
    cf->offset_y = 0.0f;

    int total_pixels = width * height;
    cf->pendulums = calloc(total_pixels, sizeof(FractalPendulum));
    cf->max_speeds = calloc(total_pixels, sizeof(double));
    if (!cf->pendulums || !cf->max_speeds) {
        LOG_FATAL("[SYS] Memory allocation failed -> Target: FractalPendulum array in create_chaos_fractal()");
        return NULL;
    }

    cf->current_step = 0;
    cf->is_evolving = false;
    cf->frame_counter = 0;

    reset_chaos_fractal_state(cf);
    return cf;
}

static void *chaos_map_worker_live(void *arg) {
    FractalThreadArgs *args = (FractalThreadArgs *) arg;
    ChaosFractal *cf = args->cf;

    int width = cf->pixel_buffer.width;
    int height = cf->pixel_buffer.height;
    Color *pixels = (Color *) cf->pixel_buffer.data;

    // checkerboard rendering: calculate target pixel offset for this frame (0 to 3)
    int target_subframe = cf->frame_counter % 4;
    int target_dx = target_subframe % 2;
    int target_dy = target_subframe / 2;

    for (int y = args->start_y; y < args->end_y; y++) {
        if (y % 2 != target_dy) continue;

        for (int x = 0; x < width; x++) {
            if (x % 2 != target_dx) continue;

            int idx1 = y * width + x;

            FractalPendulum *p = &cf->pendulums[idx1];

            for (int step = 0; step < args->steps_per_frame; step++) {
                const double DT = 0.016; // TODO:
                update_fractal_pendulum(p, DT);

                const double current_speed = fabs(p->omega1) + fabs(p->omega2);
                if (current_speed > cf->max_speeds[idx1]) {
                    cf->max_speeds[idx1] = current_speed;
                }
            }

            float speed = (float) cf->max_speeds[idx1];
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
            if (cf->zoom == 1.0) {
                int idx2 = (height - 1 - y) * width + (width - 1 - x); // origin-symmetric pixel

                cf->max_speeds[idx2] = cf->max_speeds[idx1];
                pixels[idx2] = pixel_color;
            }
        }
    }

    pthread_exit(NULL);
}

void reset_chaos_fractal_state(ChaosFractal *cf) {
    if (!cf) {
        LOG_ERROR("[SYS] Null pointer exception -> ChaosFractal pointer 'cf' is NULL in resize_chaos_fractal()");
        return;
    }

    int width = cf->pixel_buffer.width;
    int height = cf->pixel_buffer.height;
    Color *pixels = (Color *) cf->pixel_buffer.data;

    LOG_INFO("[SYS] Planting %d double pendulums...", (width * height));

    double effective_zoom = (cf->zoom < 1.0) ? 1.0 : cf->zoom;
    double range = PI / effective_zoom;
    double phys_offset_x = (cf->zoom == 1.0) ? 0.0 : cf->offset_x;
    double phys_offset_y = (cf->zoom == 1.0) ? 0.0 : cf->offset_y;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = y * width + x;

            cf->pendulums[idx].theta1 = phys_offset_x + (((double) x / width) * 2.0 * range - range);
            cf->pendulums[idx].theta2 = phys_offset_y + (((double) y / height) * 2.0 * range - range);

            cf->pendulums[idx].omega1 = 0.0;
            cf->pendulums[idx].omega2 = 0.0;

            cf->max_speeds[idx] = 0.0f;

            pixels[idx] = BLACK;
        }
    }

    cf->current_step = 0;
    cf->zoom = 1.0;
    cf->offset_x = 0.0;
    cf->offset_y = 0.0;
    UpdateTexture(cf->texture, cf->pixel_buffer.data);
    LOG_INFO("[SYS] Chaos Fractal state reset ready.");
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
    cf->pendulums = calloc(total_pixels, sizeof(FractalPendulum));
    cf->max_speeds = calloc(total_pixels, sizeof(double));
    if (!cf->pendulums || !cf->max_speeds) {
        LOG_FATAL("[SYS] Memory allocation failed -> Target: FractalPendulum array in resize_chaos_fractal()");
        return;
    }

    cf->current_step = 0;
    cf->is_evolving = false;

    reset_chaos_fractal_state(cf);
}

void evolve_chaos_map_mt(ChaosFractal *cf, int steps_per_frame) {
    if (!cf) {
        LOG_ERROR("[SYS] Null pointer exception -> ChaosFractal pointer 'cf' is NULL in evolve_chaos_map_mt()");
        return;
    }

    pthread_t threads[NUM_THREADS];
    FractalThreadArgs thread_args[NUM_THREADS];

    int calc_height = (cf->zoom == 1.0) ? (cf->pixel_buffer.height / 2) : cf->pixel_buffer.height;
    int rows_per_thread = calc_height / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i].cf = cf;
        thread_args[i].steps_per_frame = steps_per_frame;
        thread_args[i].start_y = i * rows_per_thread;
        thread_args[i].end_y = (i == NUM_THREADS - 1) ? calc_height : (i + 1) * rows_per_thread;

        pthread_create(&threads[i], NULL, chaos_map_worker_live, &thread_args[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    cf->current_step += steps_per_frame;
    cf->frame_counter++;
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

void export_chaos_fractal_image(ChaosFractal *cf) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    const char *filename = TextFormat("chaos_fractal_%04d%02d%02d_%02d%02d%02d.png", t->tm_year + 1900,
                                      t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

    const int sw = GetScreenWidth();
    const int sh = GetScreenHeight();
    RenderTexture2D target = LoadRenderTexture(sw, sh);

    BeginTextureMode(target);
    ClearBackground(BLACK);
    Texture2D tex = cf->texture;
    float tiles = (cf->zoom < 1.0) ? (1.0 / cf->zoom) : 1.0;
    float center_x = (cf->offset_x / (2.0 * PI)) * tex.width + (tex.width / 2.0);
    float center_y = (cf->offset_y / (2.0 * PI)) * tex.height + (tex.height / 2.0);
    float src_x = center_x - ((tex.width * tiles) / 2.0);
    float src_y = center_y - ((tex.height * tiles) / 2.0);

    Rectangle src = {src_x, src_y, (float) tex.width * tiles, (float) tex.height * tiles};
    Rectangle dst = {0.0f, 0.0f, (float) sw, (float) sh};

    DrawTexturePro(tex, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
    EndTextureMode();

    Image img = LoadImageFromTexture(target.texture);
    ImageFlipVertical(&img);

    ExportImage(img, filename);
    UnloadImage(img);
    UnloadRenderTexture(target);

    LOG_INFO("[SYS] Fractal image saved to root directory: %s", filename);
}
