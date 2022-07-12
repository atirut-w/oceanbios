# OceanBIOS
Custom BIOS for the OCMOS addon for OpenComputers.

## Booting
The BIOS checks for boot sector signature(0x55, 0xAA) instead of using OETF CAB boot specifications to allow mixing filesystems, partitions and bootloaders more easily. This was one of the main reasons I made this BIOS.

When there are no bootable disks, the BIOS hangs with a message telling you to restart instead of dropping to monitor. Implementing a monitor for debugging is not a priority.

## Disk images
Disk image created by some utilities, such as `mkfs.fat`, always have a boot sector signature. If you don't want the BIOS to boot a disk image, you can simply remove the said signature before compressing them for OpenComputers.
