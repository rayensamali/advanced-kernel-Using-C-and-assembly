OBJS = kernel.o command_line.o kernel_asm.o
CFLAGS = -nostdlib -ffreestanding -fno-stack-protector
LDFLAGS = -T linker.ld -nostdlib

kernel.bin : $(OBJS)
	ld $(LDFLAGS) -o kernel.bin $(OBJS)

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

kernel_asm.o: kernel.asm
	nasm -f elf32 kernel.asm -o kernel_asm.o

clean:
	rm -f *.o kernel.bin

run:
	qemu-system-i386 -kernel kernel.bin
