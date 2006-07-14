ENTRY(start)
OUTPUT_FORMAT(elf32-i386)
SECTIONS
{
    . = 0x100000;
    PROVIDE(kstart = .);
    .text : {
        code = .;
        *(.text)
        *(.rodata.*)
        . = ALIGN(4096);
    }
    .data : {
        data = .;
        *(.data)
        . = ALIGN(4096);
    }
    .bss : {
        bss = .;
        *(.bss)
        . = ALIGN(4096);
    }
    PROVIDE(kend = .);
    end = .;
}
