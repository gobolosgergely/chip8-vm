#include "chip8.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

int init_chip8(chip8 *vm) {
    if (vm == NULL) {
        fprintf(stderr, "Error: NULL pointer passed to init_chip8.\n");
        return -1; 
    }

    memset(vm, 0, sizeof(chip8));

    static const uint8_t fontset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };

    memcpy(vm->memory, fontset, sizeof(fontset));

    vm->pc = 0x200; // Program counter starts at 0x200

    return 0;
}

int read_rom(const char *filename, chip8 *vm) {
    FILE *file = fopen(filename, "rb");

    if (file == NULL) {
        fprintf(stderr, "Error: Could not open ROM file %s\n", filename);
        return -1;
    }

    size_t max_size = sizeof(vm->memory) - vm->pc;
    size_t bytes_read = fread(vm->memory + vm->pc, sizeof(uint8_t), max_size, file);

    if (bytes_read == 0) {
        fprintf(stderr, "Error: ROM file is empty or could not be read.\n");
        fclose(file);
        return -1;
    }
    
    fclose(file);
    return 0;
}