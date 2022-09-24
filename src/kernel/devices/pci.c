#include "pci.h"
#include <cpu/io.h>
#include <console.h>
#include <devices/drivers/ahci/ahci.h>

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

static pci_device_t* check_device(uint8_t bus, uint8_t device, uint8_t func)
{
    pci_device_t pci;
    pci.bus = bus;
    pci.device = device;
    pci.func = func;

    pci.vendor_id = pci_read_word(bus, device, func, 0x0);
    pci.device_id = pci_read_word(bus, device, func, 0x2);

    pci.revision_id = pci_read_word(bus, device, func, 0x8) & 0xFF;
    pci.prog_interface_id = pci_read_word(bus, device, func, 0x8) >> 8;
    pci.subclass_id = pci_read_word(bus, device, func, 0xA) & 0xFF;
    pci.class_id = pci_read_word(bus, device, func, 0xA) >> 8;           

    pci.cache_line_size = pci_read_word(bus, device, func, 0xC) & 0xFF;
    pci.latency_timer = pci_read_word(bus, device, func, 0xC) >> 8;
    pci.header_type = pci_read_word(bus, device, func, 0xE) & 0xFF;
    pci.bist = pci_read_word(bus, device, func, 0xE) >> 8;        
    
    switch (pci.header_type)
    {
    case 0x0: ;
        ext_header_1_t *ext_hdr = (ext_header_1_t*)&pci.ext_header;

        ext_hdr->bar5 = pci_read_word(bus, device, func, 0x26) << 16 | pci_read_word(bus, device, func, 0x24);
        break;
    default:
        break;
    }
    
    console_log("0x%x, 0x%x, 0x%x, 0x%x, 0x%x - %d %d %d", pci.vendor_id, pci.device_id, pci.class_id, pci.subclass_id, pci.prog_interface_id, bus, device, func);

    pci_devices[dev_index] = pci;

    switch (pci.class_id)
    {
    case 0x1: // Mass Storage Controller
        if (pci.subclass_id == 0x6 /*Serial ATA Controller*/ && pci.prog_interface_id == 0x1 /*AHCI 1.0*/)
            ahci_init(&pci_devices[dev_index]);
        break;
    default:
        break;
    }

    return &pci_devices[dev_index++];
}

void pci_enumerate()
{
    console_log("Enumerating PCI devices.");

    for (int bus = 0; bus < 256; bus++)
        for (int device = 0; device < 32; device++)
        {
            if (pci_read_word(bus, device, 0, 0) == 0xFFFF)
                continue;

            if((check_device(bus, device, 0)->header_type & 0x80) != 0) // multi-function device
            {
                for (int func = 1; func < 8; func++)
                {
                    if (pci_read_word(bus, device, func, 0) != 0xFFFF)
                            check_device(bus, device, func);
                }
            }  
        }

    console_log("Found %d PCI devices.", dev_index);
}