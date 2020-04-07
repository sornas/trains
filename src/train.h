#ifndef TRAIN_H
#define TRAIN_H

#include <fog.h>

#include "typedef.h"

#include "track.h"

struct Train {
    TrainID id;

    Vec2 drawing_position;

    SegmentID segment_id;
    f32 segment_position;
    s8 direction;
};

Train *new_train(Track *track);

Train *fetch_train(Track *t, TrainID id);

#endif  // ifndef TRAIN_H
