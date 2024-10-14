#ifndef IO_H
#define IO_H

// Read a byte from the specified I/O port
unsigned char inb(unsigned short port);

// Write a byte to the specified I/O port
void outb(unsigned short port, unsigned char data);

// Read a word (2 bytes) from the specified I/O port
unsigned short inw(unsigned short port);

// Write a word (2 bytes) to the specified I/O port
void outw(unsigned short port, unsigned short data);

#endif // IO_H
