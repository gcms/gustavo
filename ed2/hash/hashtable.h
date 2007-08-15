#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "base.h"

typedef unsigned int (*hash_func)(const void *key);
typedef bool (*equal_func)(const void *a, const void *b);

typedef struct {
    void *key;
    void *value;
} entry_t;

typedef struct {   
    size_t size;
    entry_t *table;

    size_t nelem;

    unsigned int colisions;
    
    hash_func h;
    equal_func eq;
} hash_table_t;

hash_table_t *hash_table_new(hash_func, equal_func);
bool hash_table_contains(hash_table_t *table, const void *key);
void *hash_table_put(hash_table_t *table, void *key, void *value);
void *hash_table_get(hash_table_t *table, const void *key);
void hash_table_delete(hash_table_t *table,
        destructor key_destructor, destructor value_destructor);
entry_t *hash_table_entries(hash_table_t *table);
#define hash_table_size(t) (t->nelem)
#define hash_table_colisions(t) (t->colisions)

#endif /* HASH_TABLE_H */
