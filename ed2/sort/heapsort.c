#include <string.h>
#include "heapsort.h"

static void swap(void *a, void *b, const size_t size) {
    char tmp[size];
    memcpy(tmp, a, size);
    memcpy(a, b, size);
    memcpy(b, tmp, size);
}

static void sift(void *start, void *end, size_t size, size_t incr,
        int (*compar)(const void *, const void *)) {
    void *root = start;
    void *child;

    for (incr *= 2; (child = root + incr) <= end; incr *= 2) {
        printf("%d\n", *(int *) root);
        if (child + size <= end && compar(child + size, child) > 0) {
            child += size;
        }

        if (compar(child, root) > 0) {
            swap(root, child, size);
            incr += child - root;
            root = child;
        } else {
            return;
        }
    }

}

void heapsort(void *base, size_t nmemb, size_t size,
        int (*compar)(const void *, const void *)) {
    void *start = base + (nmemb / 2) * size;
    void *end = base + (nmemb - 1) * size;

    printf("%d\n", *(int *) start);
    while (start > base) {
        start -= size;
        sift(start, end, size, start - base + size, compar);
    }
}
