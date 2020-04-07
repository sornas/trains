#pragma once

typedef enum {
    FORWARD,
    BACKWARD,
    NUM_BINDINGS
} Binding;
Name bindings[NUM_BINDINGS];

#define NAME(binding) bindings[binding]
