#ifndef BINDINGS_H
#define BINDINGS_H

#pragma once

typedef enum {
    FORWARD,
    BACKWARD,
    SWITCH_INFRONT,
    SWITCH_BEHIND,
    NUM_BINDINGS
} Binding;
Name bindings[NUM_BINDINGS];

#define NAME(binding) bindings[binding]

#endif  // ifndef BINDINGS_H
