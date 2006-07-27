#ifndef __ATA_H__
#define __ATA_H__


/* CONTROLLERS */
#define PRIMARY     0
#define SECONDARY   1

/* DEVICES */
#define MASTER      0
#define SLAVE       1


void ata_init(void);
void ata_read_sectors(unsigned char controller, unsigned char device,
        unsigned long sector, unsigned char sector_count, void *data);
void ata_write_sectors(unsigned char controller, unsigned char device,
        unsigned long sector, unsigned char sector_count, void *data);

#define ata_read_sector(c, d, s, b) ata_read_sectors(c, d, s, 1, b)
#define ata_write_sector(c, d, s, b) ata_write_sectors(c, d, s, 1, b)

#endif /* __ATA_H__ */
