#include "terminal.h"
#include "peekpoke.h"

void putchar(const char c)
{
    POKE(SERIALTERM, c);
}

void print(const char *s)
{
    while (*s)
    {
        POKE(SERIALTERM, *s++);
    }
}

void printbyte(const char b)
{
    char lookup[] = "0123456789abcdef";
    POKE(SERIALTERM, lookup[(b >> 4) & 0xf]);
    POKE(SERIALTERM, lookup[b & 0xf]);
}

void printshort(const short s)
{
    printbyte((s >> 8) & 0xff);
    printbyte(s & 0xff);
}
