/*
 *  kernel.c
 */

#include "keyboard_map.h"
#include "command_line.h"
#define LINES 25
#define COLUMNS_IN_LINES 80
#define BYTES_FOR_EACH_ELEMENT 2

#define SCREENSIZE (BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINES * LINES)
#define KERNEL_CODE_SEGMENT_OFFSET  0x08
#define IDT_INTERRUPT_GATE 0x8e

#define PIC1_COMMAND_PORT 0x20
#define PIC1_DATA_PORT 0x21
#define PIC2_COMMAND_PORT 0xA0
#define PIC2_DATA_PORT 0xA1

#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60

#define IDT_SIZE 256
#define ENTRY_KEY_CODE 0x1C

#define MIN_CURSOR_POS 160
#define TOP_BAR_HEIGHT 2

extern unsigned char keyboard_map[128];
extern unsigned char keyboard_map_shifted[128];
extern void keyboard_handler(void);
extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);
extern int command_index;



/* keyboard enable flag */
char keyboard_enabled = 0;

/* Current cursor location */
unsigned int current_loc = MIN_CURSOR_POS;

/* Video memory begins at address 0xb8000 */
char *vidptr = (char *)0xb8000;

/* Track if Shift is pressed */
char shift_pressed = 0;

/* IDT stands for Interrupt Descriptor Table */
struct IDT_entry {
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short int offset_higherbits;
};

struct IDT_entry IDT[IDT_SIZE];

/*
 * Clears the screen
 */
void clear_screen(void) {
    unsigned int j = 0;
    /* This loop clears the screen */
    while (j < SCREENSIZE) {
        vidptr[j++] = ' '; /* Blank character */
        vidptr[j++] = 0x0d; /* Light magenta on black screen */
    }
}

/*
 * Prints a string to the screen
 */
void print_string(const char *str) {
    unsigned int i = 0;
    /* Loop to write string to video memory */
    while (str[i] != '\0') {
        vidptr[current_loc++] = str[i++]; /* The character's ASCII */
        vidptr[current_loc++] = 0x07;     /* Light grey on black screen */
    }
}

/*
 * Initializes the IDT
 */
void idt_init(void) {
    unsigned long keyboard_address; /* Offset */
    unsigned long idt_address;
    unsigned long idt_ptr[2];

    /* Populate IDT entry for keyboard interrupt */
    keyboard_address = (unsigned long)keyboard_handler;
    IDT[0x21].offset_lowerbits = keyboard_address & 0xFFFF;
    IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
    IDT[0x21].zero = 0;
    IDT[0x21].type_attr = IDT_INTERRUPT_GATE;
    IDT[0x21].offset_higherbits = (keyboard_address & 0xFFFF0000) >> 16;

    /* Ports: PIC1, PIC2 */
    write_port(PIC1_COMMAND_PORT, 0x11);
    write_port(PIC2_COMMAND_PORT, 0x11);
    write_port(PIC1_DATA_PORT, 0x20);
    write_port(PIC2_DATA_PORT, 0x28);
    write_port(PIC1_DATA_PORT, 0x00);
    write_port(PIC2_DATA_PORT, 0x00);
    write_port(PIC1_DATA_PORT, 0x01);
    write_port(PIC2_DATA_PORT, 0x01);

    /* Fill the IDT descriptor */
    idt_address = (unsigned long)IDT;
    idt_ptr[0] = (sizeof(struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xFFFF) << 16);
    idt_ptr[1] = idt_address >> 16;
    load_idt(idt_ptr);
}
/*
 * Prints a newline on the screen
 */
void kprint_newline(void) {
    unsigned int line_size = BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINES;
    current_loc = current_loc + (line_size - (current_loc % line_size));
}
/*
 * Displays the top bar with tabs
 */
void display_top_bar(void) {
    unsigned short i;
    unsigned short local_cursor = 0;  // Start at the beginning of the screen
    char *tabs[] = {" Home ", " Text Editor ", " Command line "}; // Tab names
    int num_tabs = 3;  // Number of tabs
    unsigned char attribute = 0x71;  // Blue text (1) on Gray background (7)

    // Clear the first line and set background to gray
    for (i = 0; i < COLUMNS_IN_LINES; i++) {
        vidptr[local_cursor++] = ' ';  // Fill with spaces
        vidptr[local_cursor++] = attribute;  // Set attribute (blue on gray)
    }

    // Reset the cursor to the beginning of the screen for writing the tabs
    local_cursor = 0;

    // Display each tab in sequence with blue text and gray background
    for (int tab = 0; tab < num_tabs; tab++) {
        char *tab_name = tabs[tab];
        int j = 0;

        // Print the tab text
        while (tab_name[j] != '\0') {
            vidptr[local_cursor++] = tab_name[j++];  // Print the character
            vidptr[local_cursor++] = attribute;  // Set blue on gray
        }

        // Add some spacing between tabs
        if (tab < num_tabs - 1) {
            for (i = 0; i < 3; i++) {  // Add 3 spaces between tabs
                vidptr[local_cursor++] = ' ';
                vidptr[local_cursor++] = attribute;
            }
        }
    }

    // Reset cursor position under the top bar
    current_loc = BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINES * TOP_BAR_HEIGHT;
}

/* Displays the text in the home page */
void display_home_page(void) {
    print_string("Welcome to TUN-OS!");
    kprint_newline();
    print_string("This OS is created to help you learn how operating systems work.");
    kprint_newline();
    print_string("Use F1 for Home, F2 for Text Editor, and F3 for Root File System.");
    kprint_newline();
}

/*
 * Initializes the keyboard (enables IRQ1)
 */
void kb_init(void) {
    /* 0xFD enables only IRQ1 (keyboard) */
    write_port(0x21, 0xFD);
}



/*
 * Moves the cursor position in VGA text mode using hardware registers
 */
void move_cursor(unsigned int location) {
    unsigned short pos = location / 2;  // Divide by 2 since each character is 2 bytes (char + attribute)
    
    // Update hardware cursor position using VGA ports 0x3D4 (command) and 0x3D5 (data)
    write_port(0x3D4, 0x0F);  // Send lower byte of cursor position
    write_port(0x3D5, (unsigned char)(pos & 0xFF));
    
    write_port(0x3D4, 0x0E);  // Send higher byte of cursor position
    write_port(0x3D5, (unsigned char)((pos >> 8) & 0xFF));
}

/*
 * Blinks the cursor at the current location using hardware cursor control
 */
void blink_cursor(void) {
    static int blink_state = 0;

    if (blink_state) {
        move_cursor(current_loc);  // Set the cursor position
    } else {
        move_cursor(SCREENSIZE);  // Move the cursor off the screen to "hide" it
    }

    blink_state = !blink_state;
}

/*
 * A simple delay function to slow down cursor blinking
 */
void delay(unsigned int count) {
    while (count--) {
        asm volatile("nop");
    }
}

/*
 * Handles keyboard input
 */
void keyboard_handler_main(void) {
    unsigned char status;
    char keycode;

    /* Write EOI (End of Interrupt) */
    write_port(0x20, 0x20);

    /* Read keyboard status */
    status = read_port(KEYBOARD_STATUS_PORT);

    /* Check if buffer is not empty */
    if (status & 0x01) {
        keycode = read_port(KEYBOARD_DATA_PORT);

        /* Handle key release events (ignore releases) */
        if (keycode & 0x80) {
            keycode -= 0x80;  // Normalize keycode (subtract 0x80)

            // Handle Shift release
            if (keycode == 0x2A || keycode == 0x36) {  // Left Shift or Right Shift released
                shift_pressed = 0;  // Shift is no longer pressed
            }
            return;  // Ignore key releases
        }

        /* Handle F1, F2, F3 for tab switching */
        if (keycode == 0x3B) {  // F1 key (Home)
            clear_screen();
            display_top_bar();
            display_home_page();
            keyboard_enabled = 0;  // Disable typing on the Home page
            return;
        } else if (keycode == 0x3C) {  // F2 key (Text Editor)
            clear_screen();
            display_top_bar();
            kprint_newline();
            keyboard_enabled = 1;  // Enable typing on the Text Editor page
            return;
        } else if (keycode == 0x3D) {  // F3 key (Root File System)
            clear_screen();
            display_top_bar();
            print_command_prompt();
            keyboard_enabled = 1;  // Enable typing on the Root File System page
            return;
        }

        /* Detect Shift press */
        if (keycode == 0x2A || keycode == 0x36) {  // Left Shift or Right Shift pressed
            shift_pressed = 1;  // Set Shift as pressed
            return;
        }

        /* Ignore regular keys if keyboard input is disabled */
        if (!keyboard_enabled) {
            return;  // Keyboard input is disabled (on the Home page)
        }

        /* Handle Backspace key */
        if (keycode == 0x0E) {  // Backspace key
            if (current_loc > MIN_CURSOR_POS) {
                current_loc -= 2;  // Move back by one character (2 bytes per character in video memory)
                vidptr[current_loc] = ' ';  // Clear the character at cursor
                vidptr[current_loc + 1] = 0x07;  // Set the attribute (light grey on black)
                command_index--;
		move_cursor(current_loc);  // Update cursor position
            }
            return;
        }

        /* Handle regular key input if keyboard is enabled */
        if (keycode < 128) {
            if (shift_pressed) {
                vidptr[current_loc++] = keyboard_map_shifted[(unsigned char)keycode];  // Use shifted map when Shift is pressed
            } else {
                vidptr[current_loc++] = keyboard_map[(unsigned char)keycode];  // Use normal map when Shift is not pressed
            }
            vidptr[current_loc++] = 0x07;  /* Light grey on black */
        }

        /* Handle Enter key */
        if (keycode == ENTRY_KEY_CODE) {
            kprint_newline();
	    execute_command();
            return;
        }
    }

    // Update cursor position on screen
    move_cursor(current_loc);
}

/*
 * Kernel main function
 */
void kmain(void) {
    clear_screen();
    display_top_bar();
    kprint_newline();
    display_home_page();
    idt_init();
    kb_init();

    // Main loop
    while (1) {
        blink_cursor();  // Blink the cursor
        delay(100000);   // Adjust this value for slower blinking
    }
}

