#ifndef __IDT_H__
#define __IDT_H__

typedef struct {
    unsigned long edi, esi, ebp, esp, ebx, edx, ecx, eax;
#define irq_num err_code
    unsigned long int_no, err_code;
    unsigned long eip, cs, eflags, useresp, ss;
} stack_frame_t;

typedef int (*idt_handler_t)(stack_frame_t *frame);


void
idt_init(void);

idt_handler_t
idt_install_handler(unsigned int int_no, idt_handler_t handler);

#define irq_install_handler(no, hdl) idt_install_handler(32 + no, hdl)


#endif /* __IDT_H__ */
