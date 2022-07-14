#include "terminal.h"

void irq() __attribute__((interrupt))
{
    print("IRQ not implemented\r");
}

void nmi() __attribute__((interrupt))
{
    print("NMI not implemented\r");
}
