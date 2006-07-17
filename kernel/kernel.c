#include <system.h>
#include <video.h>
#include <gdt.h>
#include <idt.h>
#include <clock.h>
#include <keyboard.h>

void cmain(void)
{
    int i;

    video_init();
    gdt_init();
    idt_init();
    
    clock_init();
    keyboard_init();

    puts("Hello!\n");

    for (i = 0; i < 24; i++) {
        printf("hello %d\n", i);
    }    

    puts("\nasdf\n");

    while (1);
}
