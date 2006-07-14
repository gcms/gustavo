#include <system.h>
#include <stdio.h>
#include <idt.h>

static void
clock_setup(int hz)
{
    short divisor = 1193180 / hz;

    outportb(0x43, 0x36);
    outportb(0x40, divisor);
    outportb(0x40, divisor >> 8);
}

#include <video.h>
static int
clock_handler(stack_frame_t *r)
{
    static int tick = 0;
    static int time = 0;

    tick = (tick + 1) % 100;

    if (tick == 0) {
        time++;
        cls();
        printf("%d\n", time);
    }

    return 0;
}

void
clock_init(void)
{
    irq_install_handler(0, clock_handler);
    clock_setup(100);   
}
