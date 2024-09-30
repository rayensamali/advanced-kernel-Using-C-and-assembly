unsigned char keyboard_map[128] = {
    0,  27, '&', 0xE9, '"', '\'', '(', '-', 0xE8, '_', 0xE7, 0xE0, ')', '=', '\b',    /* Backspace */
    '\t', /* Tab */ 'a', 'z', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '^', '$', '\n', /* Enter key */
    0, /* Control */ 'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 0xF9, '`',
    0, /* Left Shift */ '\\', 'w', 'x', 'c', 'v', 'b', 'n', ',', ';', ':', '!', 0, /* Right Shift */
    '*',
    0, /* Alt */ ' ', /* Space bar */ 0, /* Caps lock */
    0, /* F1 */ 0, /* F2 */ 0, /* F3 */ 0, /* F4 */ 0, /* F5 */ 0, /* F6 */ 0, /* F7 */ 0, /* F8 */
    0, /* F9 */ 0, /* F10 */ 0, /* Num lock */ 0, /* Scroll Lock */
    0, /* Home key */ 0, /* Up arrow */ 0, /* Page Up */ '-',
    0, /* Left arrow */ 0, /* Right arrow */ '+',
    0, /* End key */ 0, /* Down arrow */ 0, /* Page Down */ 0, /* Insert key */ 0, /* Delete key */
    0, 0, 0, 0, /* F11 Key */ 0 /* F12 Key */
};

unsigned char keyboard_map_shifted[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 0xB0, '+', '\b',    /* Backspace */
    '\t', /* Tab */ 'A', 'Z', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 0xA8, '*', '\n', /* Enter key */
    0, /* Control */ 'Q', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', '%', 0xA3,
    0, /* Left Shift */ '>', 'W', 'X', 'C', 'V', 'B', 'N', '?', '.', '/', 0xA7, 0, /* Right Shift */
    '*',
    0, /* Alt */ ' ', /* Space bar */ 0, /* Caps lock */
    0, /* F1 */ 0, /* F2 */ 0, /* F3 */ 0, /* F4 */ 0, /* F5 */ 0, /* F6 */ 0, /* F7 */ 0, /* F8 */
    0, /* F9 */ 0, /* F10 */ 0, /* Num lock */ 0, /* Scroll Lock */
    0, /* Home key */ 0, /* Up arrow */ 0, /* Page Up */ '-',
    0, /* Left arrow */ 0, /* Right arrow */ '+',
    0, /* End key */ 0, /* Down arrow */ 0, /* Page Down */ 0, /* Insert key */ 0, /* Delete key */
    0, 0, 0, 0, /* F11 Key */ 0 /* F12 Key */
};
