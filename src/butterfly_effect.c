#include "butterfly_effect.h"
#include "logger.h"
#include "rlgl.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

static void update_butterfly_camera_position(ButterflyEffect *effect) {
    const float center_z = ((BF_SWARM_SIZE - 1) * BF_GAP) / 2.0f;
    const float total_length = fabsf((BF_SWARM_SIZE - 1) * BF_GAP);
    const float dynamic_radius = BF_CAM_BASE_DIST + (total_length * BF_CAM_LENGTH_MULT);

    effect->camera.position.x = sinf(effect->camera_angle) * dynamic_radius; // fine tune it
    effect->camera.position.y = BF_CAM_Y_OFFSET;
    effect->camera.position.z = center_z + (cosf(effect->camera_angle) * dynamic_radius);

    effect->camera.target = (Vector3){0.0f, BF_CAM_TARGET_Y, center_z};
}


ButterflyEffect *create_butterfly_effect(void) {
    ButterflyEffect *effect = calloc(1, sizeof(ButterflyEffect));
    if (!effect) return NULL;

    // effect->pendulums = malloc(BF_SWARM_SIZE * sizeof(DoublePendulum));
    effect->pendulums = calloc(BF_SWARM_SIZE, sizeof(DoublePendulum));
    if (!effect->pendulums) return NULL;

    effect->camera_angle = 0.0f;

    effect->camera_angle = 0.0f;
    effect->camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    effect->camera.fovy = BF_CAM_FOV;
    effect->camera.projection = CAMERA_PERSPECTIVE;
    update_butterfly_camera_position(effect);

    const float t1 = (GetRandomValue(0, 360) * PI) / 180.0f;
    const float t2 = (GetRandomValue(0, 360) * PI) / 180.0f;
    for (int i = 0; i < BF_SWARM_SIZE; i++) {
        effect->pendulums[i].l1 = 150.0f;
        effect->pendulums[i].l2 = 150.0f;
        effect->pendulums[i].m1 = 10.0f;
        effect->pendulums[i].m2 = 10.0f;
        effect->pendulums[i].g = GRAVITY;

        //effect->pendulums[i].theta1 = (PI / 2.0f) + (i * DIFFERENCE);
        //effect->pendulums[i].theta2 = PI / 2.0f;
        effect->pendulums[i].theta1 = t1 + (i * BF_DIFFERENCE);
        effect->pendulums[i].theta2 = t2;

        effect->pendulums[i].omega1 = 0.0f;
        effect->pendulums[i].omega2 = 0.0f;
        effect->pendulums[i].trail_count = 0;
    }

    return effect;
}

void update_butterfly_effect(ButterflyEffect *effect, float dt, bool is_paused, bool show_trail) {
    if (!effect) {
        LOG_ERROR(
            "[SYS] Null pointer exception -> ButterflyEffect pointer 'effect' is NULL in update_butterfly_effect()");
        return;
    }

    update_butterfly_camera_position(effect);

    if (is_paused) return;
    for (int i = 0; i < BF_SWARM_SIZE; i++) {
        update_pendulum(&effect->pendulums[i], dt, show_trail);
    }
}

void draw_butterfly_effect(const ButterflyEffect *effect, bool show_trail, bool show_only_trails) {
    if (!effect) {
        LOG_ERROR("[SYS] Null pointer exception -> ButterflyEffect pointer is NULL in draw_butterfly_effect()");
        return;
    }

    rlSetClipPlanes(BF_CLIP_NEAR, BF_CLIP_FAR);

    BeginMode3D(effect->camera);
    Vector3 main_origin = {0.0f, 0.0f, 0.0f};
    Vector3 main_end = {0.0f, 0.0f, (BF_SWARM_SIZE - 1) * BF_GAP};
    DrawCylinderEx(main_origin, main_end, 1.0f, 1.0f, 8, DARKGRAY);

    for (int i = 0; i < BF_SWARM_SIZE; i++) {
        DoublePendulum *p = &effect->pendulums[i];

        // trail drawing
        if (show_trail) {
            // !
            const float z = i * BF_GAP;
            for (int j = 0; j < p->trail_count - 1; j++) {
                float alpha = 1.0f - ((float) j / (float) p->trail_count);

                Color trail_color = ColorFromHSV((float) i / BF_SWARM_SIZE * BF_COLOR_HUE_RANGE + BF_COLOR_HUE_OFFSET,
                                                 0.8f, 0.9f);
                trail_color.a = (unsigned char) (alpha * 255);

                Vector3 start_point = {p->trail[j].x, -p->trail[j].y, z};
                Vector3 end_point = {p->trail[j + 1].x, -p->trail[j + 1].y, z};

                DrawLine3D(start_point, end_point, trail_color);
            }
        }

        if (!show_only_trails) {
            Vector3 current_origin = {0.0f, 0.0f, i * BF_GAP};

            Vector3 p1 = {
                p->l1 * sinf(p->theta1),
                -p->l1 * cosf(p->theta1),
                i * BF_GAP
            };

            Vector3 p2 = {
                p1.x + p->l2 * sinf(p->theta2),
                p1.y - p->l2 * cosf(p->theta2),
                i * BF_GAP
            };

            Color color = ColorFromHSV((float) i / BF_SWARM_SIZE * 120.0f + 140.0f, 0.8f, 0.9f);
            color.a = 255;

            DrawCylinderEx(current_origin, p1, BF_LINK_RADIUS, BF_LINK_RADIUS, 8, color);
            DrawCylinderEx(p1, p2, BF_LINK_RADIUS, BF_LINK_RADIUS, 8, color);

            DrawSphere(current_origin, BF_LINK_RADIUS, color);
            DrawSphere(p1, BF_NODE_RADIUS, color);
            DrawSphere(p2, BF_NODE_RADIUS, color);
        }
    }

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
