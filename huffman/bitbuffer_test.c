#include <stdlib.h>

#include "bitbuffer.h"

int main(int argc, char *argv[]) {
    bitbuffer_t *buffer;

    buffer = bitbuffer_new();

    bitbuffer_add(buffer, 0xff, 32);
    bitbuffer_set_bits(buffer, 0xff, 4, 8);
    printf("%x\n", buffer->data[0]);
    printf("%x\n", buffer->data[1]);
    printf("%x\n", buffer->data[2]);
    printf("%x\n", buffer->data[3]);

    bitbuffer_set_bits(buffer, 0x3a0, 4, 10);
    printf("%x\n", buffer->data[0]);
    printf("%x\n", buffer->data[1]);
    printf("%x\n", bitbuffer_get_bits(buffer, 4, 10));

    return 0;
}
