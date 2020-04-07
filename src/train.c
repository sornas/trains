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
