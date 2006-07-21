#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <paging.h>

typedef struct {
    phys_addr_t page_dir;

    unsigned int page_map_idx;

    /* 64 entries on the page dir (256 MB) is enough */
    virt_addr_t heap_first; /* Aligned on a 4-meg boundary */
    virt_addr_t heap_last;  /* Multiples of 4-meg boundary - 1 */

    virt_addr_t brk;        /* break. heap_first <= brk <= heap_last */
} process_t;

extern process_t *current_proc;

#endif /* __PROCESS_H__ */
