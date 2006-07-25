#include <const.h>

.globl gdt_init

gdt_init:
    lgdt    gdtr

    mov     $0x10, %ax  /* second gdt descriptor (2 << 3) */
    mov     %ax, %ds
    mov     %ax, %es
    mov     %ax, %fs
    mov     %ax, %gs
    mov     %ax, %ss
    jmp     $0x8, $here /* first gdt descriptor (1 << 3) */
here:
    ret

gdtr:
.word (gdt_end - gdt - 1)
.long gdt


/* 
 * "The base address of the GDT should be aligned on an eight-byte boundary to
 * yield the best processor performance."
 * Intel Architecture Software Developer's Manual, Volume 3  System Programming
 */
.align 8
gdt:
gdt_null:
.long 0, 0
gdt_code:
.word ~0, 0
.byte 0, 0x9A, 0xCF, 0
gdt_data:
.word ~0, 0
.byte 0, 0x92, 0xCF, 0
gdt_end:


.equiv IDT_SIZE, (8 * IDT_ENTRY_NUM)

idtr:
.word IDT_SIZE
idtr_base:
.long idt

/* As the GDT this one should also be aligned on a 8 byte boundary */
.bss
.global idt
.align 8
idt:
.space IDT_SIZE

.text
.global idt_load
idt_load:
    lidt idtr
    sti
    ret


.globl isr0
isr0:
    pushl   $0
    
    pushl   $0
    jmp isr_stub


.globl isr1
isr1:
    pushl   $0

    pushl   $1
    jmp isr_stub


.globl isr2
isr2:
    pushl   $0

    pushl   $2
    jmp isr_stub


.globl isr3
isr3:
    pushl   $0

    pushl   $3
    jmp isr_stub


.globl isr4
isr4:
    pushl   $0

    pushl   $4
    jmp isr_stub


.globl isr5
isr5:
    pushl   $0

    pushl   $5
    jmp isr_stub


.globl isr6
isr6:
    pushl   $0

    pushl   $6
    jmp isr_stub


.globl isr7
isr7:
    pushl   $0

    pushl   $7
    jmp isr_stub


.globl isr8
isr8:
    pushl   $8
    jmp isr_stub


.globl isr9     /* Is it really needed? remove to test later. */
isr9:
    pushl   $0

    pushl   $9
    jmp isr_stub


.globl isr10
isr10:
    pushl   $10
    jmp isr_stub


.globl isr11
isr11:
    pushl   $11
    jmp isr_stub


.globl isr12
isr12:
    pushl   $12
    jmp isr_stub


.globl isr13
isr13:
    pushl   $13
    jmp isr_stub


.globl isr14
isr14:
    pushl   $14
    jmp isr_stub


.globl isr16
isr16:
    pushl   $0

    pushl   $16
    jmp isr_stub


.globl isr17
isr17:
    pushl   $0

    pushl   $17
    jmp isr_stub


.globl isr18
isr18:
    pushl   $0

    pushl   $18
    jmp isr_stub


.globl isr19
isr19:
    pushl   $0

    pushl   $19
    jmp isr_stub


.extern fault_handler
isr_stub:
    pusha

    mov %esp, %eax
    push %eax

    call fault_handler

    addl $4, %esp

    popa
    
    addl $8, %esp

    iret

irq_stub:
    pusha
    
    mov %esp, %eax
    pushl %eax

    call fault_handler

    addl $4, %esp
    
    popa

    movl    4(%esp), %eax
    subl    $8, %eax
    js      EOI_PIC1     /* irq less than 8 */

EOI_PIC2:
    movb    $0x20, %al
    outb    %al, $PIC2


EOI_PIC1:
    movb    $0x20, %al
    outb    %al, $PIC1


    addl $8, %esp

    iret

.globl irq0
irq0:
    pushl $0
    pushl $32
    jmp irq_stub

.globl irq1
irq1:
    pushl $1
    pushl $33
    jmp irq_stub

.globl irq2
irq2:
    pushl $2
    pushl $34
    jmp irq_stub

.globl irq3
irq3:
    pushl $3
    pushl $35
    jmp irq_stub

.globl irq4
irq4:
    pushl $4
    pushl $36
    jmp irq_stub

.globl irq5
irq5:
    pushl $5
    pushl $37
    jmp irq_stub

.globl irq6
irq6:
    pushl $6
    pushl $38
    jmp irq_stub

.globl irq7
irq7:
    pushl $7
    pushl $39
    jmp irq_stub

.globl irq8
irq8:
    pushl $8
    pushl $40
    jmp irq_stub

.globl irq9
irq9:
    pushl $9
    pushl $41
    jmp irq_stub

.globl irq10
irq10:
    pushl $10
    pushl $42
    jmp irq_stub

.globl irq11
irq11:
    pushl $11
    pushl $43
    jmp irq_stub

.globl irq12
irq12:
    pushl $12
    pushl $44
    jmp irq_stub

.globl irq13
irq13:
    pushl $13
    pushl $45
    jmp irq_stub

.globl irq14
irq14:
    pushl $14
    pushl $46
    jmp irq_stub

.globl irq15
irq15:
    pushl $15
    pushl $47
    jmp irq_stub

.equiv PIC1, 0x20
.equiv PIC2, 0xA0

.equiv ICW1, 0x11
.equiv PIC1_ICW2, 0x20  /* 32, start interrupt vector number */
.equiv PIC2_ICW2, 0x28  /* 40, start interrupt vector number */
.equiv PIC1_ICW3, 0x04  /* bit 2 set, 2 has slave */
.equiv PIC2_ICW3, 0x02  /* slave is at master IRQ 2 */
.equiv ICW4, 0x01       /* 8086 mode */


.global pic_init
pic_init:

    /* SENDING ICW1 */

    movb    $ICW1, %al
    outb    %al, $PIC1
    outb    %al, $PIC2

    /* SENDING ICW2 */
    movb    $PIC1_ICW2, %al
    outb    %al, $(PIC1 + 1)

    movb    $PIC2_ICW2, %al
    outb    %al, $(PIC2 + 1)

    /* SENDING ICW3 */

    movb    $PIC1_ICW3, %al
    outb    %al, $(PIC1 + 1)

    movb    $PIC2_ICW3, %al
    outb    %al, $(PIC2 + 1)

    /* SENDING ICW4 */

    movb    $ICW4, %al
    outb    %al, $(PIC1 + 1);
    outb    %al, $(PIC2 + 1);

    mov $0, %al
    outb    %al, $(PIC1 + 1)
    outb    %al, $(PIC2 + 1)

    ret

.global halt_impl
halt_impl:
    hlt
halt_loop:
    jmp halt_loop

.global set_page_dir
set_page_dir:
    pushl %ebp
    movl %esp, %ebp

    movl 8(%ebp), %eax
    andl $0xFFFFF000, %eax
    
    mov %cr3, %ebx
    or %ebx, %eax
    mov %eax, %cr3

    popl %ebp
    
    ret

.global enable_paging
enable_paging:
    mov %cr0, %eax
    or  $(1 << 31), %eax
    
    mov %eax, %cr0

    ret

.global disable_paging
disable_paging:
    mov %cr0, %eax
    and $~(1 << 31), %eax

    mov %eax, %cr0

    ret

.global get_cr2
get_cr2:
    mov %cr2, %eax
    ret

.global pg_enabled
pg_enabled:
    mov %cr0, %eax
    andl $(1 << 31), %eax
    shr $31, %eax
    ret

.global invlpg
invlpg:
    pushl %ebp
    movl %esp, %ebp
    
    invlpg 8(%ebp)

    popl %ebp
    ret

