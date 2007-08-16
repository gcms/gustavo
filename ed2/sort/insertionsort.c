#include <stdlib.h>
#include <string.h>
#include "insertionsort.h"

void insertionsort(void *base, size_t nmemb, size_t size,
        int (*compar)(const void *, const void *)) {
    void *end, *tmp, *p, *q;

    end = base + (nmemb - 1) * size;
    tmp = malloc(size);

    for (p = base; p <= end; p += size) {
        memcpy(tmp, p, size);
        for (q = p - size; q >= base && compar(tmp, q) < 0; q -= size) {
            memcpy(q + size, q, size);
        }
        memcpy(q + size, tmp, size);
    }

    free(tmp);
}
