typedef struct Segment Segment;
typedef struct Connection Connection;
typedef struct Track Track;

typedef u32 SegmentID;
typedef u32 ConnectionID;

struct Segment {
    SegmentID id;
    /* input */

    u32 num_points;
    Vec2 *points;

    /* generated */

    u32 num_draw_points;
    Vec2 *draw_points;

    Vec2 start;
    Vec2 end;

    ConnectionID connection_start;
    ConnectionID connection_end;
};

struct Connection {
    ConnectionID id;

    SegmentID segment_a;
    u8 a_end;

    u32 num_segment_b;
    SegmentID *segment_b;
    u8 *b_ends;

    u32 active_segment_b;
};

struct Track {
    u32 num_segments;
    Segment *segments;
    u32 num_connections;
    Connection *connections;
};

Segment *new_segment(Track *t);

Segment *next_segment(Track *t, Segment *s);

Segment *insert_segment(Track *t, SegmentID s, u8 s_end);

void add_point(Segment *s, Vec2 p);

void connect_segment(Track *t, SegmentID a, u8 a_end, SegmentID b, u8 b_end);

void reverse_connection(Connection *c);

void terminate(Track *t, Segment *s, u8 s_end);

Segment *fetch_segment(Track *t, SegmentID id);

Connection *fetch_connection(Track *t, ConnectionID id);

void draw_track(Track *t);

void get_bezier(u32 num_points, Vec2 *points);
