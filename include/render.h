#ifndef DOUBLE_PENDULUM_RENDER_H
#define DOUBLE_PENDULUM_RENDER_H

#include "physics.h"
#include "ui.h"

void draw_pendulum(const DoublePendulum *p, int origin_x, int origin_y, bool show_trail, bool show_only_trails);

#endif //DOUBLE_PENDULUM_RENDER_H
