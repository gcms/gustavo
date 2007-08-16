#include <stdlib.h>
#include <string.h>
#include "mergesort_.h"

static void sort(void *start, void *end, size_t size,
        int (*compar)(const void *, const void *)) {
    void *mid, *tmp, *p, *q, *r;

    if (end == start) {
        return;
    }

    mid = start + (((end - start) / size / 2) * size);

    sort(start, mid, size, compar);
    sort(mid + size, end, size, compar);
    
    tmp = malloc(end - start + size);

    for (r = tmp, p = start, q = mid + size; p <= mid && q <= end; r += size) {
        if (compar(p, q) <= 0) {
            memcpy(r, p, size);
            p += size;
        } else if (q <= end) {
            memcpy(r, q, size);
            q += size;
        }
    }

    while (p <= mid) {
        memcpy(r, p, size);
        p += size;
        r += size;
    }

    while (q <= end) {
        memcpy(r, q, size);
        q += size;
        r += size;
    }

    memcpy(start, tmp, end - start + size);
    free(tmp);
}

void mergesort_(void *base, size_t nmemb, size_t size,
        int (*compar)(const void *, const void *)) {
    sort(base, base + (nmemb - 1) * size, size, compar);
}
