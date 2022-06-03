#include "terminal.h"
#include "component.h"
#include <string.h>
#include "peekpoke.h"

void tryboot() {
    char sectorsize = PEEK(0x262); // In units of 256 bytes
    if (sectorsize == 0) {
        return; // No drive mapped
    }

    // Set current R/W sector to 1
    POKE(0x266, 1);
    POKE(0x267, 0);
    char* ldptr = (char*)0x1000; // Address to load fist sector to

    for (int i = 0; i < sectorsize; i++) {
        for (int j = 0; j < 256; j++) {
            *ldptr++ = PEEK(0x263);
        }
    }

    // Verify for boot signature
    if (*(short*)(ldptr - 2) == 0xaa55) {
        putchar(7); // Beep

        // (*(void(*)())0x1000)();
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
            POKE(0x260, comp.uuid[0]);
            POKE(0x261, comp.uuid[1]);
            tryboot();
        }

        next_component();
        read_component(&comp);
    }
}

int main() {
    putchar(0); // Initialize the terminal.
    print("Ocean BIOS Copyright 2022 Atirut Wattanamongkol\r\r");

    while (1) {
        finddrive();
    }

    print("\rOk!");

    return 0;
}
