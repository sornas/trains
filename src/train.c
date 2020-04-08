#include <stdlib.h>

#include "train.h"

Train *new_train(Track *t) {
    t->trains = realloc(t->trains, (t->num_trains + 1) * sizeof(Train));
    t->trains[t->num_trains] = (Train) { t->num_trains++, 0, 0.0f, 1};
    return &t->trains[t->num_trains-1];
}

Train *fetch_train(Track *t, TrainID id) {
    return &t->trains[id];
}

void train_pass_connection(Track *track, Train *train, Connection *c) {
    Segment *cur_s = fetch_segment(track, train->segment_id);
    SegmentID cur_s_id = cur_s->id;

    if (cur_s_id == c->segment_a) {
        if (c->segment_b) {
            train->segment_id = c->segment_b[c->active_segment_b];
            train->segment_position = (c->b_ends[c->active_segment_b] == 1 ?
                    fetch_segment(track, c->active_segment_b)->length : 0);
            train->direction =
                (c->b_ends[c->active_segment_b] == 1 ? -1 : 1);
        } else {  //TODO(gu) at terminated end
        }
    }
    for (u32 i = 0; i < c->num_segment_b; i++) {
        //TODO(gu) where is terminated end?
        if (cur_s_id == c->segment_b[i]) {
            train->segment_id = c->segment_a;
            train->segment_position =
                (c->a_end == 1 ? fetch_segment(track, i)->length : 0);
            train->direction =
                (c->a_end == 1 ? -1 : 1);
        }
    }
}

void train_update(Track *track, Train *train, f32 delta) {
    fog_util_tweak_s32("Train segment", &train->segment_id);
    fog_util_tweak_f32("Train position", &train->segment_position, 0.2);

    Segment *s = fetch_segment(track, train->segment_id);
    if (train->segment_position < 0) {
        // move through start connection
        train_pass_connection(track, train, fetch_connection(track, s->connection_start));
    }
    if (train->segment_position > s->length) { 
        // move through end connection
        train_pass_connection(track, train, fetch_connection(track, s->connection_end));
    }
}

void train_draw(Track *track, Train *train) {
    Vec2 train_position = point_at_bezier_length(fetch_segment(track, train->segment_id), train->segment_position);
    if (train->segment_id >= 0 && train->segment_id < track->num_segments) {
        fog_renderer_push_point(15, train_position, fog_V4(0, 1, 0,1), TRACK_WIDTH);
    }
}
