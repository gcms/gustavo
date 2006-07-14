#include <stdlib.h>

void *
memcpy(void *dest, const void *src, size_t count)
{
    const unsigned char *sp = src;
    unsigned char *dp = dest;
    
    for (/* */; count != 0; count--)
        *dp++ = *sp++;

    return dest;
}

void *
memset(void *dest, int val, size_t count)
{
    unsigned char *temp = dest;
    for (/* */; count != 0; count--)
        *temp++ = val;
    
    return dest;
}

unsigned short *
memsetw(unsigned short *dest, unsigned short val, size_t count)
{
    unsigned short *temp = dest;
    for (/* */; count != 0; count--)
        *temp++ = val;
    
    return dest;
}

size_t
strlen(const char *str)
{
    size_t retval;
    
    for (retval = 0; *str != '\0'; str++)
        retval++;

    return retval;
}
