#include <stdlib.h>
#include <string.h>
#include "shellsort.h"

static const unsigned int steps[] = {
    88573, 29524, 9841, 3280, 1093, 364, 121, 40, 13, 4, 1
};
static const size_t num_steps = 11;
void shellsort(void *base, size_t nmemb, size_t size,
        int (*compar)(const void *, const void *)) {
    unsigned int i;
    void *end, *value, *p, *q;

    end = base + (nmemb - 1) * size;
    value = malloc(size);

    for (i = 0; i < num_steps; i++) {
        unsigned int step_size = steps[i] * size;
        for (p = base + step_size; p <= end; p += size) {
            memcpy(value, p, size);
            for (q = p - step_size; q >= base && compar(q, value) > 0;
                    q -= step_size) {
                memcpy(q + step_size, q, size);
            }
            memcpy(q + step_size, value, size);
        }
    }

    free(value);
}
