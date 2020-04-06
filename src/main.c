#include <stdio.h>
#include <stdlib.h>
#include <fog.h>

#include "main.h"
#include "track.h"

Track track = {};

void update() {
    Vec2 move = fog_input_mouse_move();
    Camera *camera = fog_renderer_fetch_camera(0);
    f32 zoom = camera->zoom;
    if (fog_input_mouse_down(0)) {
        camera->position = fog_add_v2(
                camera->position,
                fog_V2(-move.x/(400.f * zoom), move.y/(400.0f * zoom)));
    }
}

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
    fog_renderer_fetch_camera(0)->zoom = 0.5;

    {
        Track *t = &track;
        Segment *s;
        SegmentID s_id;

        s = new_segment(t);
        s_id = s->id;
        terminate(t, s, 0);
        add_point(s, fog_V2(-0.8, 0));
        add_point(s, fog_V2(-0.2, 0));

        s = next_segment(t, s);
        add_point(s, fog_V2(-0.2, 0));
        add_point(s, fog_V2(0, 0));
        add_point(s, fog_V2(0.2, 0.1));
        add_point(s, fog_V2(0.4, 0.1));

        s = next_segment(t, s);
        add_point(s, fog_V2(0.4, 0.1));
        add_point(s, fog_V2(0.8, 0.1));
        add_point(s, fog_V2(0.8, 0.4));
        terminate(t, s, 1);

        s = insert_segment(t, s_id, 1);
        add_point(s, fog_V2(-0.2, 0));
        add_point(s, fog_V2(0.4, 0));

        s = next_segment(t, s);
        add_point(s, fog_V2(0.4, 0));
        add_point(s, fog_V2(0.8, 0));
        terminate(t, s, 1);
    }

    fog_run(update, draw);
    return 0;
}
