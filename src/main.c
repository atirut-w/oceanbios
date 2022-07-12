#include "terminal.h"
#include "component.h"
#include <string.h>
#include "peekpoke.h"
#include <stdbool.h>

component_t components[MAX_COMPONENTS];
int component_count;
#define REFRESH_COMPONENTS() component_count = list_components(components)

typedef struct disk
{
    char uuid[2];
    char sectorsize;
    char rw;
    short sectorcount;
    short rwsector;
} disk_t;

disk_t *disk = (disk_t *)0x260;

void load_bs()
{
    if (disk->sectorsize < 2)
    {
        return; // Must be at least 512 bytes
    }

    disk->rwsector = 1; // Sector starts at 1 in OpenComputers
    for (int i = 0; i < 512; i++)
    {
        POKE(0x1000 + i, disk->rw);
    }
}

bool find_disk()
{
    for (int i = 0; i < component_count; i++)
    {
        if (memcmp(components[i].name, "drive", 6) == 0)
        {
            disk->uuid[0] = components[i].uuid[0];
            disk->uuid[1] = components[i].uuid[1];

            load_bs();
            if (*(short *)0x11fe == 0xaa55)
            {
                return true;
            }
        }
    }
    return false;
}

int main()
{
    putchar(0); // Initialize the terminal
    putchar(6); // Cycle to next supported terminal size
    print("Ocean BIOS Copyright 2022 Atirut Wattanamongkol\r\r");

    REFRESH_COMPONENTS();
    for (int i = 0; i < 2; i++) // Retry at least twice
    {
        if (find_disk())
        {
            putchar(7); // Beep
            asm("ldx #ff");
            asm("txs");       // Reset the stack pointer
            asm("jmp $1000"); // Jump to the boot sector
        }
    }

    print("No bootable drive found.\rInsert a drive with a valid boot sector and restart the system.\r");
    return 0;
}
