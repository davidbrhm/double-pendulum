#include "render.h"
#include "constants.h"
#include "raylib.h"
#include "logger.h"

#include <math.h>


void draw_pendulum(DoublePendulum *p, int origin_x, int origin_y) {
    if (!p) {
        LOG_ERROR("[RENDER] Null pointer exception -> Pendulum pointer 'p' is NULL in draw_pendulum()");
        return;
    }

    if (p->show_trail && p->trail_count > 1) {
        for (int i = 0; i < p->trail_count - 1; i++) {
            float alpha = 1.0f - ((float) i / (float) p->trail_count);

            Color trail_color = Fade(LIME, alpha);

            DrawLineV(
                (Vector2){p->trail[i].x + origin_x, p->trail[i].y + origin_y},
                (Vector2){p->trail[i + 1].x + origin_x, p->trail[i + 1].y + origin_y},
                trail_color
            );
        }
    }

    float x0 = (float) origin_x;
    float y0 = (float) origin_y;

    float x1 = x0 + p->l1 * sinf(p->theta1);
    float y1 = y0 + p->l1 * cosf(p->theta1);

    float x2 = x1 + p->l2 * sinf(p->theta2);
    float y2 = y1 + p->l2 * cosf(p->theta2);

    // trust the golden ratio
    float r1 = p->m1 * GOLDEN_RATIO;
    float r2 = p->m2 * GOLDEN_RATIO;

    // one color for the whole pendulum
    const Color body_color = (Color){45, 225, 115, 255};
    DrawLineEx((Vector2){x0, y0}, (Vector2){x1, y1}, ARM_THICKNESS, body_color);
    DrawLineEx((Vector2){x1, y1}, (Vector2){x2, y2}, ARM_THICKNESS, body_color);

    DrawCircleV((Vector2){x0, y0}, ARM_THICKNESS / 2.0f, body_color);

    DrawCircleV((Vector2){x1, y1}, r1, body_color);
    DrawCircleV((Vector2){x2, y2}, r2, body_color);

    float base_r = (r1 > r2) ? r1 : r2; // or simply r1
    DrawCircle(origin_x, origin_y, base_r, body_color);
}
