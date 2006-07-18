#include <system.h>
#include <paging.h>

#include <video.h>
#include <stdlib.h>
#include <assert.h>


#define PAGEDIR_ENTRIES 1024

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


extern void set_page_dir(page_dir_t page_dir);
extern void enable_paging(void);


#define MBI multiboot_info

/* Alloc frames to be used in internal data structures. */
static page_frame_t
frame_alloc_internal(size_t num_frame)
{
    page_frame_t *f, result;

    printf("frame_alloc_internal(%d)\n", num_frame);

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

/* mark a page as free or not in the bitmap */
static void
bitmap_set_free(page_frame_t address, bool free)
{
    unsigned char *bitpos;

    assert((address & 0xFFFFF000) == address);

    address = (address >> 12) & 0xFFFFF;

    bitpos = (unsigned char *) (bitmap_free + address / 8);

    if (free) {
        *bitpos |= (1 << (address % 8));
    } else {
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


static page_dir_t kpage_dir;
#define PAGE_DIR_ENTRY_ADDR(e)   ((page_tbl_entry_t *) (e & 0xFFFFF000))
#define PAGE_TBL_ENTRY_ADDR(e)   ((page_t) (e & 0xFFFFF000))

static page_dir_entry_t *
get_dir_entry(page_dir_t page_dir, unsigned int entry_idx)
{
    page_dir_entry_t *entry;

    assert(entry_idx >= 0 && entry_idx < 1024);

    entry = page_dir + entry_idx;

    if (!(*entry & PRESENT_MASK)) {
        *entry = frame_alloc();
        printf("allocating entry 0x%x\n", *entry);

        assert((*entry & 0xFFFFF000) == *entry);

        memset((void *) *entry, 0, 4096);

        *entry |= (PRESENT_MASK | RW_MASK);
    }


    return entry;
}

static page_tbl_entry_t *
get_tbl_entry(page_tbl_t page_tbl, unsigned int entry_idx)
{
    page_tbl_entry_t *entry;

    assert(entry_idx >= 0 && entry_idx < 1024);

    entry = page_tbl + entry_idx;

    if (!(*entry & PRESENT_MASK)) {
        *entry = frame_alloc();

        assert((*entry & 0xFFFFF000) == *entry);

        memset((void *) *entry, 0, 4096);

        *entry |= (PRESENT_MASK | RW_MASK);
    }


    return entry;
}

static void
page_frame_map(page_dir_t page_dir,
        page_t page, page_frame_t frame)
{
    page_dir_entry_t *dir_entry;
    page_tbl_entry_t *tbl_entry;

    assert((page & 0xFFFFF000) == page);
    assert((frame & 0xFFFFF000) == frame);

    dir_entry = get_dir_entry(page_dir, ((page >> 22) & 0x3FF));
    tbl_entry = PAGE_DIR_ENTRY_ADDR(*dir_entry) + ((page >> 12) & 0x3FF);

    *tbl_entry |= (frame & 0xFFFFF000) | PRESENT_MASK | RW_MASK;
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

static void
map_pages(void)
{
    phys_addr_t addr;

    kpage_dir = (page_dir_t) frame_alloc();

    assert(kpage_dir);

    memset((void *) kpage_dir, 0, 4096);

    for (addr = 0; addr < ALIGN(KEND, 4096); addr += 4096) {
        page_frame_map(kpage_dir, addr, addr);
    }

    for (addr = 0; addr < ALIGN(KSTART, 4096); addr += 4096) {
        page_set_flags(kpage_dir, addr, PCD_MASK);
    }

    set_page_dir(kpage_dir);
    enable_paging();
}

void
paging_init(void)
{
    printf("KSTART = 0x%x, KEND = 0x%x\n", KSTART, KEND);

    frame_free_list_init();

    bitmap_init();

    map_pages();
/*
    printf("\nbyte %d\tbit %d\n",
            (frame_free_list >> 12) / 8, (frame_free_list >> 12) % 8);

    printf("frame_free_list = 0x%x\n", frame_free_list);
    frame_free(frame_alloc());
    printf("frame_alloc() = 0x%x\n", frame_alloc());
    printf("frame_alloc() = 0x%x\n", frame_alloc());

    printf("bitmap_num_frame = %d\n", bitmap_num_frame);
    */
}
