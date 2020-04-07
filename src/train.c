#include <stdlib.h>

#include "train.h"

Train *new_train(Track *t) {
    t->trains = realloc(t->trains, (t->num_trains + 1) * sizeof(Train));
    t->trains[t->num_trains] = (Train) { t->num_trains++ };
    return &t->trains[t->num_trains];
}

Train *fetch_train(Track *t, TrainID id) {
    return &t->trains[id];
}

void train_update(Track *track, Train *train, f32 delta) {
    fog_util_tweak_u32("Train segment", &train->segment_id);
    fog_util_tweak_f32("Train position", &train->segment_position, 1);

    if (train->segment_id < track->num_segments) {
        //TODO(gu) fog_renderer_push_point(4, bezier_point_at_len(fetch_segment(&track, train->segment_id), train->segment_position), fog_V4(0, 1, 0,1), TRACK_WIDTH);
    }
}
