#include "terminal.h"
#include "component.h"
#include <string.h>
#include "peekpoke.h"
#include <stdbool.h>

// typedef struct
// {
//     volatile char uuid[2];      // First two byte of the UUID of the drive we want to do stuff with
//     volatile char sectorsize;   // Size of a sector in units of 256 bytes
//     volatile char rw;           // R/W port
//     volatile short sectorcount; // Number of sectors on the drive
//     volatile short rwsector;    // Current sector we are reading or writing to
// } diskslot_t;

// diskslot_t *disk = (diskslot_t *)0x260;

// // Load the boot sector of a disk into memory starting from 0x1000
// void load_bs()
// {
//     if (disk->sectorsize < 2)
//     {
//         return; // Must be at least 512 bytes
//     }

//     disk->rwsector = 1; // Sector starts at 1 in OpenComputers
//     for (int i = 0; i < 512; i++)
//     {
//         POKE(0x1000 + i, disk->rw);
//     }
// }

// // Look for disks with a valid boot signature
// bool find_disk()
// {
//     component_t comp;
//     complist_start();
//     read_component(&comp);

//     while (comp.name[0] != 0xff)
//     {
//         // print(comp.name);
//         // print("\r");
//         if (memcmp(comp.name, "drive", 6) == 0)
//         {
//             disk->uuid[0] = comp.uuid[0];
//             disk->uuid[1] = comp.uuid[1];

//             load_bs();
//             if (*(short *)0x11fe == 0xaa55)
//             {
//                 return true;
//             }
//         }

//         next_component();
//         read_component(&comp);
//     }

//     return 0;
// }

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
            asm ("ldx #0");
            asm ("txs"); // Reset the stack pointer
            asm ("jmp $1000"); // Jump to the boot sector
        }
    }

    putchar(7);
    print("No bootable drive found.\rInsert a drive with a valid boot sector and restart the system.\r");
    return 0;
}
