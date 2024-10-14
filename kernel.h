#ifndef KERNEL_H
#define KERNEL_H

// Shared constants
#define LINES 25
#define COLUMNS_IN_LINES 80
#define BYTES_FOR_EACH_ELEMENT 2
#define SCREENSIZE (BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINES * LINES)
#define MIN_CURSOR_POS 160
#define TOP_BAR_HEIGHT 2


// Function declarations
void move_cursor(unsigned int location);
void print_string(const char* str);
void kprint_newline(void);
void print_int(int num);
void display_top_bar(void);
void clear_screen(void);
#endif  // KERNEL_H
