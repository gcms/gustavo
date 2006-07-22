#ifndef __CLOCK_H__
#define __CLOCK_H__

/* tick frequency in hertz */
#define TICK_FREQ   100

void
clock_init(void);

unsigned int
clock_get_time(void);

void
clock_wait(unsigned int ticks);

#endif /* __CLOCK_H__ */
