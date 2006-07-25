#include <system.h>

#include <video.h>

#include <gdt.h>
#include <idt.h>
#include <paging.h>

#include <clock.h>
#include <keyboard.h>

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
    keyboard_init();

    data = kmalloc(512);
    hd_read_sector(0, data);
    printf("dev0 0x%x\n", data[0]);

    data[0] = 0;

    hd_set_device(1);
    hd_read_sector(0, data);
    printf("dev1 0x%x\n", data[0]);
    
    puts("Hello!\n");

    while (1);
}
