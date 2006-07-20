#include <system.h>
#include <paging.h>

#include <video.h>
#include <stdlib.h>
#include <assert.h>

/* The following strategy is used:
 *
 * First we obtain a list of frame runs, a frame run, is a sequence of
 * available memory aligned on a page boundary. So a frame run is a
 * sequence of available page frames. The page runs are obtained from
 * the multiboot_info_t memory map structure. We've got this structure
 * from the GRUB boot loader.
 *
 * The functions frame_free_list_init() frame_free_list_add() are used
 * for initializing the list of available frames.
 *
 * After we have a list of available frames, we "alloc" some for internal
 * data structures. For now I just alloc frames for storing a bitmap of
 * free frames. In this bitmap, a bit is set if the corresponding frame is
 * available to be allocated.
 *
 * The functions bitmap_init() and bitmap_set_free() are used to initalize
 * this bitmap. Two functions are also declared to help allocating these
 * page frames bitmap_lookup() and frame_alloc()
 *
 * Next step we set a page directory to be used for the kernel. A frame is
 * obtained to store the page directory using the frame_alloc() function.
 *
 * We start mapping the addresses of the kernel and addresses below 0x100000
 * to their correspondent frames. These frames won't be paged out during
 * the kernel execution.
 *
 * We also map the bitmap frames, so that when looking for free frames we don't
 * have to disable paging. The bitmap_free variable is then assigned to the
 * virtual address.
 *
 * After the mapping we're ready to jump into paging. We do this
 * by calling the set_page_dir() with the kernel page directory as a parameter
 * and then enable_paging();
 *
 * TODO:
 * After this we should alloc some other frames to make a higher level
 * allocation scheme. This scheme would assoc frames to the process using
 * them. The frames set as free in the bitmap in this moment will be
 * used for paging later. So the frames allocated until now will never be
 * paged out.
 */


#define PRESENT_MASK (1 << 0)
#define RW_MASK      (1 << 1)
#define US_MASK      (1 << 2)
#define PWT_MASK     (1 << 3)
#define PCD_MASK     (1 << 4)
#define ACCESS_MASK  (1 << 5)
#define PS_MASK      (1 << 6)
#define PAT_MASK     (1 << 6)
#define G_MASK       (1 << 7)


#define FRAME_RUN_SIZE(f)   (*((unsigned long *) f))
#define FRAME_RUN_NEXT(f)   (*((unsigned long *) (f + sizeof(unsigned long))))

/* Head of a list of the free frames. What we call free frames here
 * are the frames that will be allocated. That are not currently used by the
 * kernel and other reserved areas. This list store "runs" of frames. */
static page_frame_t frame_free_list;

/* Store the memory size we need to represent, in bytes. Will be used to
 * find out the space needed to management data structures. */
static unsigned long memory_size;

/* Number of frames that the physical memory can be divided into.
 * memory_num_frame = memory_size / 4096; */
static unsigned long memory_num_frame;

/* Physical address of a bitmap that determines if a frame is free or used. */
static phys_addr_t bitmap_free;

/* Size of the bitmaps in bytes.
 * bitmap_size = ALIGN(memory_num_frame, 8) / 8; */
static unsigned long bitmap_size;

/* Size of the bitmaps in frames.
 * bitmap_num_frame = ALIGN(bitmap_size, 4096) / 4096; */
static unsigned long bitmap_num_frame;


/* Loads the CR3 register with the page_dir address */
extern void set_page_dir(page_dir_t page_dir);

/* Set/Reset bit 31 (PG) of CR0 register */
extern void enable_paging(void);
extern void disable_paging(void);

/* Is paging enabled? */
extern bool pg_enabled(void);

/* Number of currently free frames. */
static unsigned long frame_free_num;

/* Index in the page directory that will be used to store the page map.
 * What I call a page map is a region of the address space that will be used
 * to store the pagetables. The page directory has a total of 1024 entries,
 * each entry of 32-bits, so that the total size of the page directory is
 * 4096 bytes. If all the 1024 entries are used we need a total of 1024
 * page tables. The page tables have also 1024 entries and need 4096 bytes
 * to be stored. So each index in the page directory is capable of representing
 * an address space of 4 MB, which is exactly the same we need to store all our
 * page tables. So the page tables are mapped into the address space of
 * an entry in the page directory. */
static unsigned int page_map_idx;


#define MBI multiboot_info

/* Alloc frames to be used in internal data structures. */
static page_frame_t
frame_alloc_internal(size_t num_frame)
{
    page_frame_t *f, result;

    for (f = &frame_free_list; *f != 0; f = &FRAME_RUN_NEXT(f)) {
        if (FRAME_RUN_SIZE(*f) > num_frame) {
            result = *f;
            *f = *f + num_frame * 4096;
            FRAME_RUN_NEXT(*f) = FRAME_RUN_NEXT(result);
            FRAME_RUN_SIZE(*f) = FRAME_RUN_SIZE(result) - num_frame;

            return result;
        }
    }

    return 0;
}

/* Add a run of frames to the frame free list. */
static void
frame_free_list_add(unsigned long start, unsigned long end)
{
    page_frame_t new_frame_run;
    unsigned long n_frames;

    start = ALIGN(start, 4096);
    n_frames = (end + 1 - start) / 4096;

    if (n_frames <= 0)
        return;

    new_frame_run = (page_frame_t) start;
    FRAME_RUN_SIZE(new_frame_run) = n_frames;
    FRAME_RUN_NEXT(new_frame_run) = frame_free_list;

    frame_free_list = new_frame_run;

    if (end + 1 > memory_size) {
        memory_size = end + 1;
    }

    printf("frame_free_add(0x%x, 0x%x)\n", start, end);
}

/* Initializes list of free frames. */
static void
frame_free_list_init(void)
{
    memory_map_t *mmap;

    /* We haven a memory map ? */
    assert(CHECK_FLAG(MBI.flags, 6));

    frame_free_list = 0;
    
    memory_size = 0;

    for (mmap = (memory_map_t *) MBI.mmap_addr;
            (unsigned long) mmap < MBI.mmap_addr + MBI.mmap_length;
            mmap = (memory_map_t *) ((unsigned long) mmap
                + mmap->size + sizeof(mmap->size))) {
        bool sliced;
        unsigned long start, end;

        assert(mmap->base_addr_high == 0);
        assert(mmap->length_high == 0);

        sliced = FALSE;
        start = mmap->base_addr_low;
        end = mmap->base_addr_low + mmap->length_low - 1;

        if (start < 0x100000) {
            if (end > 0x100000) {
                start = 0x100000;
            } else {
                continue;
            }
        }

        if (start < KSTART && KSTART <= end) {
            frame_free_list_add(start, KSTART - 1);
            sliced = TRUE;
        }
        if (start <= KEND && KEND < end) {
            frame_free_list_add(KEND + 1, end);
            sliced = TRUE;
        }

        if (!sliced) {
            frame_free_list_add(start, end);
        }
    }
}

static bool
bitmap_is_free(page_frame_t address)
{
    unsigned char *bitpos;

    assert((address & 0xFFFFF000) == address);
    
    bitpos = (unsigned char *) (bitmap_free + (address >> 12) / 8);

    return *bitpos & (1 << (address % 8));
}

/* mark a page as free or not in the bitmap */
static void
bitmap_set_free(page_frame_t address, bool free)
{
    unsigned char *bitpos;

    assert((address & 0xFFFFF000) == address);

    address = (address >> 12) & 0xFFFFF;

    bitpos = (unsigned char *) (bitmap_free + address / 8);

    if (free) {
        if (!(*bitpos & (1 << (address % 8))))
            frame_free_num++;
        *bitpos |= (1 << (address % 8));
    } else {
        if (*bitpos & (1 << (address % 8)))
            frame_free_num--;
        *bitpos &= ~(1 << (address % 8));
    }
}


page_frame_t
bitmap_lookup(void)
{
    phys_addr_t i;
    unsigned int j;

    for (i = 0; i < bitmap_size; i++) {
        unsigned char *val = (unsigned char *) (bitmap_free + i);
        for (j = 0; j < 8; j++) {
            if ((*val & (1 << j))) {
                return (page_frame_t) ((i * 8 + j) * 4096);
            }
        }
    }

    return (page_frame_t) 0;
}
static page_frame_t
frame_alloc(void)
{
    page_frame_t result;

    result = bitmap_lookup();

    if (result)
        bitmap_set_free(result, FALSE);

    return result;
}

static void
frame_free(page_frame_t frame)
{
    assert((frame & 0xFFFFF000) == frame);

    bitmap_set_free(frame, TRUE);
}

static void
bitmap_init(void)
{
    page_frame_t f, g;

    /* Number of frames the total memory can be divided into */
    memory_num_frame = memory_size / 4096;

    /* Number of bytes needed to to store a bitmap for the number of pages */
    bitmap_size = ALIGN(memory_num_frame, 8) / 8;
    bitmap_num_frame = ALIGN(bitmap_size, 4096) / 4096;

    bitmap_free = frame_alloc_internal(bitmap_num_frame);

    assert(bitmap_free);

    memset((void *) bitmap_free, 0, bitmap_num_frame * 4096);

    for (f = frame_free_list; f != 0; f = FRAME_RUN_NEXT(f)) {
        for (g = f; g < f + FRAME_RUN_SIZE(f) * 4096; g += 4096) {
            bitmap_set_free(g, TRUE);
        }
    }
}


#define kpage_dir ((page_dir_t) PAGE_MAP_DIR_ADDR(page_map_idx, page_map_idx))

#define PAGE_DIR_ENTRY_ADDR(e)   \
    ((page_tbl_entry_t *) (((phys_addr_t) e) & 0xFFFFF000))

#define IS_PAGE_ADDR(p) ((((phys_addr_t) p) & 0xFFFFF000) == ((phys_addr_t) p))


#define PAGE_MAP_DIR_ADDR(m, d) ((m << 22) + (d << 12))

static void
page_frame_map(page_dir_t page_dir, unsigned long map_idx,
        page_t page, page_frame_t frame)
{
    unsigned int dir_idx;
    page_tbl_t page_tbl;

    assert(pg_enabled() ? map_idx : !map_idx);

    assert(IS_PAGE_ADDR(page) && IS_PAGE_ADDR(frame));
    assert(!bitmap_is_free(frame));


    assert(!map_idx || (page_dir[map_idx] & PRESENT_MASK));
    assert(!map_idx || (kpage_dir == page_dir));

    dir_idx = (page >> 22) & 0x3FF;

    if (!(page_dir[dir_idx] & PRESENT_MASK)) {
        page_frame_t new_frame = frame_alloc();
        assert(new_frame);
        assert(!bitmap_is_free(new_frame));

        if (map_idx) {
            printf("map_idx = %d\tdir_idx = %d\n", map_idx, dir_idx);
            page_dir[dir_idx] = new_frame | PRESENT_MASK | RW_MASK;
            page_frame_map(page_dir, map_idx,
                    PAGE_MAP_DIR_ADDR(map_idx, dir_idx),
                    new_frame);
            printf("BEFORE MEMSET!");
            memset((void *) PAGE_MAP_DIR_ADDR(map_idx, dir_idx), 0, 4096);
            printf("AFTER MEMSET!");
        } else {
            memset((void *) new_frame, 0, 4096);
            page_dir[dir_idx] = new_frame | PRESENT_MASK | RW_MASK;
        }
    }

    assert(page_dir[dir_idx] & PRESENT_MASK);


    if (map_idx) {
        page_tbl = (page_tbl_t) PAGE_MAP_DIR_ADDR(map_idx, dir_idx);
    } else {
        page_tbl = PAGE_DIR_ENTRY_ADDR(page_dir[dir_idx]);
    }
    
    page_tbl[(page >> 12) & 0x3FF] = frame | PRESENT_MASK | RW_MASK;
}

static void
page_set_flags(page_dir_t page_dir, page_t page, unsigned long flags)
{
    page_dir_entry_t *dir_entry;
    page_tbl_entry_t *tbl_entry;

    assert((page & 0xFFFFF000) == page);

    dir_entry = page_dir + ((page >> 22) & 0x3FF);
    tbl_entry = PAGE_DIR_ENTRY_ADDR(*dir_entry) + ((page >> 12) & 0x3FF);

    assert(*tbl_entry & PRESENT_MASK);
    *tbl_entry |= (flags & 0xFFF);
}

static phys_addr_t kbrk;

static phys_addr_t
get_kbrk(void)
{
    return kbrk;
}

static phys_addr_t
next_kbrk(void)
{
    phys_addr_t result = kbrk;

    kbrk += 4096;

    return result;
}


static void
map_pages(void)
{
    phys_addr_t addr;
    phys_addr_t new_bitmap;
    
    unsigned int i;
    page_dir_t kpage_dir_phys;

    kpage_dir_phys = (page_dir_t) frame_alloc();
    printf("kpage_dir_phys = 0x%x\n", kpage_dir_phys);

    assert(kpage_dir_phys);

    memset((void *) kpage_dir_phys, 0, 4096);

    /* Map the kernel */
    for (addr = 0; addr < ALIGN(KEND, 4096); addr += 4096) {
        page_frame_map(kpage_dir_phys, 0, addr, addr);
    }

    /* Disable cache on address below 0x100000 */
    for (addr = 0; addr < ALIGN(KSTART, 4096); addr += 4096) {
        page_set_flags(kpage_dir_phys, addr, PCD_MASK);
    }

    kbrk = ALIGN(KEND, 4096);
    printf("kbrk = 0x%x\n", kbrk);

    /* Map bitmaps */
    new_bitmap = kbrk;
    for (addr = bitmap_free; addr < bitmap_free + bitmap_num_frame * 4096;
            addr += 4096) {
        page_frame_map(kpage_dir_phys, 0, next_kbrk(), addr);
    }

    for (i = 0; i < 1024; i++) {
        if (!(kpage_dir_phys[i] & PRESENT_MASK))
            break;
    }

    page_map_idx = i;
    assert(page_map_idx < 1024);

    kpage_dir_phys[page_map_idx] = (page_dir_entry_t) kpage_dir_phys;
    kpage_dir_phys[page_map_idx] |= PRESENT_MASK | RW_MASK;

    for (i = 0; i < 1024; i++) {
        if (kpage_dir_phys[i] & PRESENT_MASK) {
            printf("associng 0x%x to 0x%x\n",
                    PAGE_MAP_DIR_ADDR(page_map_idx, i),
                    PAGE_DIR_ENTRY_ADDR(kpage_dir_phys[i]));
            page_frame_map(kpage_dir_phys, 0,
                    (page_t) PAGE_MAP_DIR_ADDR(page_map_idx, i),
                    (page_frame_t) PAGE_DIR_ENTRY_ADDR(kpage_dir_phys[i]));
        }
    }


    assert(((page_map_idx + 1) << 22) >= kbrk);
    kbrk = (page_map_idx + 1) << 22;


    bitmap_free = new_bitmap;

    /* Set the page dir */
    set_page_dir(kpage_dir_phys);
}

static page_frame_t
get_page_frame(page_dir_t page_dir, page_t page)
{
    page_dir_entry_t *dir_entry;
    page_tbl_entry_t *tbl_entry;

    assert((page & 0xFFFFF000) == page);

    dir_entry = page_dir + ((page >> 22) & 0x3FF);

    if (!(*dir_entry & PRESENT_MASK))
        return 0;

    tbl_entry = PAGE_DIR_ENTRY_ADDR(*dir_entry) + ((page >> 12) & 0x3FF);
    if (!(*tbl_entry & PRESENT_MASK))
        return 0;

    return (page_frame_t) PAGE_DIR_ENTRY_ADDR(*tbl_entry);
}

static void
page_frame_umap(page_dir_t page_dir, unsigned long map_idx,
       page_t page)
{
    unsigned int dir_idx;
    page_tbl_entry_t *page_tbl;

    assert(IS_PAGE_ADDR(page));
    assert(pg_enabled() ? map_idx : !map_idx);

    dir_idx = (page >> 22) & 0x3FF;

    if (map_idx) {
        page_tbl = (page_tbl_entry_t *) PAGE_MAP_DIR_ADDR(map_idx, dir_idx);
    } else {
        page_tbl = PAGE_DIR_ENTRY_ADDR(page_dir[dir_idx]);
    }

    page_tbl += (page >> 12) & 0x3FF;

    *page_tbl &= ~PRESENT_MASK;

    bitmap_set_free((page_frame_t) PAGE_DIR_ENTRY_ADDR(*page_tbl), TRUE);
}

static phys_addr_t
page_neg_sbrk(int n_pages)
{
    phys_addr_t old_kbrk = kbrk;

    if (get_kbrk() + n_pages * 4096 < ALIGN(KEND, 4096)) {
        n_pages = (ALIGN(KEND, 4096) - get_kbrk()) / 4096;
    }

    printf("n_pages = %d\n", n_pages);

    while (n_pages++ < 0) {
        kbrk -= 4096;
        assert(kbrk >= ALIGN(KEND, 4096));

        page_frame_umap(kpage_dir, page_map_idx, kbrk);
    }

    return old_kbrk;
}

static phys_addr_t
page_sbrk(int n_pages)
{
    page_frame_t frame;
    phys_addr_t old_kbrk = kbrk;

    if (n_pages <= 0) {
        return page_neg_sbrk(n_pages);
    }

    if (frame_free_num < n_pages)
        return 0;



    while (n_pages-- > 0) {
        frame = frame_alloc();
        assert(frame);

        printf("calling page_frame_map()\n");
        printf("%d 0x%x\n", 0, kpage_dir[0]);
        printf("%d 0x%x\n", 1, kpage_dir[1]);
        printf("%d 0x%x\n", 2, kpage_dir[2]);
        printf("0x%x\n", PAGE_DIR_ENTRY_ADDR(kpage_dir[page_map_idx]));
        printf("0x%x\n", kpage_dir);

        printf("pg_enabled? %s\n", pg_enabled() ? "YES" : "NO");
        printf("page_map_idx = %d\n", page_map_idx);
        page_frame_map(kpage_dir, page_map_idx, next_kbrk(), frame);
    }
    

    return old_kbrk;
}

#include <idt.h>

extern phys_addr_t get_cr2(void);

static int
page_fault_handler(stack_frame_t *frame)
{
    phys_addr_t cr2 = get_cr2();

    printf("PAGE FAULT: 0x%x 0x%x\n", frame->code, cr2);

    halt();

    return 0;
}

void
paging_init(void)
{
    unsigned long *data;

    printf("KSTART = 0x%x, KEND = 0x%x\n", KSTART, KEND);

    frame_free_list_init();
    printf("frame_free_num = %d\n", frame_free_num);

    bitmap_init();
    printf("bitmap initialized\n");

    map_pages();

    printf("kpage_dir = 0x%x\n", kpage_dir);
    printf("asdfadsff = 0x%x\n", (1 << 22) + (1 << 12));

    idt_install_handler(14, page_fault_handler);

    enable_paging();
    
    printf("frame_free_num = %d\n", frame_free_num);
    data = (unsigned long *) page_sbrk(1);
    printf("0x%x\n", data);
    *data = 10;
    puts("SBRKED/ACCESSED");

    printf("start 0x%x\tend 0x%x\n",
            (page_map_idx << 22), (page_map_idx + 1) << 22);
    printf("frame_free_num = %d\tbrk = 0x%x\n", frame_free_num, kbrk);
    page_sbrk(-1);
    printf("frame_free_num = %d\tbrk = 0x%x\n", frame_free_num, kbrk);

    printf("\nbyte %d\tbit %d\n",
            (frame_free_list >> 12) / 8, (frame_free_list >> 12) % 8);

    printf("frame_free_list = 0x%x\n", frame_free_list);
    frame_free(frame_alloc());

    printf("frame_alloc() = 0x%x\n", frame_alloc());
    printf("frame_alloc() = 0x%x\n", frame_alloc());

    printf("bitmap_num_frame = %d\n", bitmap_num_frame);
    printf("frame_free_num = %d\n", frame_free_num);

    printf("bitmap_free[0] = 0x%x\n", ((unsigned char *) bitmap_free)[0x300 / 8]);
    printf("bitmap_free[0] = 0x%x\n", ((unsigned char *) bitmap_free)[0x300 / 8]);

}
