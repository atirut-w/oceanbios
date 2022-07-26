# OceanBIOS
Custom BIOS for the OCMOS addon for OpenComputers.

## Boot sequence
- For each unmanaged drives
    - Load the first 512 bytes into 0x1000
    - Check for boot signature(0xAA55 LE)
        - If signature is valid, reset stack pointer and jump to 0x1000
- Display error message and halt

## Disk images
Disk image created by some utilities, such as `mkfs.fat`, always have a boot sector signature. If you don't want the BIOS to boot a disk image, you can simply remove the said signature before compressing them for OpenComputers.

## Building
---

Before building this project, make sure you have the [LLVM-MOS SDK](https://github.com/llvm-mos/llvm-mos-sdk/releases) installed for compiling 6502 binaries.

---

Run the following commands from the cloned repository:

```bash
mkdir build && cd build
cmake ..
make
```

You should now have two built files, `bios.bin` and `bios.bin.elf`. The former is a raw binary file for flashing onto an EEPROM, and the latter contains the BIOS in ELF format.
