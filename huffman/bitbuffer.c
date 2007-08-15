#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "bitbuffer.h"

#define BUFFER_INITIAL_CAPACITY 256
bitbuffer_t *bitbuffer_new() {
    bitbuffer_t *this = malloc(sizeof(bitbuffer_t));
    assert(this);

    this->bits = 0;
    this->bytes = 0;

    this->data_size = BUFFER_INITIAL_CAPACITY;
    this->data = malloc(this->data_size);
    assert(this->data);
    memset(this->data, 0, this->data_size);

    return this;
}

unsigned int bitbuffer_get_bits(bitbuffer_t *this,
        unsigned int start, unsigned int size) {
    unsigned int i;
    unsigned int bytes;
    unsigned int result;

    assert(size <= (sizeof(unsigned int) * 8));
    assert(start + size <= this->bytes * 8 + this->bits);

    bytes = start / 8;
    start = start % 8;

    result = 0;

    for (i = 0; i < size; i++) {
        result = result << 1;
        result |= ((this->data[bytes + (i + start) / 8] << ((start + i) % 8)) >> 7) & 1;
    }
    
    return result;
}

void bitbuffer_set_bits(bitbuffer_t *this, unsigned int value,
        unsigned int start, unsigned int size) {
    unsigned int i;
    unsigned int bytes;

    assert(size <= (sizeof(unsigned int) * 8));
    assert(start + size <= this->bytes * 8 + this->bits);

    bytes = start / 8;
    start = start % 8;

    for (i = 0; i < size; i++) {
        unsigned char tmp = this->data[bytes + (start + i) / 8];
        size_t left_shift = (start + i) % 8 + 1;
        size_t right_shift = 8 - (start + i) % 8;
        
        tmp = ((value >> (size - i - 1)) & 1) << (7 - (start + i) % 8)
            | (((tmp >> right_shift) & 0xFF) << right_shift)
            | (((tmp << left_shift) & 0xFF) >> left_shift);

        this->data[bytes + (start + i) / 8] = tmp;
    }
}

void bitbuffer_add(bitbuffer_t *this, unsigned int value, size_t bits) {
    size_t old_bytes = this->bytes;
    size_t old_bits = this->bits;

    if (this->data_size <= this->bytes + bits / 8 + 1) {
        size_t old_size = this->data_size;
        this->data_size *= 2;
        this->data = realloc(this->data, this->data_size);
        assert(this->data);
        memset(this->data + old_size, 0, this->data_size - old_size);
    }

    assert(bits <= (sizeof(unsigned int) * 8));

    this->bits += bits;
    this->bytes += this->bits / 8;
    this->bits = this->bits % 8;

    bitbuffer_set_bits(this, value, old_bytes * 8 + old_bits, bits);

    return;
   

    value = (value << (sizeof(unsigned int) * 8 - bits))
        >> (sizeof(unsigned int) * 8 - bits);

    if (bits <= 8 - this->bits) {
        this->data[this->bytes] |= value << (8 - this->bits - bits);
        this->bits += bits;
        if (this->bits == 8) {
            this->bits = 0;
            this->bytes++;
        }
    } else {
        bits -= 8 - this->bits;

        this->data[this->bytes] |= value >> bits;
        this->bits = 0;
        this->bytes++;
        bitbuffer_add(this, value, bits);
    }
}

void bitbuffer_delete(bitbuffer_t *this) {
    free(this->data);
    free(this);
}
