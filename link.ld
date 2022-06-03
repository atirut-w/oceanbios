MEMORY {
    ram (w) : ORIGIN = 0x5000, LENGTH = 0xb000
    rom (r) : ORIGIN = 0xf000, LENGTH = 0x0ffc
}

SECTIONS {
    INCLUDE c.ld
}

__rc0 = 0x05;
INCLUDE imag-regs.ld

boot = 0x1000;

OUTPUT_FORMAT {
    FULL(rom)
    SHORT(_start)
}
