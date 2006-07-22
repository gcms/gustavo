#ifndef __PAGING_H__
#define __PAGING_H__

typedef unsigned long phys_addr_t;
typedef unsigned long virt_addr_t;

typedef unsigned long page_dir_entry_t;
typedef page_dir_entry_t *page_dir_t;
typedef unsigned long page_tbl_entry_t;
typedef page_tbl_entry_t *page_tbl_t;

typedef phys_addr_t page_t;
typedef phys_addr_t page_frame_t;


#ifndef __PROCESS_H__
#include <process.h>

extern process_t kernel_proc;

virt_addr_t page_sbrk(process_t *proc, int n_pages);
#define page_skbrk(n) page_sbrk(&kernel_proc, (n))
#endif


void
paging_init(void);


#endif /* __PAGING_H__ */
