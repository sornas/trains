#include <stdio.h>
#include <stdlib.h>
#include <fog.h>

#include "typedef.h"
#include "main.h"

#include "track.h"
#include "train.h"

Track track = {};
TrainID train_id;

void update() {
    f32 delta = fog_logic_delta();

    //Vec2 move = fog_input_mouse_move();
    Vec2 move = fog_V2(0, 0);
    Camera *camera = fog_renderer_fetch_camera(0);
    f32 zoom = camera->zoom;
    if (fog_input_mouse_down(0)) {
        camera->position = fog_add_v2(
                camera->position,
                fog_V2(-move.x/((WIN_WIDTH/2.0f) * zoom), move.y/((WIN_HEIGHT/2.0f) * zoom)));
    }

    Train *train = fetch_train(&track, train_id);

    fog_util_tweak_u32("Train segment", &train->segment_id);
    fog_util_tweak_f32("Train position", &train->segment_position, 1);

    if (fog_input_down(NAME(FORWARD), P1)) {
        train->segment_position += 2.0 * delta;
    } else if (fog_input_down(NAME(BACKWARD), P1)) {
        train->segment_position -= 2.0 * delta;
    }

    if (train->segment_id < track.num_segments) {
        //TODO(gu) fog_renderer_push_point(4, bezier_point_at_len(fetch_segment(&track, train->segment_id), train->segment_position), fog_V4(0, 1, 0,1), TRACK_WIDTH);
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
    fog_renderer_set_window_size(WIN_WIDTH, WIN_HEIGHT);
    fog_renderer_turn_on_camera(0);
    fog_renderer_fetch_camera(0)->zoom = 1;

    fog_input_add(fog_key_to_input_code(SDLK_w), NAME(FORWARD), P1);
    fog_input_add(fog_key_to_input_code(SDLK_s), NAME(BACKWARD), P1);

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

    train_id = new_train(&track)->id;

    fog_run(update, draw);
    return 0;
}
