#ifndef DOUBLE_PENDULUM_BUTTERFLY_EFFECT_H
#define DOUBLE_PENDULUM_BUTTERFLY_EFFECT_H

#include "raylib.h"
#include "physics.h"

#define SWARM_SIZE 10

typedef struct ButterflyEffect {
    DoublePendulum *pendulums;
    Camera3D camera;
    float camera_angle;
    bool is_paused;
} ButterflyEffect;

ButterflyEffect *create_butterfly_effect(void);

void update_butterfly_effect(ButterflyEffect *effect, float dt);

void draw_butterfly_effect(const ButterflyEffect *effect);

void destroy_butterfly_effect(ButterflyEffect *effect);

#endif //DOUBLE_PENDULUM_BUTTERFLY_EFFECT_H
