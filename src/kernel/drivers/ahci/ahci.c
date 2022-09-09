#include "ahci.h"
#include <console.h>
#include <memory/mem.h>

pci_device_t *pci_device;
hba_memory_t *abar;
port_t ports[32];
uint8_t port_count;

static uint8_t check_port_type(hba_port_t *port)
{
    uint32_t sata_status = port->sata_status;

    uint8_t interface_power_management = (sata_status >> 8) & 0x0F;
    uint8_t device_detection = sata_status & 0x0F;

    if (device_detection != HBA_PORT_DEV_PRESENT) return NONE;
    if (interface_power_management != HBA_PORT_IPM_ACTIVE) return NONE;

    switch (port->signature)
    {
    case SATA_SIGNATURE_ATA:
        return SATA;
        break;
    case SATA_SIGNATURE_ATAPI:
        return SATAPI;
        break;
    case SATA_SIGNATURE_PM:
        return PM;
    case SATA_SIGNATURE_SEMB:
        return SEMB;
    default:
        return NONE;
        break;
    }
}

static void probe_ports()
{
    uint32_t ports_implemented = abar->ports_implemented;
    
    for (int i = 0; i < 32; i++)
        if (ports_implemented & (1 << i)) // port is active
        {
            int port_type = check_port_type(&abar->ports[i]);

            if (port_type == SATA || port_type == SATAPI)
            {
                ports[port_count].port_number = port_count;
                ports[port_count].port_type = port_type;
                ports[port_count].hba_port = &abar->ports[i];

                port_count++;
            }
        }
}

// 3.3.7 in docs
#define HBA_PxCMD_CR 0x8000 // bit 15
#define HBA_PxCMD_FRE 0x0010 // 4
#define HBA_PxCMD_ST 0x0001 // bit 1
#define HBA_PxCMD_FR 0x4000 // bit 14

static void stop_cmd(port_t *port)
{
    port->hba_port->cmd_and_status &= ~HBA_PxCMD_ST; // set the start bit off
    port->hba_port->cmd_and_status &= ~HBA_PxCMD_FRE;

    printf("");

    while (1)
    {
        if (port->hba_port->cmd_and_status & HBA_PxCMD_FR) continue;
        if (port->hba_port->cmd_and_status & HBA_PxCMD_CR) continue;

        break;
    }
    
}

static void start_cmd(port_t *port)
{
    while (port->hba_port->cmd_and_status & HBA_PxCMD_CR); // wait until the command list DMA engine for the port stops running

    port->hba_port->cmd_and_status |= HBA_PxCMD_FRE; 
    port->hba_port->cmd_and_status |= HBA_PxCMD_ST; // Whenever this bit is changed from a ‘0’ to a ‘1’, the HBA starts processing the command list. This bit shall only be set to ‘1’ by software after PxCMD.FRE has been set to ‘1’.
}

static void port_configure(port_t *port)
{
    stop_cmd(port);

    void *base = kmalloc(4096, 1, 0);
    port->hba_port->cmd_list_base = (uint32_t)base;
    port->hba_port->cmd_list_base_uppper = 0;
    memset((void*)port->hba_port->cmd_list_base, 0 , 1024);

    void *fis_base = kmalloc(4096, 1, 0);
    port->hba_port->fis_base = (uint32_t)fis_base;
    port->hba_port->fis_base_upper = 0;
    memset((void*)port->hba_port->fis_base, 0 , 256); 

    hba_cmd_header_t *cmd_header = (hba_cmd_header_t*)port->hba_port->cmd_list_base;
    for (int i = 0; i < 32; i++)
    {
        cmd_header[i].prdt_length = 8;

        void *cmd_table_addr = kmalloc(4096, 1, 0);
        uint32_t addr = cmd_table_addr + (i << 8); 
        cmd_header[i].cmd_table_base = (uint32_t)addr;
        cmd_header[i].cmd_table_base_upper = 0;
        memset(addr, 0, 256); 
    }

    start_cmd(port);
}

int ahci_read_port(port_t *port, uint64_t sector, uint32_t sector_count, void *buffer)
{
    uint32_t spin = 0;
    while ((port->hba_port->task_file_data & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000)
    {
        spin++;
    }
    if (spin == 1000000) return 0; // read not successful

    uint32_t sector_low = (uint32_t)sector;
    uint32_t sector_high = (uint32_t)(sector >> 32);

    port->hba_port->interrupt_status = (uint32_t)-1;

    hba_cmd_header_t *cmd_header = (hba_cmd_header_t*)port->hba_port->cmd_list_base;
    cmd_header->cmd_fis_length = sizeof(FIS_REG_H2D_t) / sizeof(uint32_t);
    cmd_header->write = 0;
    cmd_header->prdt_length = 1;

    hba_cmd_table_t *cmd_table = (hba_cmd_table_t*)(cmd_header->cmd_table_base);
    memset(cmd_table, 0, sizeof(hba_cmd_table_t) + (cmd_header->prdt_length-1) * sizeof(hba_prdt_entry_t));

    cmd_table->prdt_entry[0].data_base_addr = (uint32_t)buffer;
    cmd_table->prdt_entry[0].data_base_addr_upper = 0;
    cmd_table->prdt_entry[0].byte_count = (sector_count << 9) - 1; // 512 bytes per sector
    cmd_table->prdt_entry[0].interrupt_on_completion = 1;

    FIS_REG_H2D_t *cmd_fis = (FIS_REG_H2D_t*)(&cmd_table->cmd_fis);
    cmd_fis->fis_type = FIS_TYPE_REG_H2D;
    cmd_fis->cmd_control = 1; // command
    cmd_fis->cmd = ATA_CMD_READ_DMA_EX;

    cmd_fis->lba0 = (uint8_t)(sector_low);
    cmd_fis->lba1 = (uint8_t)(sector_low >> 8);
    cmd_fis->lba2 = (uint8_t)(sector_low >> 16);
    cmd_fis->lba3 = (uint8_t)(sector_high);
    cmd_fis->lba4 = (uint8_t)(sector_high >> 8);
    cmd_fis->lba5 = (uint8_t)(sector_high >> 16);

    cmd_fis->device_register = 1 << 6; // set LBA mode

    cmd_fis->count_low = sector_count & 0xFF;
    cmd_fis->count_high = (sector_count >> 8) & 0xFF;

    port->hba_port->cmd_issue = 1;

    printf("");

    // while (1)
    // {
    //     if(port->hba_port->cmd_issue == 0) break; // cmd is done
    //     if(port->hba_port->interrupt_status & HBA_PxIS_TFES) // error
    //     {
    //         return 0;
    //     }
    // }

    if(port->hba_port->interrupt_status & HBA_PxIS_TFES) // error
    {
        return 0;
    }

    return 1;
}

void ahci_init(pci_device_t *pci)
{
    pci_device = pci;
    abar = (hba_memory_t*)pci->bar5;

    probe_ports();

    for (int i = 0; i < port_count; i++)
    {
        port_t *port = &ports[i];
        port_configure(port);

        if (port->port_type == SATA) console_log("SATA device found. Port number: %d", port->port_number);
        else if (port->port_type == SATAPI) console_log("SATAPI device found.");
        else console_log("Invalid port type.");

        port->buffer = kmalloc(4096, 1, 0);
        memset(port->buffer, 0, 4096);
        if(!ahci_read_port(port, 0, 4, port->buffer))
            console_error("Could not read sectors!");

        console_log("%s", (char*)(port->buffer));
    }
    
    console_log("AHCI driver initialized.");
}