#include <stdlib.h>
#include <assert.h>

#include "hashtable.h"

#define INITIAL_SIZE 31

static unsigned int hash_key(const void *key, unsigned int i, size_t n,
        hash_func h) {
    return (h(key) + 31 * i + 701 * i * i) % n;
}


hash_table_t *hash_table_new(hash_func h, equal_func eq) {
    unsigned int i;
    hash_table_t *this = malloc(sizeof(hash_table_t));
    assert(this);

    this->size = INITIAL_SIZE;
    this->table = malloc(sizeof(entry_t) * this->size);
    assert(this->table);

    for (i = 0 ; i < this->size; i++) {
        this->table[i].key = NULL;
    }

    this->nelem = 0;

    this->colisions = 0;

    this->h = h;
    this->eq = eq;

    return this;
}

static int internal_add(entry_t *table, void *key, void *value,
        size_t n, hash_func h) {
    unsigned int i;

    for (i = 0; i < n; i++) {
        unsigned int k = hash_key(key, i, n, h);
        if (table[k].key == NULL) {
            table[k].key = key;
            table[k].value = value;
            return k;
        }
    }

    return -1;
}

static void grow(hash_table_t *this) {
    entry_t *old_table;
    size_t old_size;
    unsigned int i;

    old_table = this->table;
    old_size = this->size;

    this->size *= 2;
    this->table = malloc(sizeof(entry_t) * this->size);
    for (i = 0; i < this->size; i++) {
        this->table[i].key = NULL;
    }
    this->nelem = 0;

    for (i = 0; i < old_size; i++) {
        if (old_table[i].key != NULL) {
            hash_table_put(this, old_table[i].key, old_table[i].value);
        }
    }

    free(old_table);
}

static entry_t *get_entry(hash_table_t *this, const void *key) {
    unsigned int i;

    for (i = 0; i < this->size; i++) {
        unsigned int k = hash_key(key, i, this->size, this->h);

        if (this->table[k].key == NULL) {
            return NULL;
        } else if (this->eq(this->table[k].key, key)) {
            return &this->table[k];
        }
    }

    return NULL;
}

void *hash_table_put(hash_table_t *this, void *key, void *value) {
    entry_t *e = get_entry(this, key);

    if (e != NULL) {
        void *result = e->value;
        e->key = key;
        e->value = value;

        return result;
    }

    while (internal_add(this->table, key, value, this->size, this->h) == -1) {
        this->colisions++;
        grow(this);
    }

    this->nelem++;

    return NULL;
}


void *hash_table_get(hash_table_t *this, const void *key) {
    entry_t *e = get_entry(this, key);
    return e != NULL ? e->value : NULL;
}

void hash_table_delete(hash_table_t *this,
        destructor key_destructor, destructor value_destructor) {
    unsigned int i;

    for (i = 0; i < this->size; i++) {
        if (this->table[i].key != NULL) {
            if (key_destructor) {
                key_destructor(this->table[i].key);
            }
            if (value_destructor) {
                value_destructor(this->table[i].value);
            }
        }
    }

    free(this->table);
    free(this);
}

entry_t *hash_table_entries(hash_table_t *this) {
    unsigned int i, pos;
    entry_t *entries = malloc(sizeof(entry_t) * this->nelem);

    for (i = 0, pos = 0; pos < this->nelem && i < this->size; i++) {
        if (this->table[i].key != NULL) {
            entries[pos++] = this->table[i];
        }
    }

    assert(pos == this->nelem);

    return entries;
}
bool hash_table_contains(hash_table_t *this, const void *key) {
    return get_entry(this, key) != NULL;
}
