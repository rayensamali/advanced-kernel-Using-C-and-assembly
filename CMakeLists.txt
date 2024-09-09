cmake_minimum_required(VERSION 3.8)

# Configure NASM
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)
# We will link this directly using LD, not CC frontend
set(CMAKE_ASM_NASM_LINK_EXECUTABLE "ld <LINK_FLAGS> <OBJECTS> -o <TARGET>")

project(C_kernel LANGUAGES C ASM_NASM)

# Compile options just for C files
add_compile_options($<$<COMPILE_LANG_AND_ID:C,GNU>:-m32>)
add_compile_options($<$<COMPILE_LANG_AND_ID:C,GNU>:-fno-stack-protector>)
add_compile_options($<$<COMPILE_LANG_AND_ID:C,GNU>:-Wall>)


# Add kernel executable
add_executable(kernel kernel.asm kernel.c)

# Force NASM linking
set_target_properties(kernel PROPERTIES LINKER_LANGUAGE "ASM_NASM")

# Set target architecture and linker file used
target_link_options(kernel PRIVATE -m elf_i386 -T ${CMAKE_SOURCE_DIR}/link.ld)
