#include <fog.h>
#include <stdlib.h>

#include "main.h"
#include "track.h"

Track track = {};

void update() {}

void draw() {
    draw_track(&track);
}

int main(int argc, char **argv) {
    for (u32 i = 0; i < NUM_BINDINGS; i++) {
        bindings[i] = fog_input_request_name(1);
    }

    fog_init(argc, argv);
    fog_renderer_set_window_size(800, 800);
    fog_renderer_turn_on_camera(0);
    fog_renderer_fetch_camera(0)->zoom = 0.8;

    {
        Segment *segment;

        segment = add_segment(&track);
        add_point(segment, fog_V2( 0.5, 0));
        add_point(segment, fog_V2(-0.5, 0));

        segment = add_segment(&track);
        add_point(segment, fog_V2(-0.5, 0));
        add_point(segment, fog_V2(-1  , 0));
        add_point(segment, fog_V2(-1  , 0.5));
        add_point(segment, fog_V2(-0.5, 0.5));

        segment = add_segment(&track);
        add_point(segment, fog_V2(-0.5, 0.5));
        add_point(segment, fog_V2( 0.5, 0.5));

        segment = add_segment(&track);
        add_point(segment, fog_V2(0.5, 0.5));
        add_point(segment, fog_V2(1  , 0.5));
        add_point(segment, fog_V2(1  , 0));
        add_point(segment, fog_V2(0.5, 0));
    }

    fog_run(update, draw);
    return 0;
}
