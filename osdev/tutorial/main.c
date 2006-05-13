#include <system.h>

unsigned char *memcpy(void *dest, const void *src, int count)
{
    char *d = dest;
    const char *s = src;

    while (count--) {
        d[count] = s[count];
    }

    return dest;
}

unsigned char *memset(void *dest, int val, int count)
{
    char *d = dest;

    while (count--) {
        d[count] = val;
    }

    return dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, int count)
{
    while (count--) {
        dest[count] = val;
    }

    return dest;
}

size_t strlen(const char *str)
{
    size_t count = 0;

    while (*str++) {
        count++;
    }

    return count;
}

/* We will use this later on for reading from the I/O ports to get data
*  from devices such as the keyboard. We are using what is called
*  'inline assembly' in these routines to actually do the work */
unsigned char inportb (unsigned short _port)
{
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

/* We will use this to write to I/O ports to send bytes to devices. This
*  will be used in the next tutorial for changing the textmode cursor
*  position. Again, we use some inline assembly for the stuff that simply
*  cannot be done in C */
void outportb (unsigned short _port, unsigned char _data)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

/* This is a very simple main() function. All it does is sit in an
*  infinite loop. This will be like our 'idle' loop */
int main()
{
    /* You would add commands after here */
    gdt_install();
    idt_install();
    isrs_install();
    irq_install();

    __asm__ __volatile__ ("sti");

    init_video();

    timer_install();
    kb_install();


    puts("Hello World!");

    /* ...and leave this loop in. There is an endless loop in
    *  'start.asm' also, if you accidentally delete this next line */
    for (;;);

    return 0;
}
	
