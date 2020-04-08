#include <stdio.h>
#include <stdlib.h>
#include <fog.h>

#include "typedef.h"
#include "main.h"

#include "track.h"
#include "train.h"

Track track = {};
Train *train;

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

    if (fog_input_down(NAME(FORWARD), P1)) {
        train->segment_position += 0.5 * delta;
    } else if (fog_input_down(NAME(BACKWARD), P1)) {
        train->segment_position -= 0.5 * delta;
    }

    if (fog_input_pressed(NAME(SWITCH_INFRONT), P1)) {
        switch_infront(train, &track);
    }
    if (fog_input_pressed(NAME(SWITCH_BEHIND), P1)) {
        switch_behind(train, &track);
    }

    train_update(&track, train, delta);
}

void draw() {
    draw_track(&track);
    train_draw(&track, train);
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
    fog_input_add(fog_key_to_input_code(SDLK_i), NAME(SWITCH_INFRONT), P1);
    fog_input_add(fog_key_to_input_code(SDLK_k), NAME(SWITCH_BEHIND), P1);

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

    train = new_train(&track);

    fog_run(update, draw);
    return 0;
}
