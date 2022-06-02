#include "peekpoke.h"

void putchar(const char c)
{
    POKE(0x2ff, c);
}

void print(const char* str) {
    while (*str) {
        POKE(0x2ff, *str++);
    }
}

void main() {
    putchar(0); // Initialize the screen
    while (1);
}
