#include "raylib.h"
#include "physics.h"
#include "constants.h"
#include "error.h"

#include <stdlib.h>


DoublePendulum *create_pendulum(void) {
    DoublePendulum *p = calloc(1, sizeof(DoublePendulum));
    if (!p) {
        log_error(ERR_ALLOCATION_FAILED, "create_pendulum", true);
        return NULL;
    }

    p->g = GRAVITY;
    p->l1 = BASE_LENGTH;
    p->l2 = BASE_LENGTH;
    p->m1 = BASE_MASS;
    p->m2 = BASE_MASS;

    p->theta1 = PI / 2.0f;
    p->theta2 = PI / 2.0f;

    p->omega1 = 0.0f;
    p->omega2 = 0.0f;

    p->is_paused = true;
    p->show_trail = false;

    return p;
}

void update_pendulum(DoublePendulum *p, float dt) {
    if (p->is_paused) return;

    // TODO: Implement the Lagrangian differential equations and RK4 integrator
}

void destroy_pendulum(DoublePendulum *p) {
    // TODO: null pointer check
    free(p);
}
