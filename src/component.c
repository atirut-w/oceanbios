#include "component.h"
#include "peekpoke.h"
#include "terminal.h"

int list_components(component_t *buf)
{
    int count;

    POKE(COMPLIST_PORT, 0xff);
    for (count = 0; count < MAX_COMPONENTS; count++)
    {
        for (int i = 0; i < 16; i++)
        {
            buf[count].name[i] = PEEK(COMPLIST_PORT);
            if (buf[count].name[i] == 0)
            {
                break;
            }
            else if (buf[count].name[i] == 0xff)
            {
                return count;
            }
        }
        print(buf[count].name);
        print("\r");

        for (int i = 0; i < 16; i++)
        {
            buf[count].uuid[i] = PEEK(COMPLIST_PORT);
        }

        POKE(COMPLIST_PORT, 0);
    }

    return count;
}
