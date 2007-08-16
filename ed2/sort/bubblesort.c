#include <stdlib.h>
#include <string.h>

#include "bubblesort.h"

void bubblesort(void *base, size_t nmemb, size_t size,
        int (*compar)(const void *, const void *)) {
    void *last_change, *current, *change;

    void *tmp = malloc(size);

    for (last_change = base + size * (nmemb - 1); last_change != base;
            last_change = change) {
        change = base;
        for (current = base; current < last_change; current += size) {
            if (compar(current, current + size) > 0) {
                /* swap, (current) by (current + 1) */
                memcpy(tmp, current, size);
                memcpy(current, current + size, size);
                memcpy(current + size, tmp, size);

                change = current;
            }
        }
    }

    free(tmp);
}
