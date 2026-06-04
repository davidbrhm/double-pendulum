#include "butterfly_effect.h"
#include "logger.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define DIFFERENCE 0.0001f

ButterflyEffect *create_butterfly_effect(void) {
    ButterflyEffect *effect = calloc(1, sizeof(ButterflyEffect));
    if (!effect) {
        LOG_FATAL("[SYS] Memory allocation failed -> Target: ButterflyEffect struct in create_butterfly_effect()");
        return NULL;
    }

    effect->pendulums = malloc(SWARM_SIZE * sizeof(DoublePendulum));
    if (!effect->pendulums) {
        LOG_FATAL("[SYS] Memory allocation failed -> Target: DoublePendulum struct in create_butterfly_effect()");
        return NULL;
    }

    effect->is_paused = false;
    effect->camera_angle = 0.0f;

    float center_z = ((SWARM_SIZE - 1) * GAP) / 2.0f;
    effect->camera.position = (Vector3){0.0f, 0.0f, center_z + CAMERA_RADIUS};
    effect->camera.target = (Vector3){0.0f, 0.0f, center_z};
    effect->camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    effect->camera.fovy = 30.0f;
    effect->camera.projection = CAMERA_PERSPECTIVE;

    for (int i = 0; i < SWARM_SIZE; i++) {
        effect->pendulums[i].l1 = 30.0f;
        effect->pendulums[i].l2 = 30.0f;
        effect->pendulums[i].m1 = 10.0f;
        effect->pendulums[i].m2 = 10.0f;

        effect->pendulums[i].g = GRAVITY;

        effect->pendulums[i].theta1 = (PI / 2.0f) + (i * DIFFERENCE);
        effect->pendulums[i].theta2 = PI / 2.0f;
        effect->pendulums[i].omega1 = 0.0f;
        effect->pendulums[i].omega2 = 0.0f;

        effect->pendulums[i].show_trail = false;
        effect->pendulums[i].is_paused = false;
    }

    return effect;
}

void update_butterfly_effect(ButterflyEffect *effect, float dt) {
    if (!effect || effect->is_paused) return;

    for (int i = 0; i < SWARM_SIZE; i++) {
        update_pendulum(&effect->pendulums[i], dt);
    }
}

void draw_butterfly_effect(const ButterflyEffect *effect) {
    if (!effect) return;

    BeginMode3D(effect->camera);
    Vector3 main_origin = {0.0f, 0.0f, 0.0f};
    Vector3 main_end = {0.0f, 0.0f, (SWARM_SIZE - 1) * GAP};
    DrawCylinderEx(main_origin, main_end, 1.0f, 1.0f, 8, DARKGRAY);

    for (int i = 0; i < SWARM_SIZE; i++) {
        DoublePendulum *p = &effect->pendulums[i];

        Vector3 current_origin = {0.0f, 0.0f, i * GAP};

        Vector3 p1 = {
            p->l1 * sinf(p->theta1),
            -p->l1 * cosf(p->theta1),
            i * GAP
        };

        Vector3 p2 = {
            p1.x + p->l2 * sinf(p->theta2),
            p1.y - p->l2 * cosf(p->theta2),
            i * GAP
        };

        Color color = ColorFromHSV((float) i / SWARM_SIZE * 120.0f + 140.0f, 0.8f, 0.9f);
        color.a = 200;

        DrawCylinderEx(current_origin, p1, 3.0f, 3.0f, 8, color);
        DrawCylinderEx(p1, p2, 3.0f, 3.0f, 8, color);

        DrawSphere(current_origin, 4.0f, color);
        DrawSphere(p1, 6.0f, color);
        DrawSphere(p2, 6.0f, color);
    }

    DrawGrid(20, 50.0f);
    EndMode3D();
}

void destroy_butterfly_effect(ButterflyEffect *effect) {
    if (!effect) {
        LOG_ERROR("[SYS] Null pointer exception -> ButterflyEffect pointer is NULL in destroy_butterfly_effect()");
        return;
    }

    if (!effect->pendulums) {
        LOG_ERROR("[SYS] Null pointer exception -> DoublePendulum array pointer is NULL in destroy_butterfly_effect()");
    } else {
        free(effect->pendulums);
    }

    free(effect);
}
