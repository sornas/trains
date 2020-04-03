#include <stdlib.h>
#include <fog.h>

#include "track.h"

void add_point(Segment *to_segment, Vec2 point) {
    to_segment->num_points++;
    to_segment->points = realloc(to_segment->points, to_segment->num_points * sizeof(Vec2));
    to_segment->points[to_segment->num_points - 1] = point;
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

        fog_renderer_push_line(0, segment.points[0], segment.points[segment.num_points-1], fog_V4(0, 0, 0, 1), TRACK_POINT_SIZE/2);
        fog_renderer_push_point(0, segment.points[0], TRACK_ENDPOINT_COLOR, TRACK_POINT_SIZE);
        fog_renderer_push_point(0, segment.points[segment.num_points-1], TRACK_ENDPOINT_COLOR, TRACK_POINT_SIZE);

        for (u32 j = 1; j < segment.num_points-1; j++) {
            fog_renderer_push_point(0, segment.points[j], TRACK_CONTROLPOINT_COLOR, TRACK_POINT_SIZE);
        }
    }
}
