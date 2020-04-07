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

void train_update(Track *trach, Train *train, f32 delta);

void train_draw(Track *track, Train *train);

#endif  // ifndef TRAIN_H
