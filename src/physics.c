#include "physics.h"

#include <assert.h>

#include "logger.h"

#include <stdlib.h>
#include <math.h>

#include "chaos_fractal.h"

DoublePendulum *create_pendulum(void) {
    DoublePendulum *p = calloc(1, sizeof(DoublePendulum));
    if (!p) {
        LOG_FATAL("[SYS] Memory allocation failed -> Target: DoublePendulum struct in create_pendulum()");
        return NULL;
    }

    p->g = GRAVITY;
    p->l1 = BASE_LENGTH; // TODO: move consts!
    p->l2 = BASE_LENGTH;
    p->m1 = BASE_MASS;
    p->m2 = BASE_MASS;

    p->theta1 = PI / 2.0f;
    p->theta2 = PI / 2.0f;

    p->omega1 = 0.0f;
    p->omega2 = 0.0f;

    p->trail_count = 0;

    return p;
}

void display_pendulum(DoublePendulum *p, int origin_x, int origin_y) {
    // not implemented yet
}

/** // TODO: rewrite!
 * Calculates the angular accelerations (alpha1, alpha2) for a given state using Lagrangian mechanics
 * @param p  Pointer to the DoublePendulum instance
 * @param t1 Angle of the first arm (theta1)
 * @param t2 Angle of the second arm (theta2)
 * @param w1 Angular velocity of the first arm (omega1)
 * @param w2 Angular velocity of the second arm (omega2)
 * @param a1 Pointer to store the resulting acceleration of the first arm
 * @param a2 Pointer to store the resulting acceleration of the second arm
 */
static void calculate_accelerations(double t1, double t2, double w1, double w2,
                                    double m1, double m2, double l1, double l2, double g,
                                    double *alpha1, double *alpha2) {
    // Acceleration of the first arm
    double num1 = -g * (2 * m1 + m2) * sin(t1);
    double num2 = -m2 * g * sin(t1 - 2 * t2);
    double num3 = -2 * sin(t1 - t2) * m2;
    double num4 = w2 * w2 * l2 + w1 * w1 * l1 * cos(t1 - t2);
    double den1 = l1 * (2 * m1 + m2 - m2 * cos(2 * t1 - 2 * t2));
    *alpha1 = (num1 + num2 + num3 * num4) / den1;

    // Acceleration of the second arm
    double num5 = 2 * sin(t1 - t2);
    double num6 = w1 * w1 * l1 * (m1 + m2) + g * (m1 + m2) * cos(t1) + w2 * w2 * l2 * m2 * cos(t1 - t2);
    double den2 = l2 * (2 * m1 + m2 - m2 * cos(2 * t1 - 2 * t2));
    *alpha2 = (num5 * num6) / den2;
}

/**
 * Performs a single Runge-Kutta 4 (RK4) integration step to update the pendulum's state.
 * Implemented using Loop Unrolling for maximum CPU performance, avoiding branching and arrays.
 *
 * @param theta1 Pointer to the first arm's angle
 * @param theta2 Pointer to the second arm's angle
 * @param omega1 Pointer to the first arm's angular velocity
 * @param omega2 Pointer to the second arm's angular velocity
 * @param m1 Mass of the first arm
 * @param m2 Mass of the second arm
 * @param l1 Length of the first arm
 * @param l2 Length of the second arm
 * @param g Gravitational acceleration constant
 * @param dt Delta time (time step) in seconds
 */
static void rk4_step(double *theta1, double *theta2, double *omega1, double *omega2, double m1, double m2, double l1,
                     double l2, double g, double dt) {
    double t1 = *theta1, t2 = *theta2;
    double w1 = *omega1, w2 = *omega2;
    double a1, a2;

    // Step 1: Evaluate at initial state
    calculate_accelerations(t1, t2, w1, w2, m1, m2, l1, l2, g, &a1, &a2);
    double k1_t1 = w1 * dt;
    double k1_t2 = w2 * dt;
    double k1_w1 = a1 * dt;
    double k1_w2 = a2 * dt;

    // Step 2: Evaluate at midpoint (using Step 1)
    calculate_accelerations(t1 + k1_t1 * 0.5, t2 + k1_t2 * 0.5, w1 + k1_w1 * 0.5, w2 + k1_w2 * 0.5, m1, m2, l1, l2, g,
                            &a1, &a2);
    double k2_t1 = (w1 + k1_w1 * 0.5) * dt;
    double k2_t2 = (w2 + k1_w2 * 0.5) * dt;
    double k2_w1 = a1 * dt;
    double k2_w2 = a2 * dt;

    // Step 3: Evaluate at midpoint (using Step 2)
    calculate_accelerations(t1 + k2_t1 * 0.5, t2 + k2_t2 * 0.5, w1 + k2_w1 * 0.5, w2 + k2_w2 * 0.5, m1, m2, l1, l2, g,
                            &a1, &a2);
    double k3_t1 = (w1 + k2_w1 * 0.5) * dt;
    double k3_t2 = (w2 + k2_w2 * 0.5) * dt;
    double k3_w1 = a1 * dt;
    double k3_w2 = a2 * dt;

    // Step 4: Evaluate at end of step (using Step 3)
    calculate_accelerations(t1 + k3_t1, t2 + k3_t2, w1 + k3_w1, w2 + k3_w2, m1, m2, l1, l2, g, &a1, &a2);
    double k4_t1 = (w1 + k3_w1) * dt;
    double k4_t2 = (w2 + k3_w2) * dt;
    double k4_w1 = a1 * dt;
    double k4_w2 = a2 * dt;

    // Weighted average
    *theta1 += (k1_t1 + 2.0 * k2_t1 + 2.0 * k3_t1 + k4_t1) / 6.0;
    *theta2 += (k1_t2 + 2.0 * k2_t2 + 2.0 * k3_t2 + k4_t2) / 6.0;
    *omega1 += (k1_w1 + 2.0 * k2_w1 + 2.0 * k3_w1 + k4_w1) / 6.0;
    *omega2 += (k1_w2 + 2.0 * k2_w2 + 2.0 * k3_w2 + k4_w2) / 6.0;
}

/**
 * Updates the physical state of the standard visual double pendulum.
 *
 * @param p Pointer to the DoublePendulum instance
 * @param dt Delta time (time step) in seconds
 * @param record_trail Boolean flag to determine if trail coordinates should be updated
 */
void update_pendulum(DoublePendulum *p, float dt, bool record_trail) {
    assert(p != NULL && "Pendulum pointer is null in update_pendulum!");

    double t1 = p->theta1;
    double t2 = p->theta2;
    double w1 = p->omega1;
    double w2 = p->omega2;

    rk4_step(&t1, &t2, &w1, &w2, p->m1, p->m2, p->l1, p->l2, p->g, dt);

    p->theta1 = (float)t1;
    p->theta2 = (float)t2;
    p->omega1 = (float)w1;
    p->omega2 = (float)w2;


    // Update trial
    if (record_trail) {
        float x1 = p->l1 * sinf(p->theta1);
        float y1 = p->l1 * cosf(p->theta1);
        float x2 = x1 + p->l2 * sinf(p->theta2);
        float y2 = y1 + p->l2 * cosf(p->theta2);

        Vector2 current_pos = {x2, y2};

        int max_shift = (p->trail_count < TRAIL_MAX_LENGTH) ? p->trail_count : TRAIL_MAX_LENGTH - 1;
        for (int i = max_shift; i > 0; i--) {
            p->trail[i] = p->trail[i - 1];
        }

        p->trail[0] = current_pos;

        if (p->trail_count < TRAIL_MAX_LENGTH) {
            p->trail_count++;
        }
    }
}

void randomize_pendulum(DoublePendulum *p) {
    if (!p) {
        LOG_ERROR("[SYS] Null pointer exception -> Pendulum pointer 'p' is NULL in randomize_pendulum()");
        return;
    }

    p->theta1 = (GetRandomValue(0, 360) * PI) / 180.0f;
    p->theta2 = (GetRandomValue(0, 360) * PI) / 180.0f;
    p->omega1 = p->omega2 = 0.f;
    p->trail_count = 0;

    LOG_INFO("[INPUT] Key 'R' pressed -> Pendulum randomized (T1: %.2f, T2: %.2f)", p->theta1, p->theta2);
}

void destroy_pendulum(DoublePendulum *p) {
    if (!p) {
        LOG_ERROR("[SYS] Null pointer exception -> Pendulum pointer 'p' is NULL in destroy_pendulum()");
        return;
    }

    free(p);
}

/**
 * High-performance update function for fractal generation.
 * Uses 64-bit double precision and fixed global parameters to maximize cache locality.
 *
 * @param p Pointer to the FractalPendulum struct
 * @param dt Delta time (time step) in seconds
 */
void update_fractal_pendulum(FractalPendulum *p, double dt) {
    assert(p != NULL && "FractalPendulum pointer is null!");

    rk4_step(&p->theta1, &p->theta2, &p->omega1, &p->omega2,
             FRACTAL_M1, FRACTAL_M2, FRACTAL_L1, FRACTAL_L2, FRACTAL_G, dt);
}
