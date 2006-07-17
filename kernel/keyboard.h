#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__


#define KEY_SHIFT_MASK   (1 << 0)
#define KEY_CTRL_MASK    (1 << 1)
#define KEY_ALT_MASK     (1 << 2)


#define BREAK(k)    (k | 0x80)
#define IS_BREAK(k) (k & 0x80)

#define KEY_ERROR   0x0000
#define KEY_Esc     0x0001
#define KEY_1       0x0002
#define KEY_2       0x0003
#define KEY_3       0x0004
#define KEY_4       0x0005
#define KEY_5       0x0006
#define KEY_6       0x0007
#define KEY_7       0x0008
#define KEY_8       0x0009
#define KEY_9       0x000a
#define KEY_0       0x000b
#define KEY_minus   0x000c
#define KEY_plus    0x000d
#define KEY_Backspace 0x000e
#define KEY_Tab     0x000f
#define KEY_Q       0x0010
#define KEY_W       0x0011
#define KEY_E       0x0012
#define KEY_R       0x0013
#define KEY_T       0x0014
#define KEY_Y       0x0015
#define KEY_U       0x0016
#define KEY_I       0x0017
#define KEY_O       0x0018
#define KEY_P       0x0019
#define KEY_openbracket 0x001a
#define KEY_closebracket 0x001b
#define KEY_Enter   0x001c
#define KEY_LeftCtrl 0x001d
#define KEY_A       0x001e
#define KEY_S       0x001f
#define KEY_D       0x0020
#define KEY_F       0x0021
#define KEY_G       0x0022
#define KEY_H       0x0023
#define KEY_J       0x0024
#define KEY_K       0x0025
#define KEY_L       0x0026
#define KEY_colon   0x0027
#define KEY_quote   0x0028
#define KEY_TILDE   0x0029
#define KEY_LeftShift 0x002a
#define KEY_backslash 0x002b
#define KEY_Z       0x002c
#define KEY_X       0x002d 
#define KEY_C       0x002e
#define KEY_V       0x002f
#define KEY_B       0x0030
#define KEY_N       0x0031
#define KEY_M       0x0032
#define KEY_comma   0x0033
#define KEY_dot     0x0034
#define KEY_slash   0x0035
#define KEY_RightShift 0x0036
#define KEY_Keypad_star 0x0037
#define KEY_LeftAlt 0x0038
#define KEY_Space   0x0039
#define KEY_CapsLock 0x003a
#define KEY_F1      0x003b
#define KEY_F2      0x003c
#define KEY_F3      0x003d
#define KEY_F4      0x003e
#define KEY_F5      0x003f
#define KEY_F6      0x0040
#define KEY_F7      0x0041
#define KEY_F8      0x0042
#define KEY_F9      0x0043
#define KEY_F10     0x0044
#define KEY_NumLock 0x0045
#define KEY_ScrollLock 0x0046
#define KEY_Keypad_7 0x0047
#define KEY_Keypad_8 0x0048
#define KEY_Keypad_9 0x0049
#define KEY_Keypad_minus 0x004a
#define KEY_Keypad_4 0x004b
#define KEY_Keypad_5 0x004c
#define KEY_Keypad_6 0x004d
#define KEY_Keypad_plus 0x004e
#define KEY_Keypad_1 0x004f
#define KEY_Keypad_2 0x0050
#define KEY_Keypad_3 0x0051
#define KEY_Keypad_0 0x0052
#define KEY_Keypad_dot 0x0053
#define KEY_AltSysRq 0x0054

#define KEY_F11 0x0057
#define KEY_F12 0x0058

#define KEY_Keypad_Enter 0xe01c
#define KEY_RightCtrl    0xe01d
#define KEY_fake_LeftShift 0xe02a
#define KEY_Keypad_slash   0xe035
#define KEY_fake_RightShift 0xe036
#define KEY_CtrlPrtScn  0xe037
#define KEY_RightAlt  0xe038
#define KEY_CtrlBreak 0xe046
#define KEY_GreyHome 0xe047
#define KEY_GreyUp 0xe048
#define KEY_GreyPgUp 0xe049
#define KEY_GreyLeft 0xe04b
#define KEY_GreyRight 0xe04d
#define KEY_GreyEnd 0xe04f
#define KEY_GreyDown 0xe050
#define KEY_GreyPgDn 0xe051
#define KEY_GreyInsert 0xe052
#define KEY_GreyDelete 0xe053

void
keyboard_init(void);

typedef enum {
    KEY_PRESS, KEY_RELEASE
} key_event_type_t;

typedef struct {
    key_event_type_t type;
    unsigned short key;
    unsigned char state;
} key_event_t;

typedef int (*key_handler_t)(key_event_t *event);

void
keyboard_set_handler(key_handler_t handler);

#endif /* __KEYBOARD_H__ */
