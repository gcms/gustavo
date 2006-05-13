#include <system.h>

void timer_phase(int tempo) {
    outportb(0x43, 0x36);
    outportb(0x40, 1193181 & 0xFF);
    outportb(0x40, (1193181 >> 8) & 0xFF);
}

/* This will keep track of how many ticks that the system
*  has been running for */
int timer_ticks = 0;

/* Handles the timer. In this case, it's very simple: We
*  increment the 'timer_ticks' variable every time the
*  timer fires. By default, the timer fires 1193181.222 times
*  per second. Why 1193181.222Hz? Some engineer at IBM must've
*  been smoking something funky */
void timer_handler(struct regs *r)
{
    /* Increment our 'tick count' */
    timer_ticks++;

    /* Every 1193181 clocks (approximately 1 second), we will
    *  display a message on the screen */
    /*
    if (timer_ticks % 100 == 0)
    {
        puts("One second has passed\n");
    }
    */
}

void timer_wait(int ticks) {
    unsigned long eticks;
    eticks = timer_ticks + ticks;
    while (timer_ticks < eticks);
}

/* Sets up the system clock by installing the timer handler
*  into IRQ0 */
void timer_install(void)
{
    irq_install_handler(0, timer_handler);
    timer_phase(100);
    /* Installs 'timer_handler' to IRQ0 */
}
	
