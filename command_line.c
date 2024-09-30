#include "keyboard_map.h"
#include "command_line.h"

#define COMMAND_MAX_LENGTH 128

/* Current cursor location for the command line  */

extern unsigned int current_loc;
extern char *vidptr;

/* Buffer to store user input  */

char command_buffer[COMMAND_MAX_LENGTH];
unsigned int command_index = 0;

/*
* Prints the command prompt
*/

void print_command_prompt(void){
	print_string("TUN-OS> ");
}

/*
*	Handles a single command based on the input buffer
*/

void execute_command(void){
	command_buffer[command_index] = '\0'
	if(command_index==0){
		print_command_prompt();
		return;
	}
	if(strcmp(command_buffer,"clear") == 0){
		clear_screen();
		display_top_bar();
		print_command_prompt();
	}else if(strcmp(command_buffer,"help") == 0){ 
	       print_string("Available commands : clear, help  " );
	       kprint_newline();
	       print_string("clear : to clear all the screen from the commands ");
		kprint_newline("help : help will display all the commands you can use  ");
		kprint_newline();
	       print_sting("** this command line version is still in developpment**");
	}else {
		print_string("Unknown command :");
		print_string("command_buffer");
		kprint_newline();
		print_command_prompt();
	}
/* Reset the command buffer to index */
	command_index = 0;

}

/*
* Adds a character to the command buffer and displays it on the screen
*/

void add_to_command_buffer(char c){
	if(command_index < COMMAND_MAX_LENGTH-1){
		command_buffer[command_index++] = c;
		vidptr[current_loc++] = c;
		vidptr[current_loc++] = 0x07;
	}
}




