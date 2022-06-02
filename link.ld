MEMORY {
    ram (w) : ORIGIN = 0x1000, LENGTH = 0xe000
    rom (r) : ORIGIN = 0xf000, LENGTH = 0x0ffc
}

SECTIONS {
    INCLUDE c.ld
}

__rc0 = 0x05;
INCLUDE imag-regs.ld

OUTPUT_FORMAT {
    FULL(rom)
    SHORT(_start)
}
