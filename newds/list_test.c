#include "list.h"
#include <assert.h>
#include <string.h>

static int
string_cmp(const void *a, const void *b)
{
    return strcmp((const char *) a, (const char *) b);
}

int main(int argc, char *argv[])
{
    list_t *list;

    list = list_new_full(string_cmp, NULL);


    assert(list_size(list) == 0);
    assert(list_append(list, "hello"));
    assert(list_find(list, "hello"));
    assert(strcmp(list_first(list), "hello") == 0);

    assert(list_prepend(list, "asdf"));
    assert(strcmp(list_first(list), "asdf") == 0);
    assert(strcmp(list_last(list), "hello") == 0);

    assert(list_size(list) == 2);
    assert(strcmp(list_get(list, 1), "hello") == 0);
    assert(strcmp(list_get(list, 0), "asdf") == 0);

    assert(list_remove(list, list_first(list)));

    assert(list_size(list) == 1);

    return 0;
}
