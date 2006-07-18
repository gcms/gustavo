#ifndef __PAGING_H__
#define __PAGING_H__

typedef unsigned long phys_addr_t;

typedef unsigned long page_dir_entry_t;
typedef page_dir_entry_t *page_dir_t;
typedef unsigned long page_tbl_entry_t;
typedef page_tbl_entry_t *page_tbl_t;

typedef phys_addr_t page_t;
typedef phys_addr_t page_frame_t;

void
paging_init(void);


#endif /* __PAGING_H__ */
