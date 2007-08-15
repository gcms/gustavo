#include <stdio.h>
#include <stdlib.h>

void _my_assert(const char *file, int lineno, const char *e, const char *msg) {
    fprintf(stderr, "assertion \"%s\" failed: %s\nfile: %s, line %d\n",
            e, msg, file, lineno);
    abort();
}
