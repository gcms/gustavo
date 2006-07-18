#include <system.h>

#include <video.h>

#include <gdt.h>
#include <idt.h>
#include <paging.h>

#include <clock.h>
#include <keyboard.h>

#include <assert.h>

multiboot_info_t multiboot_info;

void cmain(multiboot_info_t *info)
{
    multiboot_info = *info;

    video_init();
    gdt_init();
    idt_init();
    paging_init();

    clock_init();
    keyboard_init();

    puts("Hello!\n");

    while (1);
}
