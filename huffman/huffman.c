#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "huffman.h"
#include "codecalculator.h"
#include "bitbuffer.h"
#include "strutil.h"
#include "hashtable.h"
#include "fileutil.h"

static huffman_t *huffman_alloc() {
    huffman_t *this = malloc(sizeof(huffman_t));

    this->symbols = NULL;
    this->pos_symbols = 0;
    this->num_symbols = 0;

    this->attrs = NULL;
    this->pos_attrs = 0;
    this->num_attrs = 0;

    return this;
}

#define INITIAL_SYMBOL_LIST_SIZE 256
static void huffman_add_symbol(huffman_t *this, char *symbol) {
    if (this->pos_symbols >= this->num_symbols) {
        this->num_symbols = this->num_symbols == 0 ? INITIAL_SYMBOL_LIST_SIZE
                                                   : this->num_symbols * 2;
        this->symbols = realloc(this->symbols,
                this->num_symbols * sizeof(char *));
        assert(this->symbols);
    }

    this->symbols[this->pos_symbols++] = symbol;
}

#define INITIAL_ATTR_LIST_SIZE 91
static symbol_attr_t *huffman_add_attr(huffman_t *this, char *symbol) {
    if (this->pos_attrs >= this->num_attrs) {
        this->num_attrs = this->num_attrs == 0 ? INITIAL_ATTR_LIST_SIZE
                                               : this->num_attrs * 2;
        this->attrs = realloc(this->attrs,
                this->num_attrs * sizeof(symbol_attr_t));
        assert(this->attrs);
    }

    this->attrs[this->pos_attrs].symbol = symbol;
    this->attrs[this->pos_attrs].freq = 0;

    return &this->attrs[this->pos_attrs++];
}

unsigned int strhash(const void *p) {
    return hash((const char *) p);
}

bool strequals(const void *a, const void *b) {
    return a == b || strcmp((const char *) a, (const char *) b) == 0;
}

static int symbolcmp(const void *a, const void *b) {
    return ((symbol_attr_t *) a)->freq - ((symbol_attr_t *) b)->freq;
}

huffman_t *huffman_new_from_text_file(const char *filename) {
    huffman_t *this = huffman_alloc();
    FILE *fp;
    void *table;
    char *symbol;

    if ((fp = fopen(filename, "r")) == NULL) {
        huffman_delete(this);
        return NULL;
    }

    table = hash_table_new(strhash, strequals);

    while ((symbol = get_string_token(fp, " ")) != NULL) {
        symbol_attr_t *attr;
        attr = hash_table_get(table, symbol);
        if (attr != NULL) {
            huffman_add_symbol(this, hash_table_get_key(table, symbol));
            attr->freq++;
            free(symbol);
        } else {
            huffman_add_symbol(this, symbol);
            attr = huffman_add_attr(this, symbol);
            attr->freq++;
            hash_table_put(table, symbol, attr);
        }

    }

    hash_table_delete(table, NULL, NULL);

    qsort(this->attrs, this->pos_attrs, sizeof(symbol_attr_t), symbolcmp);

    return this;
}

static unsigned int calculate_table_size(huffman_t *this) {
    unsigned int size, i;

    for (size = 0, i = 0; i < this->pos_attrs; i++) {
        size += sizeof(uint16) + strlen(this->attrs[i].symbol) + 1;
    }

    return size;
}

bool huffman_write_compressed_file(huffman_t *this, const char *filename) {
    FILE *fp;
    code_calculator_t *calc;
    bitbuffer_t *buffer;
    unsigned int i;

    if ((fp = fopen(filename, "w")) == NULL) {
        return false;
    }

    buffer = bitbuffer_new();
    bitbuffer_add(buffer, calculate_table_size(this), sizeof(uint32) * 8);

    for (i = 0; i < this->pos_attrs; i++) {
        const char *ch;
        bitbuffer_add(buffer, this->attrs[i].freq, sizeof(uint16) * 8);
        for (ch = this->attrs[i].symbol; *ch; ch++) {
            bitbuffer_add(buffer, *ch, sizeof(char) * 8);
        }
        bitbuffer_add(buffer, 0, sizeof(char) * 8);
    }

    calc = code_calculator_new(this->attrs, this->pos_attrs, CODER);

    for (i = 0; i < this->pos_symbols; i++) {
        code_t *code = code_calculator_get_code(calc, this->symbols[i]);
        if (code == NULL) {
            /* TODO: handle */
            code_calculator_delete(calc);
            bitbuffer_delete(buffer);
            fclose(fp);

            fprintf(stderr, "Invalid Code");
            return false;
        }
        bitbuffer_add(buffer, code->value, code->bits);
    }

    code_calculator_delete(calc);

    for (i = 0; i < bitbuffer_num_bytes(buffer); i++) {
        if (fputc(bitbuffer_get(buffer, i), fp) == EOF) {
            /* TODO: handle */
            bitbuffer_delete(buffer);
            fclose(fp);

            perror("fputc");
            return false;
        }
    }


    bitbuffer_delete(buffer);

    fclose(fp);

    return true;
}

void huffman_delete(huffman_t *this) {
    free(this->attrs);
    free(this);
}

huffman_t *huffman_new_from_compressed_file(const char *filename) {
    huffman_t *this = huffman_alloc();
    FILE *fp;
    bitbuffer_t *buffer;
    code_calculator_t *calc;

    uint32 size;
    unsigned int position, end;
    int ch;

    if ((fp = fopen(filename, "r")) == NULL) {
        huffman_delete(this);
        return NULL;
    }
    

    buffer = bitbuffer_new();

    while ((ch = fgetc(fp)) != EOF) {
        bitbuffer_add(buffer, ch, sizeof(char) * 8);
    }

    position = 0;

    size = bitbuffer_get_bits(buffer, position, sizeof(uint32) * 8);
    position += sizeof(uint32) * 8;

    size += sizeof(uint32);
    size *= 8;

    while (position < size) {
        symbol_attr_t *attr;

        uint16 freq;
        char *str;
        size_t strsiz;
        size_t strpos;

        freq = bitbuffer_get_bits(buffer, position, sizeof(uint16) * 8);
        position += sizeof(uint16) * 8;

        strpos = 0;
        strsiz = 21;
        str = malloc(strsiz);

        while ((ch = bitbuffer_get_bits(buffer, position, 8)) != 0) {
            if (strpos == strsiz) {
                strsiz *= 2;
                str = realloc(str, strsiz);
                assert(str);
            }
            str[strpos++] = ch;
            position += 8;
        }
        position += 8;

        str[strpos] = 0;

        attr = huffman_add_attr(this, str);
        attr->freq = freq;
    }

    calc = code_calculator_new(this->attrs, this->pos_attrs, DECODER);


    end = bitbuffer_num_bytes(buffer) * 8;
    size = 1;
    while (position < end) {
        while (size <= end - position) {
            code_t code;
            char *symbol;

            code.value = bitbuffer_get_bits(buffer, position, size);
            code.bits = size;

            if ((symbol = code_calculator_get_symbol(calc, &code)) != NULL) {
                huffman_add_symbol(this, symbol);

                position += size;
                size = 1;
                break;
            } else {
                size++;
            }
        }

        if (size > end - position) {
            position++;
        }
    }

    code_calculator_delete(calc);

    bitbuffer_delete(buffer);

    fclose(fp);

    return this;
}
bool huffman_write_text_file(huffman_t *this, const char *filename) {
    FILE *fp;
    unsigned int i;

    if ((fp = fopen(filename, "w")) == NULL) {
        return false;
    }

    for (i = 0; i < this->pos_symbols; i++) {
        fprintf(fp, "%s%s",
                this->symbols[i], (i == this->pos_symbols - 1 ? "" : " "));
    }

    fclose(fp);

    return true;
}
