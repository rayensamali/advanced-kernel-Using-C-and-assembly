/*
 *  kernel.c
*/
#include "keyboard_map.h"

#define LINES 25
#define COLUMNS_IN_LINES 80
#define BYTES_FOR_EACH_ELEMENT 2
#define SCREENSIZE (BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINES * LINES)
#define KERNEL_CODE_SEGMENT_OFFSET  0x08
#define IDT_INTERRUPT_GATE 0x8e
#define PIC1_COMMAND_PORT 0X20
#define PIC1_DATA_PORT 0X21
#define PIC2_COMMAND_PORT 0XA0
#define PIC2_DATA_PORT 0XA1
#define KEYBOARD_STATUS_PORT 0X64
#define KEYBOARD_DATA_PORT 0X60
#define IDT_SIZE 256
#define ENTRY_KEY_CODE 0X1C

extern unsigned char keyboard_map[128];
extern void keyboard_handler(void);
extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);

/* current cursor location*/
unsigned int current_loc = 0;

/* video memory beginns at adress 0xb8000*/
char *vidptr = (char*) 0xb8000;






/* IDT stand for Interrupt descruptor table   */
struct IDT_entry{
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
};



struct IDT_entry IDT[IDT_SIZE];


void idt_init(void){
	unsigned long keyboard_address; /*offset*/
	unsigned long idt_address;
	unsigned long idt_ptr[2];

	/*
	 * populate IDT entry of keyboard's interrupt
	 */
	keyboard_address = (unsigned long) keyboard_handler;
	IDT[0x21].offset_lowerbits = keyboard_address & 0xFFFF;
	IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x21].zero = 0;
	IDT[0x21].type_attr = IDT_INTERRUPT_GATE;
	IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;
	

	/*
	 * Ports
	 * 		PIC1		PIC2
	 * command	0x20		0xA0
	 * data		0x21		0xA1
	 */

	/* ICW 1 - begin initialization */

	write_port(PIC1_COMMAND_PORT, 0x11);
	write_port(PIC2_COMMAND_PORT, 0x11);
	/* ICW 2 - remap offset address of IDT*/

	/*
	 * IN x 86 protected mode , we have to remap the PICs BEYOND 0X20 because Intel heve desined the first 32 
	 * interrupts as reserved for cpu exeption
	 */
	 write_port(PIC1_DATA_PORT, 0X20);
	 write_port(PIC2_DATA_PORT, 0X28);


	 /*  ICW3 - setup cascading*/
	 write_port(PIC1_DATA_PORT, 0X00);
	 write_port(PIC2_DATA_PORT, 0X00);

	 /*  ICW4 - envirement info*/
	/*0x01 indication for using 80x86 */
	 write_port(PIC1_DATA_PORT, 0X01);
	 write_port(PIC1_DATA_PORT, 0X01);

	/* mask interrupts */

	 write_port(PIC1_DATA_PORT, 0Xff);
	 write_port(PIC2_DATA_PORT, 0Xff);
	
	 /* fill the idt descriptor */

	 idt_address = (unsigned int)  IDT;

	 idt_ptr[0] = (sizeof (struct IDT_entry)* IDT_SIZE) + ((idt_address & 0xffff) << 16);

	 idt_ptr[1] = idt_address >> 16;

	 load_idt(idt_ptr);


}

void kb_init(void){
	/*
	 * 0xFD is 11111101 - enables only IRQ1 (keyboard) 0: enabled 1: disabled
	 */
	write_port(0x21, 0xFD);
}

void kprint_newline(void){
	unsigned int line_size = BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINES;
	current_loc= current_loc + (line_size - (current_loc % (line_size)));
}
void keyboard_handler_main(void){

	unsigned char status;
	char keycode;

	/* write EOI */

	write_port(0x20, 0x20);
	status = read_port(KEYBOARD_STATUS_PORT);
	/* lowesr bit of status will be set if buffer is not empty */
	
	if(status & 0x1){
		keycode = read_port(KEYBOARD_DATA_PORT);
		if(keycode <0 && keycode < 128)
			return;

		if(keycode == ENTRY_KEY_CODE){
			kprint_newline();
			return;
		}

		vidptr[current_loc++] = keyboard_map[(unsigned char ) keycode];
		vidptr[current_loc++] = 0x07;
			
	}



}





void clear_screen(void);
void print_string(const char *str);


void kmain(void)
{
	const char *str = "my first kernel using C language";
	clear_screen();
	print_string(str);
	kprint_newline();
	kprint_newline();
	
	idt_init();
	kb_init();
	
	while(1);

	return;
}

/*
 * screen claer function
 */
void clear_screen(void)
{
	
	unsigned int j = 0;
	/* this loop clears  the screen 
	 * there are 25 lines each of 80 columns ; each element takes 2 bytes 
	 */
	while ( j < SCREENSIZE )
	{
		/*blank character */
		vidptr[j++] = ' ';
		/* attribute-  byte - light magenta on black screen */
		vidptr[j++] = 0x0d;
			
	}	
}

void print_string(const char *str){
	
	unsigned int i = 0;	
	
	/*
	 * this loop writes the string to the video memory
	 */
	while (str[i] != '\0') {
		vidptr[current_loc++] = str[i++]; /* the character's 	ASCII */
		vidptr[current_loc++] = 0x07; /* attribute byte : light grey black screen */
		
	}
}
