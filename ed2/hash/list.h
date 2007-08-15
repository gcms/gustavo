#ifndef LIST_H
#define LIST_H

#include "base.h"
typedef bool (*list_handler)(const void *value, int index, void *data);

void *list_new(comparator cmp);

void list_set_comparator(void *l, comparator cmp);
bool list_append(void *l, void *data);
bool list_add(void *l,int index, void *data);
void list_clear(void *l, bool free_data);
bool list_contains(void *l, void *data);
void *list_get(void *l, int index);
int list_size(void *l);
bool list_remove(void *l, void *data, bool free_data);
void *list_remove_by_index(void *l, int index);
void *list_set(void *l, int index, void *data);
void list_iterate(void *l, list_handler hdl, void *data);
void *list_iterator(void *l);
void list_delete(void *l, bool free_data);


#endif /* LIST_H */
