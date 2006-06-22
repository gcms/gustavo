#ifndef LIST_H
#define LIST_H

#include "base.h"

typedef struct list_ list_t;


list_t *list_new_full(compare_func_t comparator,
        destroy_func_t destructor);
list_t *list_new(void);


boolean list_insert(list_t *list, int index, void *data);
boolean list_append(list_t *list, void *data);
boolean list_prepend(list_t *list, void *data);

void *list_remove(list_t *list, void *data);
void *list_find(list_t *list, void *data);
void *list_get(list_t *list, int index);
void *list_first(list_t *list);
void *list_last(list_t *list);

int list_size(list_t *list);

void list_clear(list_t *list);

void list_delete(list_t *list);

#endif /* LIST_H */
