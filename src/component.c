#include "component.h"
#include "peekpoke.h"

void complist_start()
{
    POKE(COMPLIST_PORT, 0xff);
}

void read_component(component_t *comp)
{
    for (int i = 0; i < 16; i++)
    {
        comp->name[i] = PEEK(COMPLIST_PORT);
        if (comp->name[i] == 0)
        {
            break;
        }
        else if (comp->name[i] == 0xff)
        {
            return;
        }
    }

    for (int i = 0; i < 16; i++)
    {
        comp->uuid[i] = PEEK(COMPLIST_PORT);
    }
}

void next_component()
{
    POKE(COMPLIST_PORT, 0);
}
