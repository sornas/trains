#define TRACK_ENDPOINT_COLOR fog_V4(1, 1, 1, 0.8)
#define TRACK_CONTROLPOINT_COLOR fog_V4(1, 1, 1, 0.5)
#define TRACK_POINT_SIZE 0.04

typedef struct Point Point;
typedef struct Segment Segment;
typedef struct Connection Connection;
typedef struct Track Track;

struct Point {
    Vec2 point;
    Segment *parent;
};

struct Segment {
    u32 num_points;
    Point *points;
};

struct Connection {
    Vec2 point;
    u32 num_other_points;
    Vec2 *other_points;
};

struct Track {
    u32 num_segments;
    Segment *segments;
};

void draw_track(Track *track);

Point *add_point(Segment *to_segment, Vec2 point);

Segment *add_segment(Track *to_track);
