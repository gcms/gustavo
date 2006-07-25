#ifndef __STDIO_H__
#define __STDIO_H__

unsigned char
inportb(unsigned short port);

void
outportb(unsigned short port, unsigned char data);

unsigned short
inportw(unsigned short port);

void
outportw(unsigned short port, unsigned short data);

#endif /* __STDIO_H__ */
