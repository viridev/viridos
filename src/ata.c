#include <io.h>
#include <ata.h>
#include <stdint.h>

void ata_read_disk(int LBA, char *buffer)
{
    outb(0x1F6, 0xE0 | ((LBA >> 24) & 0x0F)); // drive select
    outb(0x1F1, 0x00);                        // wait
    outb(0x1F2, 1);                           // Sector count
    outb(0x1F3, LBA);                         // send LBA
    outb(0x1F4, LBA >> 8);                    // ...
    outb(0x1F5, LBA >> 16);                   // ...
    outb(0x1F7, 0x20);                        // Send Command READ

    while (!(inb(0x1F7) & 8))
        ;
    for (int i = 0; i <= 255; i++)
    {
        uint16_t val = inw(0x1F0);
        buffer[i * 2] = val & 0xff;
        buffer[i * 2 + 1] = val >> 8;
    }
}