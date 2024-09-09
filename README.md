# Custom Operating System Kernel

this repository cotains the source code for a basic custom operating system kernel.
This version of the kernel is desined to handle basic keyboard input and output to the screen, utilizing both C and assembly code.

#Features
- Basic keyboard input handling
- Screen output to the console
- Interrupt Descriptor Table (IDT) setup
- Programmable Interrupt Controllers (PIC) initialisation

## Prerequesities
Before you start, make sure you have the following tools installed:
- **Assembler**: nasm (Netwide assembler)
- **C compielr**: GCC (GNU Compiler Collection)
- **Linker**: LD (GNU Linker)
- **CMake**: CMake build configuration tool
- **Make**/**Ninja**: Build orchestration tools supported by CMake
- **Bootloader**: GRUB (Grand Unified Bootloader)

## Building the Kernel

To compile and buid the kernel, follow these steps:
  
 1. **Create build directory**
     Create build directory where kernel will be built, this assumes that Ninja is used but you can replace it with Make:
     ```sh
     cmake -G Ninja -B build .
     ```
 2. **Build the source code**
    This will build all the source code and link the resulting binary:
    ```sh
    cmake --build build
     ```
 3. **Test the kernel**
   Test your kernel using an emulator like QEMU:
    ```sh
    qemu-system-i386 -kernel kernel
    ```

## How It works
- **Assembly Code** (`Kernel.asm`): this file contains the low level kernel entry point, stack setup, and keyboard interrupt handling. It also includes the Multiboot header required by GRUB to load the kernel.managing screen output, and basic kernel operations.

- **C Codde**(`Lernel.asm`): Implements the Kernel's main functionality, including IDT and PIC initialization, handling keyboard input, managing screen output, and basic kernel operations.

- **Keyboard Handlig**: The kenel processes Keyboard interupts, updates the screen based on user input , and handles special keys like Enter. the other special keys will be added in the upcoming commits.

- **Screen output**: Uses direct video memory access to display text on the screen.

## Sources

[first source for printing just the string on the screen](https://arjunsreedharan.org/post/82710718100/kernels-101-lets-write-a-kernel)

[second source for the keyboard interruption handling](https://arjunsreedharan.org/post/99370248137/kernels-201-lets-write-a-kernel-with-keyboard)

[OS dev wiki](https://wiki.osdev.org/Expanded_Main_Page)

## Contributing

Contributions are welcome if you have suggestions, issues, or would like to contribute code, please submit issues or pull requests.







      
