#include <system.h>
#include <stdio.h>
#include <idt.h>
#include <assert.h>
#include <clock.h>

static void
clock_setup(int hz)
{
    short divisor = 1193180 / hz;

    outportb(0x43, 0x36);
    outportb(0x40, divisor);
    outportb(0x40, divisor >> 8);
}


static unsigned int tick = 0;
static unsigned int time = 0;

unsigned int
clock_get_time(void)
{
    return time;
}

#include <video.h>
static int
clock_handler(stack_frame_t *r)
{
    assert(r->irq_num == 0);

    tick++;

    if ((tick % 100) == 0) {
        time++;
    }

    return 0;
}

void
clock_wait(unsigned int wait_ticks)
{
    wait_ticks += tick;

    while (tick < wait_ticks);
}

void
clock_init(void)
{
    irq_install_handler(0, clock_handler);
    clock_setup(TICK_FREQ);
}
