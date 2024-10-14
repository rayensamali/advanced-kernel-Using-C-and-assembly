#include "io.h"
#include "kernel.h"

// ATA command codes
#define ATA_CMD_READ_SECTORS  0x20
#define ATA_CMD_WRITE_SECTORS 0x30

// ATA status flags
#define ATA_STATUS_BSY 0x80    // Busy
#define ATA_STATUS_DRQ 0x08    // Data request ready

// ATA I/O Ports for Primary Controller
#define ATA_PRIMARY_COMMAND_PORT   0x1F7
#define ATA_PRIMARY_DATA_PORT      0x1F0
#define ATA_PRIMARY_DRIVE_SELECT   0x1F6
#define ATA_PRIMARY_SECTOR_COUNT   0x1F2
#define ATA_PRIMARY_SECTOR_NUMBER  0x1F3
#define ATA_PRIMARY_CYLINDER_LOW   0x1F4
#define ATA_PRIMARY_CYLINDER_HIGH  0x1F5

// Function prototypes
void ata_wait_busy();
void ata_wait_ready();
int ata_read_sector(unsigned int lba, unsigned char *buffer);
int ata_write_sector(unsigned int lba, const unsigned char *buffer);

// Wait for the disk to be ready (clear BSY bit)
void ata_wait_busy() {
    print_string("Waiting for ATA drive to be ready...\n");
    while (inb(ATA_PRIMARY_COMMAND_PORT) & ATA_STATUS_BSY);
    print_string("ATA drive is ready (BSY cleared).\n");
    
}

// Wait until the DRQ bit is set (indicates the drive is ready to transfer data)
void ata_wait_ready() {
    print_string("Waiting for ATA drive to request data transfer (DRQ)...\n");
    while (!(inb(ATA_PRIMARY_COMMAND_PORT) & ATA_STATUS_DRQ));
    print_string("ATA drive is ready for data transfer (DRQ set).\n");
}

// Read a sector using LBA from the disk
int ata_read_sector(unsigned int lba, unsigned char *buffer) {
    print_string("Reading from sector: ");
    print_int(lba);
    kprint_newline();

    ata_wait_busy();
    outb(ATA_PRIMARY_DRIVE_SELECT, 0xE0 | ((lba >> 24) & 0x0F));  // Select drive and LBA mode
    outb(ATA_PRIMARY_SECTOR_COUNT, 1);                            // Number of sectors to read
    outb(ATA_PRIMARY_SECTOR_NUMBER, (unsigned char)lba);          // LBA low byte
    outb(ATA_PRIMARY_CYLINDER_LOW, (unsigned char)(lba >> 8));    // LBA mid byte
    outb(ATA_PRIMARY_CYLINDER_HIGH, (unsigned char)(lba >> 16));  // LBA high byte
    outb(ATA_PRIMARY_COMMAND_PORT, ATA_CMD_READ_SECTORS);         // Issue the read command

    ata_wait_ready();

    for (int i = 0; i < 256; i++) {
        unsigned short data = inw(ATA_PRIMARY_DATA_PORT);
        buffer[i * 2] = data & 0xFF;
        buffer[i * 2 + 1] = (data >> 8) & 0xFF;
    }

    print_string("Read data: ");
    for (int i = 0; i < 10; i++) {  // Print first 10 bytes for debugging
        print_int(buffer[i]);
        print_string(" ");
    }
    kprint_newline();

    return 0;
}


// Write a sector using LBA to the disk
int ata_write_sector(unsigned int lba, const unsigned char *buffer) {
    print_string("Writing to sector: ");
    print_int(lba);
    kprint_newline();

    ata_wait_busy();
    outb(ATA_PRIMARY_DRIVE_SELECT, 0xE0 | ((lba >> 24) & 0x0F));  // Select drive and LBA mode
    outb(ATA_PRIMARY_SECTOR_COUNT, 1);                            // Number of sectors to write
    outb(ATA_PRIMARY_SECTOR_NUMBER, (unsigned char)lba);          // LBA low byte
    outb(ATA_PRIMARY_CYLINDER_LOW, (unsigned char)(lba >> 8));    // LBA mid byte
    outb(ATA_PRIMARY_CYLINDER_HIGH, (unsigned char)(lba >> 16));  // LBA high byte
    outb(ATA_PRIMARY_COMMAND_PORT, ATA_CMD_WRITE_SECTORS);        // Issue the write command

    ata_wait_ready();

    for (int i = 0; i < 256; i++) {
        unsigned short data = buffer[i * 2] | (buffer[i * 2 + 1] << 8);
        outw(ATA_PRIMARY_DATA_PORT, data);
    }

    print_string("Data written: ");
    for (int i = 0; i < 10; i++) {  // Print first 10 bytes for debugging
        print_int(buffer[i]);
        print_string(" ");
    }
    kprint_newline();

    return 0;
}


