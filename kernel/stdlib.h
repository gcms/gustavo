#ifndef __STDLIB_H__
#define __STDLIB_H__

#define NULL (void *)0

typedef unsigned long size_t;

typedef unsigned long uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

typedef signed long int32_t;
typedef signed short int16_t;
typedef signed char int8_t;

typedef unsigned char uchar_t;
typedef unsigned short ushort_t;
typedef unsigned int uint_t;
typedef unsigned long ulong_t;

typedef char char_t;
typedef short short_t;
typedef int int_t;
typedef long long_t;

typedef uint8_t byte_t;
typedef uint16_t word_t;
typedef uint32_t dword_t;

typedef int bool;
#define FALSE (0)
#define TRUE (!FALSE)

#define CHECK_FLAG(flags,bit)	((flags) & (1 << (bit)))
#define ALIGN(x, a) (((x) % (a)) ? ((x) + (a)) - ((x) % (a)) : (x))

void *
memcpy(void *dst, const void *src, size_t len);

void *
memset(void *dst, int c, size_t len);

unsigned short *
memsetw(unsigned short *dst, unsigned short val, size_t len);

size_t
strlen(const char *str);

#endif /* __STDLIB_H__ */
