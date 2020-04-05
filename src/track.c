#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <fog.h>

#include "track.h"

Connection *new_connection(Track *t) {
    t->connections = realloc(t->connections, (t->num_connections + 1) * sizeof(Connection));
    t->connections[t->num_connections] = (Connection) { t->num_connections++ };
    return &t->connections[t->num_connections - 1];
}

Segment *new_segment(Track *t) {
    t->segments = realloc(t->segments, (t->num_segments + 1) * sizeof(Segment));
    t->segments[t->num_segments] = (Segment) { t->num_segments++ };
    return &t->segments[t->num_segments - 1];
}

Segment *next_segment(Track *t, Segment *s) {
    SegmentID s_id = s->id;
    Segment *new_s = new_segment(t);
    s = fetch_segment(t, s_id);
    Connection *c = new_connection(t);
    c->segment_a = s->id;
    c->a_end = 1;
    c->num_segment_b = 1;
    c->segment_b = malloc(1 * sizeof(Segment *));
    c->b_ends = malloc(1 * sizeof(u8));
    c->segment_b[0] = new_s->id;
    c->b_ends[0] = 0;
    s->connection_end = c->id;
    new_s->connection_start = c->id;
    return new_s;
}

void add_point(Segment *s, Vec2 p) {
    s->num_points++;
    s->points = realloc(s->points, s->num_points * sizeof(Vec2));
    s->points[s->num_points-1] = p;
}

void connect_segment(Track *t, SegmentID a, u8 a_end, SegmentID b, u8 b_end) {
    Connection *c = new_connection(t);
    c->segment_a = a;
    c->a_end = a_end;
    c->num_segment_b = 1;
    c->segment_b = malloc(1 * sizeof(Segment *));
    c->b_ends = malloc(1 * sizeof(u8));
    c->segment_b[0] = b;
    c->b_ends[0] = b_end;
}

void terminate(Track *t, Segment *s, u8 s_end) {
    Connection *c = new_connection(t);
    c->segment_a = s->id;
    c->a_end = s_end;
    c->num_segment_b = 0;
    c->segment_b = NULL;
    c->b_ends = NULL;
}

void draw_track(Track *t) {
    for (u32 i = 0; i < t->num_segments; i++) {
        Segment *s = &t->segments[i];
        if (s->num_points == 0) continue;
        for (u32 j = 0; j < s->num_points-1; j++) {
            fog_renderer_push_line(0,
                    s->points[j+0],
                    s->points[j+1],
                    fog_V4(1, 1, 1, 1),
                    0.04);
        }
    }

    for (u32 i = 0; i < t->num_connections; i++) {
        Connection *c = &t->connections[i];
        //fog_renderer_push_point(0, fetch_segment(t, c->segment_a)->points[0], fog_V4(1, 0, 0, 0.75), 0.04);
        for (u32 j = 0; j < c->num_segment_b; j++) {
            //fog_renderer_push_point(0, fetch_segment(t, c->segment_b[j])->points[0], fog_V4(0, 1, 0, 0.75), 0.04);
        }
    }
}

Segment *fetch_segment(Track *t, SegmentID id) {
    return &t->segments[id];
}

Connection *fetch_connection(Track *t, ConnectionID id) {
    return &t->connections[id];
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
        for (u32 j = 0; i < num_points; ++i) {
            x += fog_binomial(num_points-1, j) * pow((1 - t), (num_points-j-1)) * pow(t, j) * points[j].x;
            y += fog_binomial(num_points-1, j) * pow((1 - t), (num_points-j-1)) * pow(t, j) * points[j].y;
        }
        // fog_V2(x, y);
#define SQ(NEW, OLD) ((NEW - OLD) * (NEW - OLD))
        len += sqrt(SQ(x, old_x) + SQ(y, old_y));
#undef SQ
    }
}
