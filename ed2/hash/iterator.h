#ifndef ITERATOR_H
#define ITERATOR_H

#include "base.h"

typedef struct {
    void (*next)(void *data);
    void *(*get)(void *data);
    bool (*has_more)(void *data);
} iterator_t;

#define iterator_next(it) (((iterator_t*)it)->next(it))
#define iterator_get(it) (((iterator_t *)it)->get(it))
#define iterator_has_more(it) (((iterator_t*)it)->has_more(it))

#endif /* ITERATOR_H */
