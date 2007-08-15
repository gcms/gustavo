#include <stdio.h>
#include <errno.h>
#include "huffman.h"

int main(int argc, char *argv[]) {
    huffman_t *huff;
    size_t i;

    if (argc < 4) {
        fprintf(stderr, "usage: %s <option> <input> <output>\n"
                "<options>\n c\tcompress\n d\tdecompress\n", argv[0]);
        return -1;
    }

    if (argv[1][0] == 'c') {
        if ((huff = huffman_new_from_text_file(argv[2])) == NULL) {
            fprintf(stderr, "%s: %s\n", argv[2], strerror(errno));
            return -1;
        }
        if (!huffman_write_compressed_file(huff, argv[3])) {
            huffman_delete(huff);
            fprintf(stderr, "Error while trying to write compressed file\n");
            return -1;
        }

        huffman_delete(huff);
    } else if (argv[1][0] == 'd') {
        if ((huff = huffman_new_from_compressed_file(argv[2])) == NULL) {
            fprintf(stderr, "%s: %s\n", argv[2], strerror(errno));
            return -1;
        }
        if (!huffman_write_text_file(huff, argv[3])) {
            huffman_delete(huff);
            fprintf(stderr, "%s\n", strerror(errno));
            return -1;
        }
        huffman_delete(huff);
    } else {
        fprintf(stderr, "Invalid Option");
        fprintf(stderr, "usage: %s <option> <input> <output>\n"
                "<options>\n c\tcompress\n d\tdecompress\n", argv[0]);
    }

    return 0;
}
