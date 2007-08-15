/*
 * Copyright (C) 2005 Gustavo Cipriano Mota Sousa <gustavomota@inf.ufg.br>
 * hashtable.h
 */

#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include "base.h"

/** Defines the interface of a hash function. */
typedef unsigned int (*hash_func)(const void *key);

/** Defines the interface of a function to compare equality. */
typedef bool (*equal_func)(const void *a, const void *b);

/**
 * Interface used by the hash_table_foreach function.
 * @see hash_table_foreach
 */
typedef bool (*hash_table_handler)(const void *key, const void *value,
        void *data);

/** Interface used by functions that remove elements. */
typedef void (*delete_func)(void *data);

/**
 * Creates a hash table..
 * @param hash function used to calculate the hash values
 * @param equals function used to compare if two keys are equal.
 * @return a newly allocated hash table
 */
void *hash_table_new(hash_func hash, equal_func equals);

/**
 * Put an element in a hash table.
 * @param table table to which the key/value pair will be added
 * @param key key at which value will be added
 * @param value value to be added
 * @return the element previosly stored in the position indicated by key,
 * if there wasn't any returns NULL.
 */
void *hash_table_put(void *table, void *key, void *value);

/**
 * Get an element from a hash table.
 * @param table table to which the key will be searched for
 * @param key key at which the desired element is stored
 * @return the value contained at the position indicated by key, or NULL
 * if there isn't any element in this position.
 */
void *hash_table_get(void *table, const void *key);

/**
 * Returns a key from this table.
 * @param table table to which the key will be searched for
 * @param key the k
 * @return the key equals to the key param according to the equals function
 * passed to the hash_table_new when the table was created. Returns NULL
 * if the key is not found.
 */
void *hash_table_get_key(void *table, const void *key);


/**
 * Removes an element from this table.
 * @param table this table
 * @param key at which the element to be removed can be found.
 * @param key_del the function to delete the key element, NULL
 * if you don't want to delete it, or if it's a numerical values.
 * @return the value stored at the position specified by key
 */
void *hash_table_remove(void *table, const void *key, delete_func key_del);

/**
 * Gets the number of elements stored in this table.
 * @param table this table
 * @return size of the table
 */
size_t hash_table_size(void *table);

/**
 * Calls a callback function for each key/value pair in this table.
 * @param table this table
 * @param hdl function which will be executed
 * @param data aditional data information which will be passed to the hdl
 * function
 */
void hash_table_foreach(void *table, hash_table_handler hdl, void *data);


/**
 * Free memory used by this hash table.
 * @param table this table
 * @param key_del function used to delete key elements
 * @param value_del function used to delete value elements
 */
void hash_table_delete(void *table, delete_func key_del, delete_func value_del);


#endif /* HASH_TABLE_H_ */
