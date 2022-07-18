#include "terminal.h"

void irq() __attribute__((interrupt, no_isr))
{
    char a, x, y;
    asm volatile(
        "sta %0\n"
        "stx %1\n"
        "sty %2\n"
        : "=m"(a), "=m"(x), "=m"(y));

    print("TODO: Implement IRQ. A: $");
    printbyte(a);
    print(", X: $");
    printbyte(x);
    print(", Y: $");
    printbyte(y);
    print("\n");

    asm(
        "lda %0\n"
        "ldx %1\n"
        "ldy %2\n"
        "rti\n" ::"m"(a),
        "m"(x), "m"(y));
    return;
}

void nmi() __attribute__((interrupt, no_isr))
{
    char a, x, y;
    asm volatile(
        "sta %0\n"
        "stx %1\n"
        "sty %2\n"
        : "=m"(a), "=m"(x), "=m"(y));

    print("TODO: Implement NMI. A: $");
    printbyte(a);
    print(", X: $");
    printbyte(x);
    print(", Y: $");
    printbyte(y);
    print("\n");

    asm(
        "lda %0\n"
        "ldx %1\n"
        "ldy %2\n"
        "rti\n" ::"m"(a),
        "m"(x), "m"(y));
    return;
}
