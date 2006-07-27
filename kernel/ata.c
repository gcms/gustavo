#include <stdio.h>
#include <assert.h>

#include <stdlib.h>
#include <video.h>

#include <ata.h>

/* Command Registers Offset */
#define DATA            0x00    /* RW */

#define ERROR           0x01    /* R */
#define FEATURES        0x01    /* W */

#define SECTOR_COUNT    0x02    /* RW */

#define LBA_LOW         0x03    /* RW */
#define SECTOR_NUMBER   0x03    /* RW */

#define LBA_MID         0x04    /* RW */
#define CYLINDER_LOW    0x04    /* RW */

#define LBA_HIGH        0x05    /* RW */
#define CYLINDER_HIGH   0x05    /* RW */

#define DEVICE          0x06    /* RW */
#define DRIVE           0x06    /* RW */
#define HEAD            0x06    /* RW */

#define STATUS          0x07    /* R */
#define COMMAND         0x07    /* W */

/* Control Registers Offset */
#define ALT_STATUS      0x02    /* R */
#define DEV_CONTROL     0x02    /* W */

#define FORWARD_ISA     0x03



/* Command and Control block start offsets */
#define COMMAND0        0x1F0
#define CONTROL0        0x3F4

#define COMMAND1        0x170
#define CONTROL1        0x374


/* STATUS register flags */
#define ERR_MASK    (1 << 0)
#define DRQ_MASK    (1 << 3)
#define DF_MASK     (1 << 5)
#define DRDY_MASK   (1 << 6)
#define BSY_MASK    (1 << 7)

/* DRIVE/DEVICE register flags */
#define DEV_MASK    (1 << 4)

/* DEV_CONTROL register flags */
#define nIEN_MASK   (1 << 1)
#define SRST_MASK   (1 << 2)
#define HOB_MASK    (1 << 7)

/* ERROR register flags */
#define ABRT_MASK   (1 << 2)

/* COMMANDS: */
#define READ_SECTOR 0x20
#define WRITE_SECTOR 0x30
#define IDENTIFY_DEVICE 0xEC
#define EXECUTE_DEVICE_DIAGNOSTIC 0x90

typedef struct {
    bool present;   /* Is the device present */

    unsigned char controller;
    unsigned char device;

    unsigned short command;
    unsigned short control;

    unsigned char serial[21];
    unsigned char firmware[9];
    unsigned char model[41];

    unsigned long sector_num;
} ata_disk_t;

typedef union {
    unsigned char byte[512];
    unsigned short word[256];
    unsigned long dword[128];
} sector_t;


/* 0 = PRIMARY MASTER, 1 = PRIMARY SLAVE,
 * 2 = SECONDARY MASTER, 3 = SECONDARY SLAVE */
static ata_disk_t disks[4];

#define     get_command(ctrl)   ((ctrl) == 0 ? COMMAND0 : COMMAND1)
#define     get_control(ctrl)   ((ctrl) == 0 ? CONTROL0 : CONTROL1)
#define     get_disk(c, d)      (&disks[2 * (c) + (d)])

#define     get_controller(d)   ((((ata_disk_t *) (d)) - disks) / 2)
#define     get_device(d)       ((((ata_disk_t *) (d)) - disks) % 2)

static void
read_port_words(unsigned short port, unsigned short *data,
        unsigned int count);

static void
wait_status(unsigned short command, unsigned char set, unsigned char clear)
{
    unsigned char status;

    do {
        status = inportb(command + STATUS);
    } while ((status & set) != set && (status & clear) != 0);
}

static void
execute_device_diagnostic(unsigned char controller)
{
    unsigned short command;
    unsigned char dev0_err, dev1_err;
    ata_disk_t *dev0, *dev1;

    assert(controller <= 1);

    command = get_command(controller);
    
    wait_status(command, 0, BSY_MASK | DRQ_MASK);

    /* "This command shall be accepted regardless of the state of DRDY" */

    /* "This command shall cause the devices to perform the internal
     * diagnostic tests. Both devices, if present, shall execute this
     * command regardless of which device is selected."*/
    outportb(command + COMMAND, EXECUTE_DEVICE_DIAGNOSTIC);

    /* Read ERROR registers from both devices */
    wait_status(command, 0, BSY_MASK | DRQ_MASK);
    outportb(command + DEVICE, 0xe0);    /* select device 0 */
    wait_status(command, 0, BSY_MASK);
    dev0_err = inportb(command + ERROR);

    wait_status(command, 0, BSY_MASK | DRQ_MASK);
    outportb(command + DEVICE, 0xf0);    /* select device 1 */
    wait_status(command, 0, BSY_MASK);
    dev1_err = inportb(command + ERROR);

    dev0 = get_disk(controller, 0);
    dev1 = get_disk(controller, 1);

    dev0->present = FALSE;
    dev1->present = FALSE;

    printf("dev0_err = 0x%x\n", dev0_err);
    printf("dev1_err = 0x%x\n", dev1_err);
    
    /* Table 26 is used for the following tests */
    if (dev0_err == 0x01 || dev0_err == 0x81) {
        /* Device 0 passed */
        dev0->present = TRUE;
    }
    
    if (!(dev0_err & 0x80)) {
        /* Device 1 passed or not present */
        if (dev1_err == 0x01) {
            /* Device 1 passed */
            dev1->present = TRUE;
        }
    }

    printf("controller %d\n", controller);
    printf("dev0 present? %s\n", dev0->present ? "YES" : "NO");
    printf("dev1 present? %s\n", dev1->present ? "YES" : "NO");
}

/* "A host should issue an IDENTIFY DEVICE and/or IDENTIFY PACKET DEVICE
 * command after the power-on or hardware reset protocol has completed to
 * determine the current status of features implemented by the device(s)." */
static bool
identify_device(unsigned char controller, unsigned char device, void *data)
{
    unsigned short command;

    assert(controller <= 1 && device <= 1);

    command = get_command(controller);

    /* Gotta write the DEVICE register. Section 7.7.3 states:
     * "This register shall be written only when both BSY and DRQ are
     * cleared to zero." */
    wait_status(command, 0, BSY_MASK | DRQ_MASK);

    /* Bit 4 of DEVICE register indicates which device */
    outportb(command + DEVICE, 0xe0 | ((device << 4) & 0x10));

    /* 8.15.7 IDENTIFY DEVICE prerequisites
     * DRDY set to one */

    wait_status(command, DRDY_MASK, BSY_MASK | DRQ_MASK);

    outportb(command + COMMAND, IDENTIFY_DEVICE);

    /* "When the command is issued the device sets the BSY bit to one,
     * prepares to transfer the 256 words of device identification data to the
     * host, sets the DRQ bit to one, clears BSY bit to zero, and asserts
     * INTRQ if nIEN is cleared to zero. The host may then transfer data by
     * reading the data register." */

    if ((inportb(command + STATUS) & ERR_MASK
                && inportb(command + ERROR) & ABRT_MASK)) {
        printf("device %d.%d maybe PACKET, check signature\n",
                controller, device);
        return FALSE;
    }

    wait_status(command, DRQ_MASK, BSY_MASK);

    read_port_words(command + DATA, data, 256);

    return TRUE;
}


static void
ascii_copy(void *dst, const void *src, size_t len)
{
    unsigned char *cdst = dst;
    const unsigned char *csrc = src;

    cdst[len] = 0;

    while (len > 0) {
        len -= 2;
        cdst[len] = csrc[len + 1];
        cdst[len + 1] = csrc[len];
    }
}

static void
init_disk_from_data(ata_disk_t *disk, sector_t *sector)
{
    /* Devices supporting the CFA feature set shall place the value 848Ah in
     * word 0. In this case, the above definitions for the bits in word 0 are
     * not valid. */
    if (sector->word[0] == 0x848A) {
        goto not_present;
    }

    /* Word 0: General configuration
     * Devices that conform to this standard shall clear bit 15 to zero */
    if (sector->word[0] & (1 << 15)) {
        goto not_present;
    }

    /* If bit 7 is set to one, the device is a removable media device.
     * We don't deal with them for now */
    if (sector->word[0] & (1 << 7)) {
        goto not_present;
    }

    /* If bit 2 is set to one it indicates that the content of the
     * IDENTIFY DEVICE response is incomplete. This will occur if the device
     * supports the Power-up in Standby feature set and required data is
     * contained on the device media. In this case the content of at least
     * word 0 and word 2 shall be valid. */
    if (sector->word[0] & (1 << 2)) {
        /* FIXME: not handled correctly */
        goto not_present;
    }

    ascii_copy(disk->serial, &sector->word[10], 20);
    ascii_copy(disk->firmware, &sector->word[23], 8);
    ascii_copy(disk->model, &sector->word[27], 40);

    /* Word (61:60) Total number of user addressable sectors */
    disk->sector_num = sector->dword[30];

    /* Word 64: PIO transfer modes supported */
    if (sector->word[53] & (1 << 1)) {
        if (!(sector->word[64] & (1 << 0))) {
            /* Doesn't support PIO mode 3 */
            goto not_present;
        }
    }

    disk->present = TRUE;

    return;

not_present:
    disk->present = FALSE;
    return;
}

static void
init_disk(ata_disk_t *disk)
{
    bool not_packet;
    sector_t sector;

    not_packet = identify_device(get_controller(disk), get_device(disk),
            &sector);

    if (not_packet) {
        disk->controller = get_controller(disk);
        disk->device = get_device(disk);
        disk->command = get_command(disk->controller);
        disk->control = get_control(disk->controller);

        init_disk_from_data(disk, &sector);
    } else {    /* is packet, we don't support for now */
        /* Load packet identify device */
        disk->present = FALSE;
    }
}

void
ata_init(void)
{
    int i;

    for (i = 0; i < 4; i++) {
        init_disk(&disks[i]);
/*
        if (disks[i].present) {
            printf("%d.%d \"%s\" \"%s\" \"%s\"\n",
                    disks[i].controller, disks[i].device,
                    disks[i].serial, disks[i].firmware, disks[i].model);
            assert(&disks[i] == get_disk(disks[i].controller, disks[i].device));
        } else {
            printf("%d.%d NOT PRESENT!\n",
                    get_controller(&disks[i]), get_device(&disks[i]));
        }
        */
    }
}


static void
regs_set_sector(ata_disk_t *disk, unsigned long sector, unsigned char one)
{
    outportb(disk->command + SECTOR_COUNT, one);
    outportb(disk->command + SECTOR_NUMBER, sector & 0xFF);
    outportb(disk->command + CYLINDER_LOW, (sector >> 8) & 0xFF);
    outportb(disk->command + CYLINDER_HIGH, (sector >> 16) & 0xFF);
    outportb(disk->command + HEAD, ((sector >> 24) & 0x0F) | 0xe0
            | ((disk->device << 4) & 0x10));
    /* Only 4 bits of the last byte are significative */
    /* 0xe0 is used to enable LBA */
}

static void
read_port_words(unsigned short port, unsigned short *data,
        unsigned int count)
{
    unsigned int i;

    for (i = 0; i < count; i++) {
        data[i] = inportw(port);
    }
}

static void
write_port_words(unsigned short port, unsigned short *data,
        unsigned int count)
{
    unsigned int i;

    for (i = 0; i < count; i++) {
        outportw(port, data[i]);
    }
}

static void
ata_disk_read_sectors(ata_disk_t *disk,
        unsigned long sector, unsigned char sector_count,
        void *data)
{
    assert(disk->present);
    assert(sector < disk->sector_num);

    if (sector_count <= 0)
        return;

    wait_status(disk->command, 0, BSY_MASK | DRQ_MASK);

    /* Setting registers */
    regs_set_sector(disk, sector, sector_count);

    /* 8.34.7 READ SECTOR prerequisites
     * DRDY set to one */
    wait_status(disk->command, DRDY_MASK, BSY_MASK | DRQ_MASK);
    outportb(disk->command + COMMAND, READ_SECTOR);

    assert(!(inportb(disk->command + STATUS) & ERR_MASK));

    wait_status(disk->command, DRQ_MASK, 0);

    read_port_words(disk->command + DATA, data, 256 * sector_count);
}

static void
ata_disk_write_sectors(ata_disk_t *disk,
        unsigned long sector, unsigned char sector_count,
        void *data)
{
    assert(disk->present);
    assert(sector < disk->sector_num);

    if (sector_count <= 0)
        return;

    wait_status(disk->command, 0, BSY_MASK | DRQ_MASK);

    regs_set_sector(disk, sector, sector_count);

    wait_status(disk->command, DRDY_MASK, BSY_MASK | DRQ_MASK);
    outportb(disk->command + COMMAND, WRITE_SECTOR);

    assert(!(inportb(disk->command + STATUS) & ERR_MASK));

    wait_status(disk->command, DRQ_MASK, 0);
    write_port_words(disk->command + DATA, data, 256 * sector_count);
}

void
ata_read_sectors(unsigned char controller, unsigned char device,
        unsigned long sector, unsigned char sector_count,
        void *data)
{
    ata_disk_read_sectors(get_disk(controller, device),
            sector, sector_count, data);
}

void
ata_write_sectors(unsigned char controller, unsigned char device,
        unsigned long sector, unsigned char sector_count,
        void *data)
{
    ata_disk_write_sectors(get_disk(controller, device),
            sector, sector_count, data);
}
