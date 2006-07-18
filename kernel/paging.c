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

static phys_addr_t valid_bitmap;

/* Size of the bitmaps in bytes.
 * bitmap_size = ALIGN(memory_num_frame, 8) / 8; */
static unsigned long bitmap_size;

/* Size of the bitmaps in frames.
 * bitmap_num_frame = ALIGN(bitmap_size, 4096) / 4096; */
static unsigned long bitmap_num_frame;


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

    valid_bitmap = frame_alloc_internal(bitmap_num_frame);
    bitmap_free = frame_alloc_internal(bitmap_num_frame);

    assert(valid_bitmap);
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


void
paging_init(void)
{
    unsigned int i, j;
    page_frame_t f;

    printf("KSTART = 0x%x, KEND = 0x%x\n", KSTART, KEND);

    frame_free_list_init();

    bitmap_init();

    for (f = frame_free_list; f != 0; f = FRAME_RUN_NEXT(f)) {
        printf("FRAME_RUN start = 0x%x,\tnum_pages = %d\n",
                f, FRAME_RUN_SIZE(f));
    }

    printf("0x%x\n", memory_size);
    printf("0x%x\n", 0x10c000 + 32500 * 4096);

    printf("\n\n");
    for (i = bitmap_free; i < bitmap_free + ALIGN(memory_num_frame, 8) / 8; i++) {
        printf("%d\t", i - bitmap_free);
        unsigned char *val = (unsigned char *) i;
        for (j = 0; j < 8; j++) {
            printf("%d", (*val & (1 << j)) ? 1 : 0);
        }

        if (i - bitmap_free == 0x21)
            break;
        printf("\n");
    }

    printf("\nbyte %d\tbit %d\n",
            (frame_free_list >> 12) / 8, (frame_free_list >> 12) % 8);

    printf("frame_free_list = 0x%x\n", frame_free_list);
    frame_free(frame_alloc());
    printf("frame_alloc() = 0x%x\n", frame_alloc());
    printf("frame_alloc() = 0x%x\n", frame_alloc());
}
