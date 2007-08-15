#include <stdlib.h>
#include <string.h>

#include "quicksort.h"

static void swap(void *a, void *b, const size_t size) {
    char tmp[size];
    memcpy(tmp, a, size);
    memcpy(a, b, size);
    memcpy(b, tmp, size);
}

static void *partition(void *start, void *end,  size_t size,
        int (*compar)(const void *, const void *)) {
    void *p;
    void *pivot = start + (((end - start) / size / 2) * size);

    swap(pivot, end, size);

    for (p = start; p < end; p += size) {
        if (compar(p, end) <= 0) {
            swap(start, p, size);
            start += size;
        }
    }

    swap(start, end, size);

    return start;
}

static void sort(void *start, void *end, size_t size,
        int (*compar)(const void *, const void*)) {
    void *pivot;

    if (end > start) {
        pivot = partition(start, end, size, compar);

        sort(start, pivot - size, size, compar);
        sort(pivot + size, end, size, compar);
    }
}

void quicksort(void *base, size_t nmemb, size_t size,
        int (*compar)(const void *, const void *)) {
    sort(base, base + ((nmemb - 1) * size), size, compar);
}
