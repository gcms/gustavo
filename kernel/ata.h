#ifndef __ATA_H__
#define __ATA_H__

void hd_set_device(int device);
void hd_read_sector(unsigned long sector, void *data);
void hd_write_sector(unsigned long sector, void *data);

#endif /* __ATA_H__ */
