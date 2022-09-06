#include "pci.h"
#include <cpu/io.h>
#include <console.h>

pci_device_t pci_devices[256];
int dev_index = 0;

static uint16_t pci_read_word(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset)
{
    uint32_t addr = ((uint32_t)bus << 16)
                  | ((uint32_t)device << 11) 
                  | ((uint32_t)func << 8) 
                  | (offset & 0xFC) 
                  | ((uint32_t)0x80000000);
    outl(CONFIG_ADDRESS, addr);

    uint16_t word = (uint16_t)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);

    return word;
}

void pci_enumerate()
{
    console_log("Enumerating PCI devices.");

    for (int bus = 0; bus < 256; bus++)
    {
        for (int device = 0; device < 32; device++)
        {
            uint16_t vendor_id = pci_read_word(bus, device, 0, 0);
            if (vendor_id == 0xFFFF)
                continue; // Device doesn't exist

            pci_device_t pci;
            pci.bus = bus;
            pci.device = device;

            pci.vendor_id = vendor_id;
            pci.device_id = pci_read_word(bus, device, 0, 0x2);
            pci.class_id = pci_read_word(bus, device, 0, 0xA) >> 8;
            pci.subclass_id = pci_read_word(bus, device, 0, 0xA) & 0x00FF;
            pci.prog_interface_id = pci_read_word(bus, device, 0, 0x8) >> 8;
            
            console_log("0x%x, 0x%x, 0x%x, 0x%x, 0x%x", pci.vendor_id, pci.device_id, pci.class_id, pci.subclass_id, pci.prog_interface_id);

            pci_devices[dev_index] = pci;
            dev_index++;
        }
    }

    console_log("Found %d PCI devices.", dev_index);
}