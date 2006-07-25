#include <stdio.h>
#include <assert.h>

#define DATA            0x00
#define ERROR           0x01
#define FEATURES        0x01
#define SECTOR_COUNT    0x02
#define SECTOR_NUMBER   0x03
#define CYLINDER_LOW    0x04
#define CYLINDER_HIGH   0x05
#define DRIVE           0x06
#define HEAD            0x06
#define STATUS          0x07
#define COMMAND         0x07

#define ALT_STATUS      0x02
#define DEV_CONTROL     0x02
#define FORWARD_ISA     0x03



#define COMMAND0        0x1F0
#define CONTROL0        0x3F4

#define COMMAND1        0x170
#define CONTROL1        0x374


/* Status register flags */
#define ERR_MASK    (1 << 0)
#define DRQ_MASK    (1 << 3)
#define DF_MASK     (1 << 5)
#define DRDY_MASK   (1 << 6)
#define BSY_MASK    (1 << 7)

static unsigned char device = 0;

static void
check_status(void)
{
    unsigned char status = inportb(COMMAND0 + STATUS);

    /* Device register:
     *  DEV shall indicate the selected device
     * Status register:
     *  BSY shall be cleared
     *  DRDY shall be set
     *  DF shall be cleared
     *  DRQ shall be cleared
     *  ERR shall be cleared
     */
    assert((status & BSY_MASK) || (status & DRQ_MASK));
    assert(status & DRDY_MASK);
    assert(!(status & DF_MASK));
    assert(!(status & ERR_MASK));

}

static void
hd_wait_busy(void)
{
    /* BSY flag */
    while (inportb(COMMAND0 + STATUS) & BSY_MASK);
}

static void
hd_wait_ready(void)
{
    /* DRQ flag */
    while (!(inportb(COMMAND0 + STATUS) & DRQ_MASK));
}

static void
hd_set_sector(unsigned long sector, unsigned char one)
{
    outportb(COMMAND0 + SECTOR_COUNT, one);
    outportb(COMMAND0 + SECTOR_NUMBER, sector & 0xFF);
    outportb(COMMAND0 + CYLINDER_LOW, (sector >> 8) & 0xFF);
    outportb(COMMAND0 + CYLINDER_HIGH, (sector >> 16) & 0xFF);
    outportb(COMMAND0 + HEAD, ((sector >> 24) & 0x0F) | 0xe0
            | ((device << 4) & 0x10));
    /* Only 4 bits of the last byte are significative */
    /* 0xe0 is used to enable LBA */
}

static void
hd_read_port_words(unsigned short port, unsigned short *data,
        unsigned int count)
{
    unsigned int i;

    for (i = 0; i < count; i++) {
        data[i] = inportw(port);
    }
}

static void
hd_write_port_words(unsigned short port, unsigned short *data,
        unsigned int count)
{
    unsigned int i;

    for (i = 0; i < count; i++) {
        outportw(port, data[i]);
    }
}

void
hd_read_sector(unsigned long sector, void *data)
{
    /* Waits the BSY flag to be cleared */
    hd_wait_busy();

    /* Section 8.34 of ATA6 spec defines values that should be
     * set on registers before issuing the command. These are set by
     * the following function */
    hd_set_sector(sector, 1);

    outportb(COMMAND0 + COMMAND, 0x20);

    check_status();


    /* Waits the DRQ flag to be set. */
    hd_wait_ready();

    /* Reading the data register. According to the ATA6 spec draft,
     * on section 7.6.3 "This register shall e accessed for host PIO
     * data transfer only when DRQ is set to one." */


    /* Once a read or write has been started, it should not be interrupted
     * by other operations in the same controller.
     * Section 7.6.4 states: "The results of a read during a PIO in or a write
     * during a PIO out are inderterminate */
    hd_read_port_words(COMMAND0 + DATA, data, 256);
}

void
hd_write_sector(unsigned long sector, void *data)
{
    hd_wait_busy();

    hd_set_sector(sector, 1);
    
    outportb(COMMAND0 + COMMAND, 0x30);

    check_status();

    hd_wait_ready();
    hd_write_port_words(COMMAND0 + DATA, data, 256);
}

void
hd_set_device(int dev)
{
    device = dev ? 1 : 0;
}
