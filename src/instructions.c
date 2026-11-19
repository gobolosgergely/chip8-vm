#include "instructions.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
    // Execute display logic only if there was no DXYN call in the current frame yet.
    if (vm->vblank_ready == 1) {
        vm->vblank_ready = 0;

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
    else {
        vm->pc -= 2; // Blocks CPU until the next frame.
    }
    
}

void op_00EE(chip8 *vm) {
    vm->sp--;
    vm->pc = vm->stack[vm->sp];
}

void op_2NNN(chip8 *vm, uint16_t opcode) {
    uint16_t nnn = opcode & 0x0FFF;

    vm->stack[vm->sp] = vm->pc;
    vm->sp++;
    vm->pc = nnn;
}

void op_3XNN(chip8 *vm, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t vx = vm->registers[x];
    uint8_t nn = opcode & 0x00FF;

    if (vx == nn) {
        vm->pc += 2;
    }
}

void op_4XNN(chip8 *vm, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint16_t vx = vm->registers[x];
    uint16_t nn = opcode & 0x00FF;

    if (vx != nn) vm->pc += 2;
}

void op_5XY0(chip8 *vm, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint16_t vx = vm->registers[x];
    uint16_t vy = vm->registers[y];

    if (vx == vy) vm->pc += 2;
}

void op_9XY0(chip8 *vm, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint16_t vx = vm->registers[x];
    uint16_t vy = vm->registers[y];

    if (vx != vy) vm->pc += 2;
}

void op_8XY0(chip8 *vm, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    
    vm->registers[x] = vm->registers[y];
}

void op_8XY1(chip8 *vm, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    vm->registers[x] |= vm->registers[y];

    vm->registers[15] = 0;
}

void op_8XY2(chip8 *vm, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    vm->registers[x] &= vm->registers[y];
    vm->registers[15] = 0;
}

void op_8XY3(chip8 *vm, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    vm->registers[x] ^= vm->registers[y];
    vm->registers[15] = 0;
}

void op_8XY4(chip8 *vm, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    uint16_t temp = 0;
    temp = vm->registers[x] + vm->registers[y];
    vm->registers[x] = temp;

    if ((temp & 0x0F00) != 0) vm->registers[15] = 1;
    else vm->registers[15] = 0;
}

void op_8XY5(chip8 *vm, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    uint8_t flag = (vm->registers[x] >= vm->registers[y]) ? 1 : 0;

    vm->registers[x] -= vm->registers[y];
    vm->registers[15] = flag;
}

void op_8XY7(chip8 *vm, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    
    uint8_t flag = (vm->registers[y] >= vm->registers[x]) ? 1 : 0;

    vm->registers[x] = vm->registers[y] - vm->registers[x];
    vm->registers[15] = flag;
}

// Legacy implementation.
// (Modern one would shift VX in place and ignore the Y completely.)
void op_8XY6(chip8 *vm, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    vm->registers[x] = vm->registers[y];

    uint16_t lost_bit = vm->registers[x] & 0x01;
    
    vm->registers[x] >>= 1;
    vm->registers[15] = lost_bit;
}

// Legacy implementation.
// (Modern one would shift VX in place and ignore the Y completely.)
void op_8XYE(chip8 *vm, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    vm->registers[x] = vm->registers[y];

    uint16_t lost_bit = vm->registers[x] >> 7;
    
    vm->registers[x] <<= 1;
    vm->registers[15] = lost_bit;
}
// Might need to add more modern implementation
void op_BNNN(chip8 *vm, uint16_t opcode) {
    uint16_t nnn = opcode & 0x0FFF;
    vm->pc = nnn + vm->registers[0];
}

void op_CXNN(chip8 *vm, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t nn = opcode & 0x00FF;
    uint8_t random_number = rand();

    vm->registers[x] = nn & random_number;
}

void op_EX9E(chip8 *vm, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t key = vm->registers[x];
    if (vm->keypad[key] == 1) vm->pc += 2;
}

void op_EXA1(chip8 *vm, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t key = vm->registers[x];
    if (vm->keypad[key] == 0) vm->pc += 2;
}

void op_FX07(chip8 *vm, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    vm->registers[x] = vm->delay_timer;
}

void op_FX15(chip8 *vm, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    vm->delay_timer = vm->registers[x];
}

void op_FX18(chip8 *vm, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    vm->sound_timer = vm->registers[x];
}

// Implemented Amiga I overflow
void op_FX1E(chip8 *vm, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    vm->i += vm->registers[x];
    if ((vm->i & 0xF000) != 0) vm->memory[15] = 1;
}

void op_FX0A(chip8 *vm, uint16_t opcode) {    
    uint8_t x = (opcode & 0x0F00) >> 8;
    
    // Out of range index for keypad
    uint8_t key_pressed = 100; 

    for (uint8_t i = 0; i < 16; i++) {
        if (vm->keypad[i] == 1){
            key_pressed = i;
            break;
        }
    }
    // If no key is pressed, this function blocks
    if (key_pressed == 100) vm->pc -= 2;
    else vm->registers[x] = key_pressed;
}

void op_FX29(chip8 *vm, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;

    // We have characters from 0x0 to 0xF
    uint8_t addr = vm->registers[x] & 0x000F;
    
    /* Character size is 5 bytes.
       We don't need to add any offset,
       since fonts begin at 0x000 in the memory. */
    vm->i = addr * 5;
}

void op_FX33(chip8 *vm, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t vx = vm->registers[x];

    uint8_t digit1 = vx / 100;
    uint8_t digit2 = (vx / 10) % 10;
    uint8_t digit3 = vx % 10;

    /* Putting each digit of VX into three successive 
       memory addresses, starting with I. */
    vm->memory[vm->i] = digit1;
    vm->memory[vm->i + 1] = digit2;
    vm->memory[vm->i + 2] = digit3;
}

// Legacy implementation: I is incremented by x + 1
void op_FX55(chip8 *vm, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;
    
    for (uint8_t idx = 0; idx <= x; idx++) {
        vm->memory[vm->i + idx] = vm->registers[idx];
    }
    vm->i += x + 1;
}

// Legacy implementation: I is incremented by x + 1
void op_FX65(chip8 *vm, uint16_t opcode) {
    uint8_t x = (opcode & 0x0F00) >> 8;

    for (uint8_t idx = 0; idx <= x; idx++) {
        vm->registers[idx] = vm->memory[vm->i + idx];
    }
    vm->i += x + 1;
}
