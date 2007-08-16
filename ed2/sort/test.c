#include <stdio.h>
#include <stdlib.h>
#include "insertionsort.h"
#include "mergesort_.h"
#include "bubblesort.h"
#include "quicksort.h"
#include "shellsort.h"

typedef struct {
    int day;
    int month;
    int year;
} Date;

int myfunc(const void *a, const void *b) {
    const Date *d1 = a;
    const Date *d2 = b;

    return d1->year != d2->year ? d1->year - d2->year
        : d1->month != d2->month ? d1->month - d2->month
        : d1->day - d2->day;
}

#include <sys/time.h>
unsigned long get_time() {
    struct timeval tp;
    if (gettimeofday(&tp, NULL) == -1) {
        return 0;
    }
    return tp.tv_sec * 1000000 + tp.tv_usec;
}

void run_sort(void *orig, size_t nelem, size_t size,
        int (*compar)(const void *, const void *),
        void (*sorter)(void *, size_t, size_t,
                int (*compar)(const void *, const void *)),
        const char *sortername) {
    unsigned int start, end;

    start = get_time();
    sorter(orig, nelem, size, compar);
    end = get_time();
    printf("%s:\n", sortername);
    
    if (start != 0 && end != 0) {
        printf("time spent: %u usec\n", sortername, end - start);
    } else {
        printf("error while trying to get time spent");
    }
}
#include <assert.h>

void assert_sorted(void *array, size_t nmemb, size_t size,
        int (*compar)(const void *, const void *)) {
    void *p, *end;

    end = array + (nmemb - 1) * size;

    for (p = array; p < end; p += size) {
        assert(compar(p, p + size) <= 0);
    }
}

#define SIZE 10000
int main() {
    int i;
    Date *orig, *tosort;

    orig = malloc(SIZE * sizeof(Date));

    srand(time(NULL) + clock());

    for (i = 0; i < SIZE; i++) {
        orig[i].year = rand() % 1000 + 1000;
        orig[i].month = rand() % 12 + 1;
        orig[i].day = rand() % 30 + 1;
    }

    tosort = malloc(SIZE * sizeof(Date));

    memcpy(tosort, orig, sizeof(Date) * SIZE);
    run_sort(tosort, SIZE, sizeof(Date), myfunc, insertionsort, "insertionsort");
    assert_sorted(tosort, SIZE, sizeof(Date), myfunc);

    memcpy(tosort, orig, sizeof(Date) * SIZE);
    run_sort(tosort, SIZE, sizeof(Date), myfunc, mergesort_, "mergesort_");
    assert_sorted(tosort, SIZE, sizeof(Date), myfunc);

    memcpy(tosort, orig, sizeof(Date) * SIZE);
    run_sort(tosort, SIZE, sizeof(Date), myfunc, bubblesort, "bubblesort");
    assert_sorted(tosort, SIZE, sizeof(Date), myfunc);

    memcpy(tosort, orig, sizeof(Date) * SIZE);
    run_sort(tosort, SIZE, sizeof(Date), myfunc, quicksort, "quicksort");
    assert_sorted(tosort, SIZE, sizeof(Date), myfunc);

    memcpy(tosort, orig, sizeof(Date) * SIZE);
    run_sort(tosort, SIZE, sizeof(Date), myfunc, shellsort, "shellsort");
    assert_sorted(tosort, SIZE, sizeof(Date), myfunc);

    /*
    for (i = 0; i < 1000; i++) {
        printf("%02d/%02d/%04d\n", orig[i].day, orig[i].month, orig[i].year);
    }
    */

    return 0;
}
