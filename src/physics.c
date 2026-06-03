#include "physics.h"
#include "logger.h"

#include <stdlib.h>
#include <math.h>

DoublePendulum *create_pendulum(void) {
    DoublePendulum *p = calloc(1, sizeof(DoublePendulum));
    if (!p) {
        LOG_FATAL("[SYS] Memory allocation failed -> Target: DoublePendulum struct in create_pendulum()");
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
    p->trail_count = 0;

    return p;
}

void display_pendulum(DoublePendulum *p, int origin_x, int origin_y) {
    // not implemented yet
}

/**
 * Calculates the angular accelerations (alpha1, alpha2) for a given state using Lagrangian mechanics
 * @param p  Pointer to the DoublePendulum instance
 * @param t1 Angle of the first arm (theta1)
 * @param t2 Angle of the second arm (theta2)
 * @param w1 Angular velocity of the first arm (omega1)
 * @param w2 Angular velocity of the second arm (omega2)
 * @param a1 Pointer to store the resulting acceleration of the first arm
 * @param a2 Pointer to store the resulting acceleration of the second arm
 */
static void calculate_accelerations(DoublePendulum *p, float t1, float t2, float w1, float w2, float *a1, float *a2) {
    float m1 = p->m1;
    float m2 = p->m2;
    float l1 = p->l1;
    float l2 = p->l2;
    float g = p->g;

    // Acceleration of the first arm
    float num1 = -g * (2 * m1 + m2) * sinf(t1);
    float num2 = -m2 * g * sinf(t1 - 2 * t2);
    float num3 = -2 * sinf(t1 - t2) * m2;
    float num4 = w2 * w2 * l2 + w1 * w1 * l1 * cosf(t1 - t2);
    float den1 = l1 * (2 * m1 + m2 - m2 * cosf(2 * t1 - 2 * t2));
    *a1 = (num1 + num2 + num3 * num4) / den1;

    // Acceleration of the second arm
    float num5 = 2 * sinf(t1 - t2);
    float num6 = w1 * w1 * l1 * (m1 + m2) + g * (m1 + m2) * cosf(t1) + w2 * w2 * l2 * m2 * cosf(t1 - t2);
    float den2 = l2 * (2 * m1 + m2 - m2 * cosf(2 * t1 - 2 * t2));
    *a2 = (num5 * num6) / den2;
}

/**
 * Updates the physical state of the double pendulum using RK4 integration
 * @param p Pointer to the DoublePendulum instance
 * @param dt Delta time (time step) in seconds
 */
void update_pendulum(DoublePendulum *p, float dt) {
    if (!p || p->is_paused) return;

    // 1. Store initial state
    float t1 = p->theta1;
    float t2 = p->theta2;
    float w1 = p->omega1;
    float w2 = p->omega2;

    float a1, a2;

    // ==========================================
    // RUNGE-KUTTA 4 (RK4) INTEGRATOR
    // Note: Implemented using Loop Unrolling for maximum CPU performance.
    // Avoids branching and array lookups by manually expanding the 4 steps.
    // ==========================================

    // Step 1: Evaluate at initial state
    calculate_accelerations(p, t1, t2, w1, w2, &a1, &a2);
    float k1_t1 = w1 * dt;
    float k1_t2 = w2 * dt;
    float k1_w1 = a1 * dt;
    float k1_w2 = a2 * dt;

    // Step 2: Evaluate at midpoint (using Step 1)
    calculate_accelerations(p, t1 + k1_t1 * 0.5f, t2 + k1_t2 * 0.5f, w1 + k1_w1 * 0.5f, w2 + k1_w2 * 0.5f, &a1, &a2);
    float k2_t1 = (w1 + k1_w1 * 0.5f) * dt;
    float k2_t2 = (w2 + k1_w2 * 0.5f) * dt;
    float k2_w1 = a1 * dt;
    float k2_w2 = a2 * dt;

    // Step 3: Evaluate at midpoint (using Step 2)
    calculate_accelerations(p, t1 + k2_t1 * 0.5f, t2 + k2_t2 * 0.5f, w1 + k2_w1 * 0.5f, w2 + k2_w2 * 0.5f, &a1, &a2);
    float k3_t1 = (w1 + k2_w1 * 0.5f) * dt;
    float k3_t2 = (w2 + k2_w2 * 0.5f) * dt;
    float k3_w1 = a1 * dt;
    float k3_w2 = a2 * dt;

    // Step 4: Evaluate at end of step (using Step 3)
    calculate_accelerations(p, t1 + k3_t1, t2 + k3_t2, w1 + k3_w1, w2 + k3_w2, &a1, &a2);
    float k4_t1 = (w1 + k3_w1) * dt;
    float k4_t2 = (w2 + k3_w2) * dt;
    float k4_w1 = a1 * dt;
    float k4_w2 = a2 * dt;


    // Weighted average
    p->theta1 += (k1_t1 + 2.0f * k2_t1 + 2.0f * k3_t1 + k4_t1) / 6.0f;
    p->theta2 += (k1_t2 + 2.0f * k2_t2 + 2.0f * k3_t2 + k4_t2) / 6.0f;
    p->omega1 += (k1_w1 + 2.0f * k2_w1 + 2.0f * k3_w1 + k4_w1) / 6.0f;
    p->omega2 += (k1_w2 + 2.0f * k2_w2 + 2.0f * k3_w2 + k4_w2) / 6.0f;


    // Update trial
    if (p->show_trail) {
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

void randomize_pendulum(DoublePendulum *p) { // (T1: 3.42, T2: 2.44)
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
