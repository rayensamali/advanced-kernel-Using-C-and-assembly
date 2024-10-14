# Compiler and Assembler flags
CFLAGS = -nostdlib -ffreestanding -fno-stack-protector -m32
ASFLAGS = -f elf32  # NASM flags for 32-bit ELF format

# Object files
OBJS = kernel.o command_line.o kernel_asm.o stringlib.o disk_io.o file_system.o io.o

# Output binary
kernel.bin: $(OBJS)
	ld -m elf_i386 -T link.ld -o kernel.bin $(OBJS)

# Compile C source files
%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

# Assemble ASM source files
kernel_asm.o: kernel.asm
	nasm $(ASFLAGS) kernel.asm -o kernel_asm.o

# Run the kernel in QEMU
run: kernel.bin
	qemu-system-i386 -kernel kernel.bin -drive file=disk.img,format=raw -m 512

# Clean the build directory
clean:
	rm -f $(OBJS) kernel.bin
