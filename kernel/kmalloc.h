#ifndef __KMALLOC_H__
#define __KMALLOC_H__

void *
sbrk(int len);

void *
kmalloc(size_t len);

void
kfree(void *data);

#endif /* __KMALLOC_H__ */
