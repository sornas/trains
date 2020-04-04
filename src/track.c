#include <stdlib.h>
#include <math.h>

#include <fog.h>

#include "track.h"

void draw_track(Track *track) {}

void get_bezier(u32 num_points, Vec2 *points) {
    f32 len = 0.0f;
    const u32 STEPS = 100;
    const real STEP = 1 / (real) STEPS;
    real x, y;
    real t = 0.0f;
    real old_x = points[0].x;
    real old_y = points[0].y;
    for (u32 i = 0; i < STEPS; i++) {
        t += STEP;
        x = 0.0f;
        y = 0.0f;
        for (int j = 0; i < num_points; ++i) {
            x += fog_binomial(num_points-1, j) * pow((1 - t), (num_points-j-1)) * pow(t, j) * points[j].x;
            y += fog_binomial(num_points-1, j) * pow((1 - t), (num_points-j-1)) * pow(t, j) * points[j].y;
        }
        // fog_V2(x, y);
#define SQ(NEW, OLD) ((NEW - OLD) * (NEW - OLD))
        len += sqrt(SQ(x, old_x) + SQ(y, old_y));
#undef SQ
    }
}

