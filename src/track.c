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

//TODO(gu) bezier-struct, call once and save all points and length
void push_bezier(Segment segment) {
    f32 len = 0.0f;
    const real STEP = 0.01;
    real old_x = segment.points[0].point.x;
    real old_y = segment.points[0].point.y;
    for (real t = STEP; t <= 1.0; t += STEP) {
        real x = 0.0;
        real y = 0.0;
        for (int i = 0; i < segment.num_points; ++i) {
            x += fog_binomial(segment.num_points-1, i) * pow((1 - t), (segment.num_points-i-1)) * pow(t, i) * segment.points[i].point.x;
            y += fog_binomial(segment.num_points-1, i) * pow((1 - t), (segment.num_points-i-1)) * pow(t, i) * segment.points[i].point.y;
        }
        len += sqrt(x*x + y*y);
        fog_renderer_push_line(0, fog_V2(old_x, old_y), fog_V2(x, y), TRACK_ENDPOINT_COLOR, TRACK_POINT_SIZE/2);
        old_x = x;
        old_y = y;
    }
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
