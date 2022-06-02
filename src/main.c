#include "peekpoke.h"

void print(const char* str) {
    while (*str) {
        POKE(0x2ff, *str++);
    }
}

void main() {
    POKE(0x2ff, 0); // Initialize the screen
    print("Ocean BIOS Copyright 2022 Atirut Wattanamongkol\r\r");
    while (1);
}
