#ifndef BASE_H
#define BASE_H

#include "myassert.h"

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

typedef uint uint32;
typedef ushort uint16;
typedef uchar uint8;

#ifndef bool
typedef int bool;
#define false 0
#define true (!false)
#endif

typedef int (*comparator)(const void *, const void *);

#endif /* BASE_H */
