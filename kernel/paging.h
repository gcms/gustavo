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

virt_addr_t page_kbrk(void);
virt_addr_t page_skbrk(int n_pages);

void
paging_init(void);


#endif /* __PAGING_H__ */
