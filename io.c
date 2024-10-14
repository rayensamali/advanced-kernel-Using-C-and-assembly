#include "io.h"

// Read a byte from the specified I/O port
unsigned char inb(unsigned short port) {
    unsigned char result;
    // Inline assembly to perform the I/O read
    asm volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

// Write a byte to the specified I/O port
void outb(unsigned short port, unsigned char data) {
    // Inline assembly to perform the I/O write
    asm volatile ("outb %0, %1" : : "a"(data), "Nd"(port));
}

// Read a word (2 bytes) from the specified I/O port
unsigned short inw(unsigned short port) {
    unsigned short result;
    // Inline assembly to perform the I/O read
    asm volatile ("inw %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

// Write a word (2 bytes) to the specified I/O port
void outw(unsigned short port, unsigned short data) {
    // Inline assembly to perform the I/O write
    asm volatile ("outw %0, %1" : : "a"(data), "Nd"(port));
}
