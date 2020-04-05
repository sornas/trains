#include <stdlib.h>
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
    c->segment_b = malloc(1 * sizeof(SegmentID));
    c->b_ends = malloc(1 * sizeof(u8));
    c->segment_b[0] = new_s->id;
    c->b_ends[0] = 0;
    s->connection_end = c->id;
    new_s->connection_start = c->id;
    return new_s;
}

Segment *insert_segment(Track *t, SegmentID s_id, u8 s_end) {
    // we assume a connection exist
    Segment *new_s = new_segment(t);
    Segment *s = fetch_segment(t, s_id);
    Connection *c = fetch_connection(t, (s_end == 0 ? s->connection_start : s->connection_end));
    if (c->segment_a != s->id) return NULL;  //TODO(gu) reverse_connection
    c->num_segment_b++;
    c->segment_b = realloc(c->segment_b, c->num_segment_b * sizeof(SegmentID));
    c->b_ends = realloc(c->b_ends, c->num_segment_b * sizeof(u8));
    c->segment_b[c->num_segment_b - 1] = new_s->id;
    c->b_ends[c->num_segment_b - 1] = 0;
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
    static b8 hi_segment = 0;
    static b8 hi_connection = 0;
    static u32 segment_id = 0;
    static u32 connection_id = 0;

    fog_util_tweak_b8("Highlight segment", &hi_segment);
    fog_util_tweak_b8("Highlight connection", &hi_connection);
    fog_util_tweak_u32("Segment ID", &segment_id);
    fog_util_tweak_u32("Connection ID", &connection_id);

    for (u32 i = 0; i < t->num_segments; i++) {
        Segment *s = &t->segments[i];
        if (!s->draw_points) continue;
        for (u32 j = 0; j < s->num_draw_points-1; j++) {
            fog_renderer_push_line(
                    (hi_segment && segment_id == s->id ? 1 : 0),
                    s->draw_points[j+0],
                    s->draw_points[j+1],
                    (hi_segment && segment_id == s->id ? TRACK_HIGHL_COLOR : TRACK_BASIC_COLOR),
                    TRACK_WIDTH);
        }
    }


    /*
    if (hi_segment && segment_id < t->num_segments) {
        Segment *s = fetch_segment(t, segment_id);
        if (!s->num_points == 0) {
            for (u32 j = 0; j < s->num_points-1; j++) {
                fog_renderer_push_line(0,
                        s->points[j+0],
                        s->points[j+1],
                        fog_V4(1, 0, 0, 1),
                        0.04);
            }
        }
    }
    */
    
    if (hi_connection && connection_id < t->num_connections) {
        Connection *c = fetch_connection(t, connection_id);

        Segment *s = fetch_segment(t, c->segment_a);
        fog_renderer_push_point(0, s->points[(c->a_end == 0 ? 0 : s->num_points-1)], fog_V4(0, 1, 0, 1), 0.04);

        for (u32 i = 0; i < c->num_segment_b; i++) {
            s = fetch_segment(t, c->segment_b[i]);
            fog_renderer_push_point(0, s->points[(c->b_ends[i] == 0 ? 0 : s->num_points-1)], fog_V4(0, 1, 0, 1), 0.04);
        }
        if (c->segment_b) {
            s = fetch_segment(t, c->segment_b[c->active_segment_b]);
            fog_renderer_push_point(0, s->points[(c->b_ends[c->active_segment_b] == 0 ? 0 : s->num_points-1)], fog_V4(0, 0, 1, 1), 0.04);
        }
    }

}

Segment *fetch_segment(Track *t, SegmentID id) {
    return &t->segments[id];
}

Connection *fetch_connection(Track *t, ConnectionID id) {
    return &t->connections[id];
}

void get_bezier(Segment *s) {
    const u32 STEPS = 100;
    const real STEP = 1 / (real) STEPS;
    s->length = 0.0f;
    s->num_draw_points = STEPS+1;
    s->draw_points = malloc((STEPS+1) * sizeof(Vec2));
    real x, y;
    real t = 0.0f;
    real old_x = s->points[0].x;
    real old_y = s->points[0].y;
    for (u32 i = 0; i <= STEPS; i++) {
        x = 0.0f;
        y = 0.0f;
        for (u32 j = 0; j < s->num_points; ++j) {
            x += fog_binomial(s->num_points-1, j) * pow((1 - t), (s->num_points-j-1)) * pow(t, j) * s->points[j].x;
            y += fog_binomial(s->num_points-1, j) * pow((1 - t), (s->num_points-j-1)) * pow(t, j) * s->points[j].y;
        }
        t += STEP;
        s->draw_points[i] = fog_V2(x, y);
#define SQ(A) ((A) * (A))
        s->length += sqrt(SQ(x - old_x) + SQ(y - old_y));
#undef SQ
    }
}
