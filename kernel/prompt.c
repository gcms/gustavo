#include <prompt.h>
#include <keyboard.h>
#include <video.h>

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

static char buffer[512];
static int bufferptr;

static int
default_handler(key_event_t *event)
{
    int i;

    if (event->type == KEY_RELEASE)
        return 0;

    if (event->key == KEY_Backspace) {
        buffer[bufferptr] = 0;
        bufferptr--;
        putch('\b');
        return 1;
    }

    if (event->key == KEY_Enter) {
        buffer[bufferptr] = 0;
        putch('\n');
        /* parse_cmd */
        printf("prompt> ");
        return 1;
    }

    if (bufferptr >= sizeof(buffer) - 2 && event->key != KEY_Enter) {
        return 0;
    }


    for (i = 0; i < sizeof(us_iso) / sizeof(keymap_t); i++) {
        if (event->key == us_iso[i].key) {
            buffer[bufferptr] = us_iso[i].ascii[event->state & KEY_SHIFT_MASK];
            putch(buffer[bufferptr]);
            bufferptr++;

            return 1;
        }
    }
    
    return 0;
}

void
prompt_init(void)
{
    bufferptr = 0;
    keyboard_set_handler(default_handler);
    printf("prompt> ");
}
