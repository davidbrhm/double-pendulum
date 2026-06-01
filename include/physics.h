#ifndef DOUBLE_PENDULUM_PHYSICS_H
#define DOUBLE_PENDULUM_PHYSICS_H
#include <stdbool.h>

typedef struct DoublePendulum {
    float l1, l2;
    float m1, m2;
    float g;

    float theta1, theta2;
    float omega1, omega2;

    bool is_paused;
    bool show_trail;

    // TODO: trail buffer array
} DoublePendulum;


DoublePendulum *create_pendulum(void);

void update_pendulum(DoublePendulum *p, float dt);

void destroy_pendulum(DoublePendulum *p);

#endif //DOUBLE_PENDULUM_PHYSICS_H
