#include "terminal.h"
#include "component.h"
#include <string.h>
#include "peekpoke.h"

typedef struct {
    volatile char uuid[2]; // First two byte of the UUID of the drive we want to do stuff with
    volatile char sectorsize; // Size of a sector in units of 256 bytes
    volatile char rw; // R/W port
    volatile short sectorcount; // Number of sectors on the drive
    volatile short rwsector; // Current sector we are reading or writing to
} diskslot_t;

void tryboot() {
    diskslot_t* disk = (diskslot_t*)0x260;
    if (disk->sectorsize == 0 || disk->sectorsize < 2) {
        return;
    }

    // Set current R/W sector to 1
    disk->rwsector = 1;
    char* ldptr = (char*)0x1000; // Address to load fist sector to

    for (int i = 0; i < 512; i++) {
        ldptr[i] = disk->rw;
    }

    // Verify for boot signature at 0x100 + 0x1FE
    if (*(short*)0x11fe == 0xaa55) {
        putchar(7); // Beep
        asm volatile ("JMP $1000");
    }
}

void finddrive() {
    component_t comp;

    complist_start();
    read_component(&comp);

    while (comp.name[0] != 0xff) {
        if (memcmp(comp.name, "drive", 6) == 0) {
            // print("Checking drive ");
            // for (int i = 0; i < 2; i++) {
            //     printbyte(comp.uuid[i]);
            // }
            // print("\r");

            // Map the drive to the first drive controller
            diskslot_t* disk = (diskslot_t*)0x260;
            disk->uuid[0] = comp.uuid[0];
            disk->uuid[1] = comp.uuid[1];
            tryboot();
        }

        next_component();
        read_component(&comp);
    }
}

int main() {
    putchar(0); // Initialize the terminal.
    putchar(6); // Cycle to next supported terminal size.
    print("Ocean BIOS Copyright 2022 Atirut Wattanamongkol\r\r");

    finddrive();

    print("No bootable drive found.\rInsert a drive with a valid boot sector and restart the system.\r");
    return 0;
}
