#ifndef __CLOCK_H__
#define __CLOCK_H__

void
clock_init(void);

unsigned int
clock_get_time(void);

void
clock_wait(unsigned int ticks);

#endif /* __CLOCK_H__ */
