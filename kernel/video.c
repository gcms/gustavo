#include <system.h>
#include <stdlib.h>
#include <stdio.h>


static unsigned short *video = (unsigned short *) 0xB8000;

static unsigned short attrib = 0x0F00;

static unsigned int csr_x = 0;
static unsigned int csr_y = 0;

#define BLANK() (attrib | 0x20)

static void
scroll(void)
{
    unsigned short blank, temp;

    blank = BLANK();

    if (csr_y >= 25) {
        temp = csr_y - 25 + 1;
        memcpy(video, video + temp * 80, (25 - temp) * 80 * 2);

        memsetw(video + (25 - temp) * 80, blank, 80);
        csr_y = 25 - 1;
    }
}

static void
update_csr(void)
{
    unsigned short temp;

    temp = csr_y * 80 + csr_x;

    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}

void
cls(void)
{
    unsigned short blank;

    blank = BLANK();

    memsetw(video, blank, 80 * 25);
    
    csr_y = csr_x = 0;
    update_csr();
}

void
putch(unsigned char c)
{
    unsigned short *where;

    switch (c) {
        case '\b':
            if (csr_x > 0) {
                csr_x--;
                where = video + (csr_y * 80 + csr_x);
                *where = BLANK();
            }
            break;
        case '\t':
            csr_x = (csr_x + 8) & ~(8 - 1);
            break;
        case '\r':
            csr_x = 0;
            break;
        case '\n':
            csr_x = 0;
            csr_y++;
            break;
        default:
            where = video + (csr_y * 80 + csr_x);
            *where = c | attrib;
            csr_x++;
            break;
    }

    if (csr_x >= 80) {
        csr_x = 0;
        csr_y++;
    }

    scroll();
    update_csr();
}

static void
itoa(char *buf, int base, int d)
{
    char *p = buf;
    char *p1, *p2;
    unsigned long ud = d;
    int divisor = 10;
  
    /* If %d is specified and D is minus, put `-' in the head.  */
    if (base == 'd' && d < 0)
    {
        *p++ = '-';
        buf++;
        ud = -d;
    }
    else if (base == 'x')
        divisor = 16;

    /* Divide UD by DIVISOR until UD == 0.  */
    do
    {
        int remainder = ud % divisor;
      
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    }
    while (ud /= divisor);

    /* Terminate BUF.  */
    *p = 0;
  
    /* Reverse BUF.  */
    p1 = buf;
    p2 = p - 1;
    while (p1 < p2)
    {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
}

void
printf(const char *format, ...)
{
    char **arg = (char **) &format;
    int c;
    char buf[20];

    arg++;
  
    while ((c = *format++) != 0) {
        if (c != '%') {
	        putch (c);
        } else {
	        char *p;
	  
	        c = *format++;
	        switch (c) {
                case 'd':
                case 'u':
                case 'x':
                    itoa (buf, c, *((int *) arg++));
                    p = buf;
                    goto string;
                    break;
                case 's':
                    p = *arg++;
                    if (!p)
                        p = "(null)";
string:
                    while (*p)
                        putch (*p++);
                    break;
                default:
                    putch (*((int *) arg++));
                    break;
            }
        }
    }
}

void
puts(const char *text)
{
    while (*text) {
        putch(*text++);
    }

    putch('\n');
}

void
settextcolor(unsigned char forecolor, unsigned char backcolor)
{
    attrib = ((backcolor << 4) | (forecolor & 0x0F)) << 8;
}

void
video_init(void)
{
    cls();
}
