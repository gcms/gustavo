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

typedef enum {
    BASE, SHIFT, NUM_STATE
} key_state_t;

typedef struct {
    unsigned short key;
    unsigned char ascii[NUM_STATE];
    unsigned char special;
    unsigned char flags;
} keymap_t;


keymap_t us_iso[] = {
	{ KEY_A, { 'a', 'A' }, 0, 0 },
	{ KEY_B, { 'b', 'B' }, 0, 0 },
	{ KEY_C, { 'c', 'C' }, 0, 0 },
	{ KEY_D, { 'd', 'D' }, 0, 0 },
	{ KEY_E, { 'e', 'E' }, 0, 0 },
	{ KEY_F, { 'f', 'F' }, 0, 0 },
	{ KEY_G, { 'g', 'G' }, 0, 0 },
	{ KEY_H, { 'h', 'H' }, 0, 0 },
	{ KEY_I, { 'i', 'I' }, 0, 0 },
	{ KEY_J, { 'j', 'J' }, 0, 0 },
	{ KEY_K, { 'k', 'K' }, 0, 0 },
	{ KEY_L, { 'l', 'L' }, 0, 0 },
	{ KEY_M, { 'm', 'M' }, 0, 0 },
	{ KEY_N, { 'n', 'N' }, 0, 0 },
	{ KEY_O, { 'o', 'O' }, 0, 0 },
	{ KEY_P, { 'p', 'P' }, 0, 0 },
	{ KEY_Q, { 'q', 'Q' }, 0, 0 },
	{ KEY_R, { 'r', 'R' }, 0, 0 },
	{ KEY_S, { 's', 'S' }, 0, 0 },
	{ KEY_T, { 't', 'T' }, 0, 0 },
	{ KEY_U, { 'u', 'U' }, 0, 0 },
	{ KEY_V, { 'v', 'V' }, 0, 0 },
	{ KEY_W, { 'w', 'W' }, 0, 0 },
	{ KEY_X, { 'x', 'X' }, 0, 0 },
	{ KEY_Y, { 'y', 'Y' }, 0, 0 },
	{ KEY_Z, { 'z', 'Z' }, 0, 0 },
    { KEY_Space, { ' ', ' ' }, 0, 0 },
    { KEY_Backspace, { '\b', '\b' }, 0, 0 },
    { KEY_Enter, { '\n', '\n' }, 0, 0 },
    { KEY_1, { '1', '!' }, 0, 0 },
    { KEY_2, { '2', '@' }, 0, 0 },
    { KEY_3, { '3', '#' }, 0, 0 },
    { KEY_4, { '4', '$' }, 0, 0 },
    { KEY_5, { '5', '%' }, 0, 0 },
    { KEY_6, { '6', '^' }, 0, 0 },
    { KEY_7, { '7', '&' }, 0, 0 },
    { KEY_8, { '8', '*' }, 0, 0 },
    { KEY_9, { '9', '(' }, 0, 0 },
    { KEY_0, { '0', ')' }, 0, 0 },
    { KEY_minus, { '-', '_' }, 0, 0 },
    { KEY_plus, { '=', '+' }, 0, 0 },
    { KEY_backslash, { '\\', '|' }, 0, 0 },
    { KEY_slash, { '/', '?' }, 0, 0 },
    { KEY_quote, { '\'', '"' }, 0, 0 },
    { KEY_comma, { ',', '<' }, 0, 0 },
    { KEY_dot, { '.', '>' }, 0, 0 }
};

static int
default_handler(key_event_t *event)
{
    int i;

    if (event->type == KEY_RELEASE)
        return 0;


    for (i = 0; i < sizeof(us_iso) / sizeof(keymap_t); i++) {
        if (event->key == us_iso[i].key) {
            putch(us_iso[i].ascii[event->state & KEY_SHIFT_MASK]);
            return 1;
        }
    }
    
    return 0;
}

void
keyboard_init(void)
{
    irq_install_handler(1, keyboard_handle);
    keyboard_set_handler(default_handler);
}
