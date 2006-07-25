#define ASM	1
#include <multiboot.h>
	
	.text

	.globl	start, _start
start:
_start:
	jmp	stublet

	/* Multiboot header.  */
	.align	4
mboot:
	/* magic */
	.long	MULTIBOOT_HEADER_MAGIC
	/* flags */
	.long	MULTIBOOT_HEADER_FLAGS
	/* checksum */
	.long	-(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

    .extern cmain
    .extern halt
stublet:
    movl $(stack + STACK_SIZE), %esp

    pushl $0
    popf

    cmp $MULTIBOOT_BOOTLOADER_MAGIC, %eax
    jne call_halt

    pushl %ebx
    
    call cmain

call_halt:
    call halt


.global get_stack
get_stack:
    movl $stack, %eax
    ret

/* Changes the value of ESP.
 * OBS: is it a good approach? */
.global change_stack
change_stack:
    movl %esp, %eax
    movl $stack, %ebx

    subl %ebx, %eax
    addl $(0xFFFFF000 - STACK_SIZE + 4096), %eax
    movl %eax, %esp

    ret

/* We align the stack on a page boundary, so that when we enable
 * paging it can be easily mapped */
.bss
.align 4096
stack:
.space STACK_SIZE
