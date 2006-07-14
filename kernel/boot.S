#define ASM	1
#include <multiboot.h>
	
	.text

	.globl	start, _start
start:
_start:
    movl $(stack + STACK_SIZE), %esp
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
stublet:
    call cmain
loop:
    jmp loop

    .align 4
    .comm	stack, STACK_SIZE
