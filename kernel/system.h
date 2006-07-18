#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <multiboot.h>

extern multiboot_info_t multiboot_info;

extern unsigned long kstart, kend;

#define KSTART  ((unsigned long) &kstart)
#define KEND    ((unsigned long) &kend)

void halt(void);

#endif /* __SYSTEM_H__ */
