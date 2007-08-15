#ifndef BITBUFFER_H_
#define BITBUFFER_H_

typedef struct {
    size_t bits;
    size_t bytes;

    unsigned char *data;
    size_t data_size;
} bitbuffer_t;

bitbuffer_t *bitbuffer_new();
void bitbuffer_add(bitbuffer_t *this, unsigned int value, size_t bits);
#define bitbuffer_free_bits(buffer) (8 - buffer->bits)
#define bitbuffer_num_bytes(buffer) (buffer->bytes \
        + (buffer->bits == 0 ? 0 : 1))
#define bitbuffer_get(buffer,i) (buffer->data[i])
unsigned int bitbuffer_get_bits(bitbuffer_t *this,
        unsigned int start, unsigned int size);
void bitbuffer_set_bits(bitbuffer_t *this, unsigned int value,
        unsigned int start, unsigned int size);
void bitbuffer_delete(bitbuffer_t *this);

#endif /* BITBUFFER_H_ */
