#ifndef QUICKSORT_H
#define QUICKSORT_H

void quicksort(void *base, size_t nmemb, size_t size,
        int (*compar)(const void *, const void *));


#endif /* QUICKSORT_H */
