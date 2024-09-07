;;kernel.asm
bits 32				;nasm directive - 32 bit
section .text
	;multiboot spec
	align 4 	
	dd 0x1BADB002	;magic
	dd 0x00		;flags
	dd -(0x1BADB002 + 0x00) ;chechksum. m+f+c should be zero
;dd define double word fo size 4bytes

global start
global keyboard_handler
global read_port
global write_port
global load_idt

extern kmain			;kmain is defined in the c file
extern keyboard_handler_main

read_port:
	mov edx, [esp + 4]
	in al, dx
	ret

write_port:
	mov edx,  [esp + 4]
	mov al, [esp + 4 + 4]
	out dx, al
	ret


load_idt:
	mov edx, [esp + 4]
	lidt [edx]
	sti
	ret
	;load_idt() function turns the interrupts on using sti instruction

keyboard_handler:
	call keyboard_handler_main
	iretd
start:
	cli				;block interrupts
	mov esp, stack_space		;set stack pointer
	call kmain
	hlt				;halt the CPU

section .bss
resb 8192				;8kn for stack
stack_space:
