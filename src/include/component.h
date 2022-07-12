#pragma once

#define COMPLIST_PORT 0x246

typedef struct
{
    char name[16]; // Name of the component
    char uuid[16]; // UUID of the component
} component_t;

void complist_start();                  // Start listing components
void read_component(component_t *comp); // Read a component from the list
void next_component();                  // Move to the next component
