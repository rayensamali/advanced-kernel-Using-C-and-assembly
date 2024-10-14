#ifndef STRINGLIB_H
#define STRINGLIB_H

#include <stddef.h>

static char *next_token = NULL;

int strcmp(const char *str1, const char *str2);
int strncmp(const char*str1, const char *str2,size_t n);
char *strncpy(char *dest, const char *src, unsigned int n);
char *strtok(char* str, const char *delim);
char *strchr(const char *str, int c);
int strlen(const char *str);
#endif
