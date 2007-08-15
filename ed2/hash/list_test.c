#include <stdio.h>
#include "list.h"
#include "iterator.h"

#include <string.h>

int my_strcmp(const void *d1, const void *d2) {
    return strcmp((char *) d1, (char *) d2);
}

int main(int argc, char *argv[]) {
    void *list = list_new(my_strcmp);
    void *it;

    list_append(list, "tosco");
    list_append(list, "doido");
    list_add(list, 2, "meu_god");

    printf("%d\n", list_size(list));
    if (list_contains(list, "doido")) {
        printf("%s\n", (char *) list_get(list, 0));
    }

    for (it = list_iterator(list); iterator_has_more(it); iterator_next(it)) {
        printf("%s\n", (void *) iterator_get(it));
    }

    return 0;
}
