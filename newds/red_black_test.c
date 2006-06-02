#include "red_black.h"

#include <assert.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int red_black_strcmp(const void *a, const void *b) {
	return strcmp((const char *) a, (const char *) b);
}

static boolean red_black_inorder_itr(void *data, void *user_data) {
    static char *str = NULL;

    if (str != NULL) {
        assert(red_black_strcmp(str, data) <= 0);
        assert(red_black_strcmp(data, str) >= 0);
    }

    str = data;

	return TRUE;
}

int main() {
	red_black_t *rb;

	assert(rb = red_black_new_full(bst_strcmp, NULL, TRUE));
	assert(red_black_insert(rb, "hello"));
	assert(red_black_insert(rb, "world"));
	assert(red_black_insert(rb, "abacaxi"));
	assert(red_black_insert(rb, "tosco"));

    assert(strcmp(red_black_remove(rb, "hello"), "hello") == 0);
    assert(red_black_remove(rb, "hello") == NULL);

	red_black_inorder(rb, red_black_inorder_itr, NULL);

	return 0;
}
