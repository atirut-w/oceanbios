#pragma once

#define COMPLIST_PORT 0x246
#define MAX_COMPONENTS 16

typedef struct component
{
    char name[16]; // Name of the component
    char uuid[16]; // UUID of the component
} component_t;

int list_components(component_t *buf);
