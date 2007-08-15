#ifndef CODE_CALCULATOR_H_
#define CODE_CALCULATOR_H_

#include "huffman.h"

typedef enum {
    CODER,
    DECODER
} code_operation_t;

typedef struct {
    unsigned int value;
    unsigned int bits;
} code_t;

typedef struct {
    void *table;
    code_t *codes;
    code_operation_t op;
} code_calculator_t;

code_calculator_t *code_calculator_new(const symbol_attr_t *symbols, size_t n,
        code_operation_t op);
code_t *code_calculator_get_code(code_calculator_t *this, const char *symbol);
char *code_calculator_get_symbol(code_calculator_t *this, const code_t *code);
void code_calculator_delete(code_calculator_t *this);

#endif /* CODE_CALCULATOR_H_ */
