#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include "base.h"

typedef struct {
    unsigned int freq;
    const char *symbol;
} symbol_attr_t;

typedef struct {
    char **symbols;
    size_t pos_symbols;
    size_t num_symbols;

    symbol_attr_t *attrs;
    size_t pos_attrs;
    size_t num_attrs;
} huffman_t;

huffman_t *huffman_new_from_text_file(const char *filename);
bool huffman_write_compressed_file(huffman_t *huff, const char *filename);
huffman_t *huffman_new_from_compressed_file(const char *filename);
bool huffman_write_text_file(huffman_t *this, const char *filename);
void huffman_delete(huffman_t *this);

#endif /* HUFFMAN_H_ */
