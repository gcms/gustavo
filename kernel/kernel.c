#include <system.h>

#include <video.h>

#include <gdt.h>
#include <idt.h>
#include <paging.h>

#include <clock.h>
#include <keyboard.h>
#include <prompt.h>
#include <kmalloc.h>
#include <ata.h>

#include <assert.h>

multiboot_info_t multiboot_info;

void cmain(multiboot_info_t *info)
{
    unsigned long *data;

    multiboot_info = *info;

    video_init();
    gdt_init();
    idt_init();
    paging_init();

    clock_init();
    ata_init();
    keyboard_init();

    prompt_init();

    puts("Hello!\n");

    data = kmalloc(512);

    ata_read_sector(PRIMARY, MASTER, 0, data);
    printf("data[0] = 0x%x\n", data[0]);

    while (1);
}
