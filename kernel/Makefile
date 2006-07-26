CC=gcc
CFLAGS=-Wall -g -O -D__ELF__ -I. -gstabs+ -fno-builtin
LD=ld
LDFLAGS=-T link.ld

all: kernel.elf

kernel.elf: kernel.o system.o boot.o i386.o stdlib.o stdio.o assert.o video.o idt.o paging.o kmalloc.o ata.o process.o clock.o keyboard.o prompt.o
	$(LD) $(LDFLAGS) kernel.o system.o boot.o i386.o stdlib.o stdio.o assert.o video.o idt.o paging.o kmalloc.o ata.o process.o clock.o keyboard.o prompt.o -o kernel.elf

clean:
	rm -f kernel.o
	rm -f system.o
	rm -f i386.o
	rm -f stdlib.o
	rm -f assert.o
	rm -f stdio.o
	rm -f boot.o
	rm -f video.o
	rm -f idt.o
	rm -f paging.o
	rm -f kmalloc.o
	rm -f ata.o
	rm -f process.o
	rm -f clock.o
	rm -f keyboard.o
	rm -f prompt.o
	rm -f kernel.elf
