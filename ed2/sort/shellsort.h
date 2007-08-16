#ifndef SHELLSORT_H
#define SHELLSORT_H


void shellsort(void *base, size_t nmemb, size_t size,
        int (*compar)(const void *, const void *));


#endif /* SHELLSORT_H */
