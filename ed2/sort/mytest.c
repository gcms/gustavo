#include <stdio.h>
#include "heapsort.h"

int compar(const void *a, const void *b) {
    return *(int *) a - *(int *) b;
}

void print_array(int *a, size_t size) {
        while (size-- > 0) {
                    printf("%d%c", a[size], size == 0 ? '\n' : ' ');
                        }
}

int main() {
    int a[] = { 23, 21, 1, 3, 5, 1234, 343, 51, 123, 663, 43, 90 };
    print_array(a, 12);
    heapsort(a, 12, sizeof(int), compar);
    print_array(a, 12);

    return 0;
}
