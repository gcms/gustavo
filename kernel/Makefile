CC=gcc
CFLAGS=-Wall -g -O -D__ELF__ -I. -gstabs+ -fno-builtin
LD=ld
LDFLAGS=-T link.ld

all: kernel.elf

kernel.elf: kernel.o system.o boot.o i386.o stdlib.o stdio.o assert.o video.o idt.o clock.o keyboard.o
	$(LD) $(LDFLAGS) kernel.o system.o boot.o i386.o stdlib.o stdio.o assert.o video.o idt.o clock.o keyboard.o -o kernel.elf

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
	rm -f keyboard.o
	rm -f clock.o
	rm -f kernel.elf
