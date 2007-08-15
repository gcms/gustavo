#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "codecalculator.h"
#include "hashtable.h"

static void calculate_redundancy_code(symbol_attr_t *a, size_t n) {
    int root;                  /* next root node to be used */
    int leaf;                  /* next leaf to be used */
    int next;                  /* next value to be assigned */
    int avbl;                  /* number of available nodes */
    int used;                  /* number of internal nodes */
    int dpth;                  /* current depth of leaves */

    /* check for pathological cases */
    if (n == 0) { return; }
    if (n == 1) { a[0].freq = 0; return; }

    /* first pass, left to right, setting parent pointers */
    a[0].freq += a[1].freq; root = 0; leaf = 2;
    for (next = 1; next < n - 1; next++) {
        /* select first item for a pairing */
        if (leaf >= n || a[root].freq < a[leaf].freq) {
            a[next].freq = a[root].freq; a[root++].freq = next;
        } else
            a[next].freq = a[leaf++].freq;

        /* add on the second item */
        if (leaf >= n || (root < next && a[root].freq < a[leaf].freq)) {
            a[next].freq += a[root].freq; a[root++].freq = next;
        } else
            a[next].freq += a[leaf++].freq;
    }
        
    /* second pass, right to left, setting internal depths */
    a[n - 2].freq = 0;
    for (next = n - 3; next >= 0; next--)
        a[next].freq = a[a[next].freq].freq + 1;

    /* third pass, right to left, setting leaf depths */
    avbl = 1; used = dpth = 0; root = n - 2; next = n - 1;
    while (avbl > 0) {
        while (root >= 0 && a[root].freq == dpth) {
            used++; root--;
        }
        while (avbl > used) {
            a[next--].freq = dpth; avbl--;
        }
        avbl = 2 * used; dpth++; used = 0;
    }
}

typedef struct {
    unsigned int base;
    unsigned int offset;
} base_offset_t;     

static base_offset_t *base_offset_new(symbol_attr_t *a, size_t n) {
    unsigned int first, last, i, index;
    base_offset_t *this = malloc(a[0].freq * sizeof(base_offset_t));

    index = n - 1;
    last = 0;
    for (i = 0; i < a[0].freq; i++) {
        unsigned int size = 0;
        first = index;
        while (a[index].freq == i + 1) {
            size++; index--;
        }

        this[i].base = i > 0 ? 2 * (this[i - 1].base + last) : 0;
        this[i].offset = index + 1;

        last = size;
    }

    return this;   
}
static void base_offset_delete(base_offset_t *this) {
    free(this);
}

static void get_code(code_t *this, const char *str, unsigned int i,
        const base_offset_t *bo) {
    unsigned int j;

    for (j = 0; i < bo[j].offset; j++);

    this->value = i + bo[j].base - bo[j].offset;
    this->bits = j + 1;
}

extern unsigned int strhash(const void *);
extern bool strequals(const void *, const void *);

static unsigned int codehash(const void *a) {
    return ((code_t *) a)->value + ((code_t *) a)->bits;
}

static bool codeequals(const void *a, const void *b) {
    const code_t *c1 = a;
    const code_t *c2 = b;
    return c1 == c2 || (c1->value == c2->value && c1->bits == c2->bits);
}


code_calculator_t *code_calculator_new(const symbol_attr_t *symbol, size_t n,
        code_operation_t op) {
    unsigned int i;
    base_offset_t *bo;
    code_calculator_t *this = malloc(sizeof(code_calculator_t));

    symbol_attr_t *tmp_symbols = malloc(n * sizeof(symbol_attr_t));
    memcpy(tmp_symbols, symbol, n * sizeof(symbol_attr_t));

    calculate_redundancy_code(tmp_symbols, n);

    bo = base_offset_new(tmp_symbols, n);

    this->codes = malloc(n * sizeof(code_t));

    this->op = op;

    if (op == DECODER) {
        this->table = hash_table_new(codehash, codeequals);
        for (i = 0; i < n; i++) {
            get_code(&this->codes[i], tmp_symbols[i].symbol, i, bo);

            hash_table_put(this->table,
                    &(this->codes[i]), (void *) tmp_symbols[i].symbol);
        }
    } else {
        this->table = hash_table_new(strhash, strequals);
        for (i = 0; i < n; i++) {
            get_code(&this->codes[i], tmp_symbols[i].symbol, i, bo);

            hash_table_put(this->table,
                    (void *) tmp_symbols[i].symbol, &(this->codes[i]));
        }
    }

    free(tmp_symbols);

    base_offset_delete(bo);


    return this;
}


code_t *code_calculator_get_code(code_calculator_t *this, const char *symbol) {
    assert(this->op != DECODER);
    return hash_table_get(this->table, symbol);
}

char *code_calculator_get_symbol(code_calculator_t *this, const code_t *code) {
    assert(this->op == DECODER);
    return hash_table_get(this->table, code);
}

void code_calculator_delete(code_calculator_t *this) {
    hash_table_delete(this->table, NULL, NULL);
    free(this->codes);
    free(this);
}
