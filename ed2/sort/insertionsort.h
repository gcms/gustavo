#ifndef INSERTIONSORT_H
#define INSERTIONSORT_H


void insertionsort(void *base, size_t nmemb, size_t size,
        int (*compar)(const void *, const void *));


#endif /* INSERTIONSORT_H */
