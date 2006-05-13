CC=gcc
CFLAGS=-Wall -O -I. -nostdinc -fno-builtin -fomit-frame-pointer -fstrength-reduce -finline-functions

ASM=nasm
ASM_OPTS=-f elf

LD_SCRIPT=link.ld
LD=ld
LD_OPTS=-T $(LD_SCRIPT) -N

all: start.o gdt.o idt.o isrs.o irq.o scrn.o timer.o kb.o main.o
	$(LD) $(LD_OPTS) start.o gdt.o idt.o isrs.o irq.o scrn.o timer.o kb.o \
   	main.o -o kernel.bin

start.o: start.asm
	$(ASM) $(ASM_OPTS) start.asm -o start.o

gdt.o: gdt.c
	$(CC) $(CFLAGS) -c gdt.c -o gdt.o

idt.o: idt.c
	$(CC) $(CFLAGS) -c idt.c -o idt.o

isrs.o: isrs.c
	$(CC) $(CFLAGS) -c isrs.c -o isrs.o

irq.o: irq.c
	$(CC) $(CFLAGS) -c irq.c -o irq.o

scrn.o: scrn.c
	$(CC) $(CFLAGS) -c scrn.c -o scrn.o

kb.o: kb.c
	$(CC) $(CFLAGS) -c kb.c -o kb.o

timer.o: timer.c
	$(CC) $(CFLAGS) -c timer.c -o timer.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

clean:
	rm -f start.o
	rm -f gdt.o
	rm -f idt.o
	rm -f irq.o
	rm -f scrn.o
	rm -f main.o
	rm -f kernel.bin
