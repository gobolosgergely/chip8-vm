#include "instructions.h"
#include <string.h>
#include <stdio.h>

void op_6XNN(chip8 *vm, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;
    uint8_t nn = opcode & 0x00FF;

    vm->registers[vx] = nn;
}

void op_7XNN(chip8 *vm, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;
    uint8_t nn = opcode & 0x00FF;

    vm->registers[vx] += nn;
}

void op_ANNN(chip8 *vm, uint16_t opcode) {
    uint16_t nnn = opcode & 0x0FFF;

    vm->i = nnn;
}

void op_1NNN(chip8 *vm, uint16_t opcode) {
    uint16_t nnn = opcode & 0x0FFF;

    vm->pc = nnn;
}

void op_00E0(chip8 *vm) {
    memset(vm->display, 0, sizeof(vm->display));
}

void op_DXYN(chip8 *vm, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;
    uint8_t vy = (opcode & 0x00F0) >> 4;
    uint8_t n = (opcode & 0x000F); // 8 wide, n tall sprite

    uint8_t x = vm->registers[vx] % 64; // mod (width of display)
    uint8_t y = vm->registers[vy] % 32; // mod (height of display)

    uint8_t pixel_was_turned_off = 0;

    uint16_t addr = vm->i;
    for (int row = 0; row < n; row++) {
        uint8_t current_row = vm->memory[addr + row];
        
        for (int col = 0; col < 8; col++) {
            if ((x + col < 64) && (y + row < 32)) {
                uint16_t display_index = (y + row)*64 + x + col;
                uint8_t current_pixel = (current_row & (0x80 >> col)) != 0;

                if (current_pixel == 1) {
                    if (vm->display[display_index] == 1) {
                        pixel_was_turned_off = 1;
                    }
                    vm->display[display_index] ^= 1;
                } 
            }      
        }
    }
    vm->registers[15] = pixel_was_turned_off;
}