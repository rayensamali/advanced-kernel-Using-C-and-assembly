#ifndef DISK_IO_H
#define DISK_IO_H

#define ATA_PRIMARY_DATA_PORT       0x1F0
#define ATA_PRIMARY_ERROR_PORT      0x1F1
#define ATA_PRIMARY_SECTOR_COUNT    0x1F2
#define ATA_PRIMARY_SECTOR_NUMBER   0x1F3
#define ATA_PRIMARY_CYLINDER_LOW    0x1F4
#define ATA_PRIMARY_CYLINDER_HIGH   0x1F5
#define ATA_PRIMARY_DRIVE_SELECT    0x1F6
#define ATA_PRIMARY_STATUS_PORT     0x1F7
#define ATA_PRIMARY_COMMAND_PORT    0x1F7

#define ATA_CMD_READ_SECTORS        0x20
#define ATA_CMD_WRITE_SECTORS       0x30
#define ATA_STATUS_BSY              0x80
#define ATA_STATUS_DRQ              0x08

int ata_read_sector(unsigned int lba, unsigned char *buffer);
int ata_write_sector(unsigned int lba,  const unsigned char *buffer);

#endif
