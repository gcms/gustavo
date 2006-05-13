#ifndef SYSTEM_H
#define SYSTEM_H

typedef unsigned int size_t;

unsigned char *memcpy(void *dest, const void *src, int count);
unsigned char *memset(void *dest, int val, int count);
unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);
size_t strlen(const char *str);
unsigned char inportb (unsigned short _port);
void outportb (unsigned short _port, unsigned char _data);

void cls(void);
void putch(unsigned char);
void puts(unsigned char *);
void settextcolor(unsigned char, unsigned char);
void init_video(void);

void gdt_install(void);

void idt_install(void);
void idt_set_gate(unsigned char num, unsigned long base,
        unsigned short sel, unsigned char flags);

/* This defines what the stack looks like after an ISR was running */
struct regs
{
    unsigned int ds, es, fs, gs;
    /* pushed the segs last */

    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; 
    /* pushed by 'pusha' */

    unsigned int int_no, err_code;
    /* our 'push byte #' and ecodes do this */

    unsigned int eip, cs, eflags, useresp, ss;
    /* pushed by the processor automatically */ 
};

void isrs_install(void);
void fault_handler(struct regs *);

void irq_install(void);
void irq_install_handler(int irq, void (*handler)(struct regs *r));
void irq_uninstall_handler(int irq);

void timer_install(void);

void kb_install(void);

#endif /* SYSTEM_H */
