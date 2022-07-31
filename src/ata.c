#include <io.h>
#include <ata.h>
#include <stdint.h>

void ata_read_disk(int lba, char *buffer)
{
    outb(0x1F6, 0xe0 | ((lba >> 0x18) & 0x0f)); // select drive
    outb(0x1F1, 0);                             // use PIO
    outb(0x1F2, 1);                             // sector count
    outb(0x1F3, lba);                           // send lba
    outb(0x1F4, lba >> 8);                      // offset lba by 8 bits
    outb(0x1F5, lba >> 16);                     // offset lba by 16 bits
    outb(0x1F7, 0x20);                          // send read command

    while (!(inb(0x1F7) & 8))
        ;
    for (int i = 0; i <= 255; i++)
    {
        uint16_t val = inw(0x1F0);
        buffer[i * 2] = val & 0xff;
        buffer[i * 2 + 1] = val >> 8;
    }
}