#include <keyboard.h>
#include <idt.h>
#include <stdlib.h>
#include <stdio.h>
#include <video.h>

static unsigned char status = 0;

static unsigned short key = 0;

static unsigned int discard = 0;

static key_handler_t keyboard_handler = NULL;

void
keyboard_set_handler(key_handler_t handler)
{
    keyboard_handler = handler;
}

static int
keyboard_handle(stack_frame_t *frame)
{
    unsigned char data = inportb(0x60);

    if (discard > 0) {
        discard--;
        return 0;
    }

    switch (data) {
        case KEY_LeftShift:
            status |= KEY_SHIFT_MASK;
            break;
        case BREAK(KEY_LeftShift):
            status &= ~KEY_SHIFT_MASK;
            break;
        case KEY_LeftCtrl:
            status |= KEY_CTRL_MASK;
            break;
        case BREAK(KEY_LeftCtrl):
            status &= ~KEY_CTRL_MASK;
            break;
        case KEY_LeftAlt:
            status |= KEY_ALT_MASK;
            break;
        case BREAK(KEY_ALT_MASK):
            status &= ~KEY_ALT_MASK;
            break;
        case 0xe0:
            key = data;
            break;
        case 0xe1:
            discard = 2;
            break;
        default:
            if (key == 0xe0) {
                key = (key << 8) | data;
            } else {
                key = data;
            }

            if (keyboard_handler) {
                key_event_t event;
                event.type = (key & 0x0080) ? KEY_RELEASE : KEY_PRESS;
                event.key = (key  & ~0x0080);
                event.state = status;
                keyboard_handler(&event);
            }

            break;
    }


    return 0;
}

void
keyboard_init(void)
{
    irq_install_handler(1, keyboard_handle);
}
