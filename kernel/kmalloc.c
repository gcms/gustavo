#include <assert.h>
#include <stdlib.h>
#include <paging.h>
#include <video.h>


static virt_addr_t start_brk = 0;
static virt_addr_t cur_brk = 0;


static void
check_brks(void)
{
    int n_pages;

    assert(cur_brk <= page_skbrk(0));

    n_pages = (page_skbrk(0) - cur_brk) / 4096;

    page_skbrk(-n_pages);    
}

void *
sbrk(int len)
{
    int n_pages;
    void *result;

    if (start_brk == 0) {
        start_brk = page_skbrk(0);
        cur_brk = start_brk;
    }

    /* SHOULD INDICATE AN ERROR */
    if (cur_brk + len < start_brk) {
        return NULL;
    }

    if (len < (int) (page_kbrk() - cur_brk)) {
        result = (void *) cur_brk;
        cur_brk += len;
        check_brks();
        return result;
    }

    assert(len > 0);

    n_pages = ALIGN(len , 4096) / 4096;

    result = (void *) page_skbrk(n_pages);
    if (result == NULL)
        return NULL;

    cur_brk += len;

    assert(cur_brk <= page_skbrk(0));

    return result;
}

#define MAGIC 0x12348765

#define RUN_SIZE(r)     (*((unsigned long *) (r)))
#define RUN_FREE(r)     (*((unsigned long *) ((unsigned long) (r) + 4)))
#define RUN_MAGIC(r)    (*((unsigned long *) ((unsigned long) (r) + 8)))
#define RUN_PREV(r)     \
    ((void *) *((unsigned long *) ((unsigned long) (r) + 12)))
#define RUN_DATA(r)     ((void *) ((unsigned long) (r) + 16))

#define RUN_HEADER_SIZE (4 + 4 + 4 + 4)

#define RUN_PREV_SET(r, p) \
    (*((unsigned long *) ((unsigned long) (r) + 12)) = (unsigned long) (p))

static void *run_list = NULL;
static void *top_run = NULL;

void *
kmalloc(size_t len)
{
    void *run;

    len = ALIGN(len, 4);

    for (run = run_list; run != NULL; run = RUN_PREV(run)) {
        if (RUN_FREE(run) && RUN_SIZE(run) >= len) {
            assert(RUN_MAGIC(run) == MAGIC);

            if (RUN_SIZE(run) >= 2 * len + RUN_HEADER_SIZE) {
                void *new_run;

                new_run = RUN_DATA(run)
                    + (RUN_SIZE(run) - len - RUN_HEADER_SIZE);
                RUN_SIZE(new_run) = len;
                RUN_FREE(new_run) = FALSE;
                RUN_MAGIC(new_run) = MAGIC;
                RUN_PREV_SET(new_run, RUN_PREV(run));

                RUN_SIZE(run) = (RUN_SIZE(run) - len - RUN_HEADER_SIZE);
                RUN_PREV_SET(run, new_run);

                if (new_run > top_run)
                    top_run = new_run;

                return RUN_DATA(new_run);
            } else {
                RUN_FREE(run) = FALSE;
                return RUN_DATA(run);
            }
        }
    }

    run = sbrk(RUN_HEADER_SIZE + len);
    if (run == NULL)
        return NULL;

    RUN_SIZE(run) = len;
    RUN_FREE(run) = FALSE;
    RUN_MAGIC(run) = MAGIC;
    RUN_PREV_SET(run, run_list);

    run_list = run;

    if (run > top_run)
        top_run = run;

    return RUN_DATA(run);
}

#define RUN_FROM_DATA(d)    ((void *) ((unsigned long) (d) - RUN_HEADER_SIZE))

/* TODO: provide a way to merge near runs */
void
kfree(void *data)
{
    void *run = RUN_FROM_DATA(data);

    if (RUN_MAGIC(run) != MAGIC) {
        printf("Invalid memory address!\n");
        assert(RUN_MAGIC(run) == MAGIC);
    }

    RUN_FREE(run) = TRUE;

    if (run == top_run && run == run_list) {
        run_list = RUN_PREV(run);
        top_run = NULL;

        assert(sbrk(0) == RUN_DATA(run) + RUN_SIZE(run));

        assert(sbrk(-RUN_SIZE(run) - RUN_HEADER_SIZE));
    }
}
