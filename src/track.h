#ifndef TRACK_H
#define TRACK_H

#include "typedef.h"

#include "train.h"

struct Segment {
    SegmentID id;

    /* Input */

    u32 num_points;
    Vec2 *points;

    /* Generated */

    u32 num_draw_points;
    Vec2 *draw_points;

    // World-scale, (approximately) how long the curve is.
    f32 length;

    // Start and end position of the curve.
    // Always equal to first and last element of points[].
    Vec2 ends[2];

    ConnectionID connection_start;
    ConnectionID connection_end;
};

struct Connection {
    ConnectionID id;

    // Assumed to always be correct.
    SegmentID segment_a;
    u8 a_end;

    // Can have any length but ideally 0, 1 or 2.
    // 0  => terminated
    // 1  => continues
    // 2+ => switch
    u32 num_segment_b;
    SegmentID *segment_b;
    u8 *b_ends;  // Which end of each segment this connection is on.

    // Index of active segment. Only matters when number of segments > 1.
    u32 active_segment_b;
};

struct Track {
    u32 num_segments;
    Segment *segments;

    u32 num_connections;
    Connection *connections;

    u32 num_trains;
    Train *trains;
};

///*
// Create a new unconnected connection.
Connection *new_connection(Track *track);

///*
// Create a new empty segment.
Segment *new_segment(Track *track);

///*
// Create a segment after some other segment.
Segment *next_segment(Track *track, Segment *segment);

///*
// Insert a segment after or before some other segment.
Segment *insert_segment(Track *track, SegmentID segment_id, u8 segment_end);

///*
// Add a point to a segment.
//
// First and last point are the respective endpoints. Other points are used when
// generating the bezier-curve, which is always done when a new point is added.
void add_point(Segment *segment, Vec2 p);

///*
// Create a connection between two segments.
void connect_segment(Track *track, SegmentID a, u8 a_end, SegmentID b, u8 b_end);

///*
//TODO(gu) Reverse a connection's ends.
void reverse_connection(Connection *c);

///*
// Create a connection at a segment's end with no b-connections.
void terminate(Track *track, Segment *segment, u8 segment_end);

///*
// Fetch a segment with a SegmentID.
Segment *fetch_segment(Track *track, SegmentID id);

///*
// Fetch a connection with a ConnectionID.
Connection *fetch_connection(Track *track, ConnectionID id);

///*
// Draw all segments in a track.
void draw_track(Track *track);

///*
// Generate a bezier-curve and store it in the segment.
void get_bezier(Segment *segment);

Vec2 point_at_bezier_length(Segment *segment, f32 length);

#endif  // ifndef TRACK_H
