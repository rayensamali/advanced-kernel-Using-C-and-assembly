#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#define COMMAND_MAX_LENGTH 128
#define MAX_ARGC 10


void print_command_prompt(void);
void execute_command(void);
void add_to_command_buffer(char c);
void cmd_clear(int argc, char* argv[]);
void cmd_help(int argc, char* argv[]);
void cmd_print(int argc, char* argv[]);
void cmd_read(int argc, char *argv[]);
void cmd_ls(int argc, char *argv[]);
void cmd_rm(int argc, char *argv[]);
void cmd_write(int argc, char *argv[]);

#endif
