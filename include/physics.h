#ifndef DOUBLE_PENDULUM_PHYSICS_H
#define DOUBLE_PENDULUM_PHYSICS_H
#include <stdbool.h>

#include "raylib.h"
#include "constants.h"

typedef struct DoublePendulum {
    float l1, l2;
    float m1, m2;
    float g;
    float theta1, theta2;
    float omega1, omega2;
    Vector2 trail[TRAIL_MAX_LENGTH];
    int trail_count;
} DoublePendulum;


DoublePendulum *create_pendulum(void);

void display_pendulum(DoublePendulum *p, int origin_x, int origin_y);

void update_pendulum(DoublePendulum *p, float dt, bool record_trail);

void randomize_pendulum(DoublePendulum *p);

void destroy_pendulum(DoublePendulum *p);

#endif //DOUBLE_PENDULUM_PHYSICS_H
