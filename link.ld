MEMORY {
    zp : ORIGIN = 0x00, LENGTH = 0xFF
    ram (w) : ORIGIN = 0x1200, LENGTH = 0x400
    rom (r) : ORIGIN = 0xf000, LENGTH = 0x0ffc
}

SECTIONS {
    INCLUDE c.ld
}

__rc0 = 0x05;
INCLUDE imag-regs.ld
__stack = 0x2000;

OUTPUT_FORMAT {
    FULL(rom)
    SHORT(_start)
}
