#include "stringlib.h"

int strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

int strncmp(const char *str1, const char *str2, size_t n) {
    if (n == 0) return 0;  // If n is 0, the strings are considered equal

    while (n > 0) {
        if (*str1 != *str2) {
            return (unsigned char)(*str1) - (unsigned char)(*str2);
        }
        if (*str1 == '\0') {
            return 0;  // Strings are equal if both reach the null character
        }
        str1++;
        str2++;
        n--;
    }
    return 0;  // If n reaches 0, strings are equal up to the first n characters
}

char *strchr(const char *str, int c) {
    while (*str) {
        if (*str == (char)c) {
            return (char *)str;
        }
        str++;
    }

    if (*str == (char)c) {
        return (char *)str;
    }
    return NULL;
}

char *strtok(char *str, const char *delim) {
    static char *next_token = NULL;  // Declare next_token as a static variable
    char *start = (str) ? str : next_token;  // Continue from the previous token if str is NULL

    if (!start) return NULL;  // No more tokens

    // Skip leading delimiters
    while (*start && strchr(delim, *start)) {
        start++;
    }

    if (*start == '\0') {  // No more tokens
        next_token = NULL;
        return NULL;
    }

    char *token = start;  // Mark the token start

    // Find the next delimiter
    while (*start && !strchr(delim, *start)) {
        start++;
    }

    if (*start) {
        *start = '\0';  // Null-terminate the token
        next_token = start + 1;
    } else {
        next_token = NULL;
    }

    return token;
}

char *strncpy(char *dest, const char *src, unsigned int n) {
    unsigned int i;

    // Copy up to 'n' characters from src to dest
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];  // Copy from src to dest
    }

    // If src is shorter than 'n', pad the rest of dest with '\0'
    for (; i < n; i++) {
        dest[i] = '\0';
    }

    return dest;
}

/*
 * Calculate the length of the string 'str'.
 */
int strlen(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

