#define TRACK_ENDPOINT_COLOR fog_V4(1, 1, 1, 0.8)
#define TRACK_CONTROLPOINT_COLOR fog_V4(1, 1, 1, 0.5)
#define TRACK_POINT_SIZE 0.04
typedef struct {
    u32 num_points;
    Vec2 *points;
} Segment;

typedef struct {
    u32 num_segments;
    Segment *segments;
} Track;

void draw_track(Track *track);

void add_point(Segment *to_segment, Vec2 point);

Segment *add_segment(Track *to_track);
