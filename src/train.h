#ifndef TRAIN_H
#define TRAIN_H

#include <fog.h>

#include "typedef.h"

#include "track.h"

struct Train {
    TrainID id;

    SegmentID segment_id;
    f32 segment_position;
    s8 direction;
};

Train *new_train(Track *track);

Train *fetch_train(Track *t, TrainID id);

void train_update(Track *t, Train *train, f32 delta);

#endif  // ifndef TRAIN_H
