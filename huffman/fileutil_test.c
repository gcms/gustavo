#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "fileutil.h"

int main(int argc, char *argv[]) {
    FILE *fp;

    fp = fopen("fileutil_test.c", "r");

    assert(strcmp(get_string_token(fp, "\n"), "#include <stdio.h>") == 0);
    assert(strcmp(get_string_token(fp, "\n"), "#include <assert.h>") == 0);
    assert(strcmp(get_string_token(fp, "\n"), "#include <string.h>") == 0);
    assert(strcmp(get_string_token(fp, "\n"), "#include \"fileutil.h\"") == 0);

    return 0;
}
