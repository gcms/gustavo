#ifndef BASE_H
#define BASE_H

#include "myassert.h"

#ifndef bool
typedef int bool;
#define false 0
#define true (!false)
#endif

typedef int (*comparator)(const void *, const void *);
typedef void (*destructor)(void *);

#endif /* BASE_H */
