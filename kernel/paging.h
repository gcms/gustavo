#ifndef __PAGING_H__
#define __PAGING_H__

typedef unsigned long phys_addr_t;

typedef unsigned long pagedir_entry_t;
typedef unsigned long pagetbl_entry_t;

typedef phys_addr_t page_frame_t;

void
paging_init(void);


#endif /* __PAGING_H__ */
