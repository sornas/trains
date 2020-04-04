#include <stdlib.h>
#include <math.h>

#include <fog.h>

#include "track.h"

Point *add_point(Segment *to_segment, Vec2 point) {
    to_segment->num_points++;
    to_segment->points = realloc(to_segment->points, to_segment->num_points * sizeof(Point));
    to_segment->points[to_segment->num_points - 1] = (Point) { point, to_segment };
    return &to_segment->points[to_segment->num_points - 1];
}

Segment *add_segment(Track *to_track) {
    to_track->num_segments++;
    to_track->segments = realloc(to_track->segments, to_track->num_segments * sizeof(Segment));
    to_track->segments[to_track->num_segments - 1] = (Segment) { 0, NULL };
    return &to_track->segments[to_track->num_segments - 1];
}

void draw_track(Track *track) {
    for (u32 i = 0; i < track->num_segments; i++) {
        Segment segment = track->segments[i];
        if (segment.num_points < 2) continue;

        push_bezier(segment);
        //fog_renderer_push_line(0, segment.points[0].point, segment.points[segment.num_points-1].point, fog_V4(0, 0, 0, 1), TRACK_POINT_SIZE/2);
        fog_renderer_push_point(0, segment.points[0].point, TRACK_ENDPOINT_COLOR, TRACK_POINT_SIZE);
        fog_renderer_push_point(0, segment.points[segment.num_points-1].point, TRACK_ENDPOINT_COLOR, TRACK_POINT_SIZE);

        for (u32 j = 1; j < segment.num_points-1; j++) {
            fog_renderer_push_point(0, segment.points[j].point, TRACK_CONTROLPOINT_COLOR, TRACK_POINT_SIZE);
        }
    }
}

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
    return;
}

