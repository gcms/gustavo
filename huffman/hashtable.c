#include <stdlib.h>
#include <assert.h>
#include "hashtable.h"

typedef struct entry_ entry_t;

struct entry_ {
    void *key;
    void *value;
    entry_t *next;
};

#define HASH_TABLE_TYPE_ID 102134


typedef struct hash_table {
    int type_id;
    size_t size;

    entry_t **table;
    size_t table_size;
    
    hash_func hash;
    equal_func equals;
} hash_table_t;

static unsigned int directhash(const void *a) {
    return (unsigned int) a;
}

static bool directequals(const void *a, const void *b) {
    return a == b;
}

#define INITIAL_SIZE 21

void *hash_table_new(hash_func hash, equal_func equals) {
    hash_table_t *this;
    size_t i;
    
    this = malloc(sizeof(hash_table_t));

    this->type_id = HASH_TABLE_TYPE_ID;
    this->size = 0;

    this->table_size = INITIAL_SIZE;
    this->table = malloc(this->table_size * sizeof(entry_t *));
    for (i = 0; i < this->table_size; i++)
        this->table[i] = NULL;

    this->hash = hash ? hash : directhash;
    this->equals = equals ? equals : directequals;

    return this;
    
}

static void entry_delete(entry_t *entry,
        delete_func keydel, delete_func valdel) {
    entry_t *next;

    while (entry) {
        next = entry->next;
        if (keydel) keydel(entry->key);
        if (valdel) valdel(entry->value);
        free(entry);
        entry = next;
    }
}

void hash_table_delete(void *table,
        delete_func key_delete, delete_func value_delete) {
    hash_table_t *this = table;
    size_t i;

    if (this == NULL) return;

    my_assert(this->type_id == HASH_TABLE_TYPE_ID,
            "Invalid hash_table Pointer");

    for (i = 0; i < this->table_size; i++)
        entry_delete(this->table[i], key_delete, value_delete);

    free(this->table);
    free(this);

}

static entry_t *entry_find(entry_t *entry,
        const void *key, equal_func equals) {
    while (entry) {
    	printf("comparing \"%s\" to \"%s\"\n", key, entry->key);
        if (equals(entry->key, key)) {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}

static entry_t *entry_lookup(hash_table_t *this,
        const void *key) {
    unsigned int hash = this->hash(key) % this->table_size;
    return entry_find(this->table[hash], key, this->equals);
}

void *hash_table_get(void *table, const void *key) {
    hash_table_t *this = table;
    entry_t *entry;

    my_assert(this->type_id == HASH_TABLE_TYPE_ID,
            "Invalid hash_table Pointer");


    entry = entry_lookup(this, key);
    return entry ? entry->value : NULL;
}

void *hash_table_get_key(void *table, const void *key) {
    hash_table_t *this = table;
    entry_t *entry;

    my_assert(this->type_id == HASH_TABLE_TYPE_ID,
            "Invalid hash_table Pointer");

    entry = entry_lookup(this, key);
    return entry ? entry->key : NULL;
}

static entry_t *entry_remove(entry_t **entry,
        const void *key, equal_func equals) {
    entry_t **p;

    for (p = entry; *p; p = &(*p)->next) {
        if (equals((*p)->key, key)) {
            entry_t *result = *p;
            *p = result->next;
            return result;
        }
    }

    return NULL;
}

void *hash_table_remove(void *table, const void *key, delete_func key_delete) {
    hash_table_t *this = table;
    entry_t *entry;
    unsigned int hash;

    my_assert(this->type_id == HASH_TABLE_TYPE_ID,
            "Invalid hash_table Pointer");

    hash = this->hash(key) % this->table_size;

    entry = entry_remove(&this->table[hash], key, this->equals);

    if (entry) {
       void *value = entry->value;
       if (key_delete) key_delete(entry->key);
       free(entry);

       this->size--;
       return value;
    }

    return NULL;
}

static entry_t *entry_new(void *key, void *value, entry_t *next) {
    entry_t *this = malloc(sizeof(entry_t));
    this->key = key;
    this->value = value;
    this->next = next;

    return this;
}

void *hash_table_put(void *table, void *key, void *value) {
    hash_table_t *this = table;
    entry_t *entry;
    unsigned int hash;
    void *result;

    my_assert(this->type_id == HASH_TABLE_TYPE_ID,
            "Invalid hash_table Pointer");


    hash = this->hash(key) % this->table_size;

    if (hash == 14)
    	printf("put \"%s\"\n", key);

    entry = entry_find(this->table[hash], key, this->equals);

    if (entry) {
        result = entry->value;
        entry->value = value;
    } else {
        this->table[hash] = entry_new(key, value, this->table[hash]);
        this->size++;
        result = NULL;
    }


    return result;
}

size_t hash_table_size(void *table) {
    hash_table_t *this = table;

    my_assert(this->type_id == HASH_TABLE_TYPE_ID,
            "Invalid hash_table Pointer");

    return this->size;
}

void hash_table_foreach(void *table, hash_table_handler hdl, void *data) {
    hash_table_t *this = table;
    size_t i;

    my_assert(this->type_id == HASH_TABLE_TYPE_ID,
            "Invalid hash_table Pointer");

    for (i = 0; i < this->table_size; i++) {
        entry_t *entry = this->table[i];
        while (entry) {
            while (!hdl(entry->key, entry->value, data));
            entry = entry->next;
        }
    }
}
