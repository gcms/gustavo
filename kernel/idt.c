#include <system.h>
#include <idt.h>
#include <stdlib.h>
#include <const.h>

/* IDT table, declared in assembly code, allocated on BSS */
extern int idt;

/* Loads the IDT */
extern void idt_load();

/* Defined in the end of this file, load processor exception handler stubs */
static void isr_init(void);
/* Defined in the end of this file, load IRQ interrupt handler stubs */
static void irq_init(void);

static idt_handler_t
idt_handler_table[IDT_ENTRY_NUM];

static void
idt_entry_set(unsigned char vector,
        unsigned long offset, unsigned short selector, unsigned char access)
{
    unsigned short *entry = (unsigned short *) ((unsigned long) &idt + vector * 8);

    entry[0] = offset;
    entry[1] = selector;
    entry[2] = (access << 8) & 0xFF00;
    entry[3] = offset >> 16;
}

/* Installs the IDT */
void idt_init(void)
{
    /* Clear out the entire IDT, initializing it to zeros */
    memset(&idt, 0, 8 * IDT_ENTRY_NUM);

    memset(idt_handler_table, 0, sizeof(idt_handler_t) * IDT_ENTRY_NUM);

    /* Add any new ISRs to the IDT here using idt_entry_set */

    isr_init();

    irq_init();

    /* Points the processor's internal register to the new IDT */
    idt_load();
}

idt_handler_t
idt_install_handler(unsigned int int_no, idt_handler_t handler)
{
    idt_handler_t result;

    if (int_no < 0 || int_no >= IDT_ENTRY_NUM) {
        return NULL;
    }

    result = idt_handler_table[int_no];

    idt_handler_table[int_no] = handler;

    return result;    
}

/* ISR stubs, defined on assembly code */
extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);

static void
isr_init(void)
{
	idt_entry_set(0, (unsigned long) isr0, 0x8, 0x8E);
	idt_entry_set(1, (unsigned long) isr1, 0x8, 0x8E);
	idt_entry_set(2, (unsigned long) isr2, 0x8, 0x8E);
	idt_entry_set(3, (unsigned long) isr3, 0x8, 0x8E);
	idt_entry_set(4, (unsigned long) isr4, 0x8, 0x8E);
	idt_entry_set(5, (unsigned long) isr5, 0x8, 0x8E);
	idt_entry_set(6, (unsigned long) isr6, 0x8, 0x8E);
	idt_entry_set(7, (unsigned long) isr7, 0x8, 0x8E);
	idt_entry_set(8, (unsigned long) isr8, 0x8, 0x8E);
	idt_entry_set(9, (unsigned long) isr9, 0x8, 0x8E);
	idt_entry_set(10, (unsigned long) isr10, 0x8, 0x8E);
	idt_entry_set(11, (unsigned long) isr11, 0x8, 0x8E);
	idt_entry_set(12, (unsigned long) isr12, 0x8, 0x8E);
	idt_entry_set(13, (unsigned long) isr13, 0x8, 0x8E);
	idt_entry_set(14, (unsigned long) isr14, 0x8, 0x8E);
	idt_entry_set(15, (unsigned long) isr14, 0x8, 0x8E);
	idt_entry_set(16, (unsigned long) isr16, 0x8, 0x8E);
	idt_entry_set(17, (unsigned long) isr17, 0x8, 0x8E);
	idt_entry_set(18, (unsigned long) isr18, 0x8, 0x8E);
	idt_entry_set(19, (unsigned long) isr18, 0x8, 0x8E);
	idt_entry_set(20, (unsigned long) isr18, 0x8, 0x8E);
	idt_entry_set(21, (unsigned long) isr18, 0x8, 0x8E);
	idt_entry_set(22, (unsigned long) isr18, 0x8, 0x8E);
	idt_entry_set(23, (unsigned long) isr18, 0x8, 0x8E);
	idt_entry_set(24, (unsigned long) isr18, 0x8, 0x8E);
	idt_entry_set(25, (unsigned long) isr18, 0x8, 0x8E);
	idt_entry_set(26, (unsigned long) isr18, 0x8, 0x8E);
	idt_entry_set(27, (unsigned long) isr18, 0x8, 0x8E);
	idt_entry_set(28, (unsigned long) isr18, 0x8, 0x8E);
	idt_entry_set(29, (unsigned long) isr18, 0x8, 0x8E);
	idt_entry_set(30, (unsigned long) isr18, 0x8, 0x8E);
	idt_entry_set(31, (unsigned long) isr18, 0x8, 0x8E);
}


/* IRQ Handler stubs, defined on assembly code */
extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);


/* Initializes the PIC by remapping the IRQ numbers */
extern void pic_init(void);

static void
irq_init(void)
{
	idt_entry_set(32, (unsigned long) irq0, 0x8, 0x8E);
	idt_entry_set(33, (unsigned long) irq1, 0x8, 0x8E);
	idt_entry_set(34, (unsigned long) irq2, 0x8, 0x8E);
	idt_entry_set(35, (unsigned long) irq3, 0x8, 0x8E);
	idt_entry_set(36, (unsigned long) irq4, 0x8, 0x8E);
	idt_entry_set(37, (unsigned long) irq5, 0x8, 0x8E);
	idt_entry_set(38, (unsigned long) irq6, 0x8, 0x8E);
	idt_entry_set(39, (unsigned long) irq7, 0x8, 0x8E);
	idt_entry_set(40, (unsigned long) irq8, 0x8, 0x8E);
	idt_entry_set(41, (unsigned long) irq9, 0x8, 0x8E);
	idt_entry_set(42, (unsigned long) irq10, 0x8, 0x8E);
	idt_entry_set(43, (unsigned long) irq11, 0x8, 0x8E);
	idt_entry_set(44, (unsigned long) irq12, 0x8, 0x8E);
	idt_entry_set(45, (unsigned long) irq13, 0x8, 0x8E);
	idt_entry_set(46, (unsigned long) irq14, 0x8, 0x8E);
	idt_entry_set(47, (unsigned long) irq15, 0x8, 0x8E);

    pic_init();
}

static unsigned char *exception_messages[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

#include <video.h>
void
fault_handler(stack_frame_t *frame)
{
    idt_handler_t handler;

    /*
    printf("INT_NO = %d\tERR_CODE = %d\n",
            frame->int_no, frame->code);
            */

    handler = idt_handler_table[frame->int_no];

    if (handler) {
        handler(frame);
    } else if (frame->int_no < 32) {
        puts(exception_messages[frame->int_no]);
        puts(" Exception. System Halted!\n");
        halt();
    }
}
