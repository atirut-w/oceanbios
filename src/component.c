#include "ocmos/component.h"

int list_components(component_t *buf)
{
    int count;

    start_component_list();
    for (count = 0; count < 16; count++)
    {
        read_component(&buf[count]);

        if (buf[count].name[0] == 0xff)
        {
            return count;
        }

        next_component();
    }

    return count;
}
