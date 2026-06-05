#ifndef DOUBLE_PENDULUM_BUTTERFLY_EFFECT_H
#define DOUBLE_PENDULUM_BUTTERFLY_EFFECT_H

#include "raylib.h"
#include "physics.h"
#include "constants.h"

// simulation constants
#define BF_SWARM_SIZE 200
#define BF_GAP (-3.5f * GOLDEN_RATIO)
#define BF_DIFFERENCE 0.0001f

// render constants
#define BF_NODE_RADIUS 6.0f
#define BF_LINK_RADIUS 3.0f
#define BF_AXLE_RADIUS 1.0f
#define BF_COLOR_HUE_RANGE 120.0f
#define BF_COLOR_HUE_OFFSET 140.0f

// camera constants
#define BF_CAM_BASE_DIST 600.0f
#define BF_CAM_LENGTH_MULT 0.5f
#define BF_CAM_Y_OFFSET (-50.0f)
#define BF_CAM_TARGET_Y (-150.0f)
#define BF_CAM_FOV 45.0f
#define BF_CAM_ROT_SPEED 0.02f
#define BF_CLIP_NEAR 0.01f
#define BF_CLIP_FAR 10000.0f

typedef struct ButterflyEffect {
    DoublePendulum *pendulums;
    Camera3D camera;
    float camera_angle;
} ButterflyEffect; // TODO: FPS + center

ButterflyEffect *create_butterfly_effect(void);

void update_butterfly_effect(ButterflyEffect *effect, float dt, bool is_paused);

void draw_butterfly_effect(const ButterflyEffect *effect);

void destroy_butterfly_effect(ButterflyEffect *effect);

#endif //DOUBLE_PENDULUM_BUTTERFLY_EFFECT_H
