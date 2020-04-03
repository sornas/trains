#pragma once

typedef enum {
    NUM_BINDINGS
} Binding;
Name bindings[NUM_BINDINGS];

#define NAME(binding) bindings[binding]
