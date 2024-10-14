#include "stringlib.h"
#include "command_line.h"
#include "kernel.h"
#include "file_system.h"
#include "disk_io.h"

/* External variables */
extern unsigned int current_loc;
extern char *vidptr;

/* Command structure definition */
typedef struct {
    const char *name;
    void (*func)(int argc, char* argv[]);
} Command;

/* Buffer to store user input */
char command_buffer[COMMAND_MAX_LENGTH];
unsigned int command_index = 0;

/* Command table */
Command commands[] = {
    {"clear", cmd_clear},
    {"help", cmd_help},
    {"print", cmd_print},
    {"ls", cmd_ls},
    {"rm", cmd_rm},
    {"write", cmd_write},
    {"read", cmd_read}
};

int num_commands = sizeof(commands) / sizeof(Command);

/* Function to print the command prompt */
void print_command_prompt(void) {
    print_string("TUN-OS> ");
    move_cursor(current_loc);
}

/* Command handler function */
void handle_command(int argc, char* argv[]) {
    for (int i = 0; i < num_commands; i++) {
        if (strcmp(argv[0], commands[i].name) == 0) {
            commands[i].func(argc, argv);
            return;
        }
    }

    // If command is not found
    print_string("Unknown command: ");
    print_string(argv[0]);
    kprint_newline();
}

/* Parse command input into arguments */
void parse_command(char* input) {
    char* argv[MAX_ARGC];
    int argc = 0;
    char* token = strtok(input, " ");  // Split by spaces

    while (token != NULL && argc < MAX_ARGC) {
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }

    // Call the command handler with parsed arguments
    handle_command(argc, argv);
}

/* Execute a command from the input buffer */
void execute_command(void) {
    // Null-terminate the command
    command_buffer[command_index] = '\0';
    parse_command(command_buffer);
    // Reset the command index for the next command
    command_index = 0;
    print_command_prompt();
}

/* Add a character to the command buffer and display it on the screen */
void add_to_command_buffer(char c) {
    if (command_index < COMMAND_MAX_LENGTH - 1) {
        command_buffer[command_index++] = c;
        vidptr[current_loc++] = c;
        vidptr[current_loc++] = 0x07;  // Light grey on black
    }
}

/*
 * Implementations of the actual commands
 */

// Clear command
void cmd_clear(int argc, char* argv[]) {
    clear_screen();
    display_top_bar();
}

// Help command
void cmd_help(int argc, char* argv[]) {
    print_string("Available commands: clear, help, print <string>");
    kprint_newline();
    print_string("clear : to clear the command line");
    kprint_newline();
    print_string("help : display the available commands");
    kprint_newline();
    print_string("print <string> : prints the provided string");
    kprint_newline();
    print_string("** this command line version is still in development **");
    kprint_newline();
}

// Print command with arguments
void cmd_print(int argc, char* argv[]) {
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            print_string(argv[i]);
            if (i < argc - 1) {
                print_string(" ");  // Add space between arguments
            }
        }
    } else {
        print_string("Error: no string provided to print");
    }
    kprint_newline();
}

void cmd_write(int argc, char* argv[]){
    if(argc > 2){
        save_file_to_disk(argv[1], &argv[2], argc-2);
    }else{
        print_string("Usage: write <filename> <contents>");
        kprint_newline();
    }
}

void cmd_read(int argc, char* argv[]){
    char buffer[512] = {0};
    if(argc > 1){
        load_file_from_disk(argv[1], buffer);
        print_string(buffer);  // Print the file content
    }else{
        print_string("Usage: read <filename>");
        kprint_newline();
    }
}

void cmd_rm(int argc, char* argv[]){
    if(argc > 1){
        delete_file(argv[1]);
    }else{
        print_string("Usage: rm <filename>");
        kprint_newline();
    }
}

void cmd_ls(int argc, char* argv[]){
    list_files();
}

