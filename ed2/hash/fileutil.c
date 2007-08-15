#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "fileutil.h"

#define INITIAL_STRING_SIZE 21


char *get_string_token(FILE *fp, const char *sep) {
    char *result;
    size_t size;
    unsigned int pos;
    int ch;
   
    size = INITIAL_STRING_SIZE;
    result = malloc(sizeof(char) * size);
    assert(result);
    pos = 0;

    while (1) {
        ch = fgetc(fp);

        if (ch == EOF) 
            break;

        if (strchr(sep, ch) != NULL) {
            if (pos == 0) {
                /* checks if there are two subsequent separators */
                continue;
            } else {
                break;
            }
        }

        if (pos + 1 == size) {
            size *= 2;
            result = realloc(result, size);
            assert(result);
        }

        result[pos++] = ch;
    }
    result[pos] = 0;


    /* if EOF ocurred before anything was read */
    if (pos == 0) {
        free(result);
        result = NULL;
    }

    return result;
}

