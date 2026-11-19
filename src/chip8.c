#include "chip8.h"
#include "instructions.h"
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

uint16_t fetch(chip8 *vm) {
    uint16_t opcode = (vm->memory[vm->pc] << 8) | vm->memory[vm->pc + 1];
    vm->pc += 2;
    return opcode;
}

int decode_execute(chip8 *vm, uint16_t opcode) {
    uint8_t nibble1 = opcode >> 12;
    uint8_t nibble4 = opcode & 0x000F;
    uint8_t second_byte = opcode;
    switch (nibble1) {
        case 0x0:
            if (opcode == 0x00E0) {
                op_00E0(vm);
            }
            else if (opcode == 0x00EE) {
                op_00EE(vm);
            }
            break;
        case 0x1:
            op_1NNN(vm, opcode);
            break;
        case 0x2:
            op_2NNN(vm, opcode);
            break;
        case 0x3:
            op_3XNN(vm, opcode);
            break;
        case 0x4:
            op_4XNN(vm, opcode);
            break;
        case 0x5:
            op_5XY0(vm, opcode);
            break;
        case 0x6:
            op_6XNN(vm, opcode);
            break;
        case 0x7:
            op_7XNN(vm, opcode);
            break;
        case 0x8:
            switch (nibble4) {
                case 0x0:
                    op_8XY0(vm, opcode);
                    break;
                case 0x1:
                    op_8XY1(vm, opcode);
                    break;
                case 0x2:
                    op_8XY2(vm, opcode);
                    break;
                case 0x3:
                    op_8XY3(vm, opcode);
                    break;
                case 0x4:
                    op_8XY4(vm, opcode);
                    break;
                case 0x5:
                    op_8XY5(vm, opcode);
                    break;
                case 0x6:
                    op_8XY6(vm, opcode);
                    break;
                case 0x7:
                    op_8XY7(vm, opcode);
                    break;
                case 0xE:
                    op_8XYE(vm, opcode);
                    break;
            }
            break;
        case 0x9:
            op_9XY0(vm, opcode);
            break;
        case 0xA:
            op_ANNN(vm, opcode);
            break;
        case 0xB:
            op_BNNN(vm, opcode);
            break;
        case 0xC:
            op_CXNN(vm, opcode);
            break;
        case 0xD:
            op_DXYN(vm, opcode);
            break;
        case 0xE:
            if (second_byte == 0x9E) {
                op_EX9E(vm, opcode);
            }
            else if (second_byte == 0xA1) {
                op_EXA1(vm, opcode);
            }
            break;
        case 0xF:
            switch (second_byte) {
                case 0x07:
                    op_FX07(vm, opcode);
                    break;
                case 0x0A:
                    op_FX0A(vm, opcode);
                    break;
                case 0x15:
                    op_FX15(vm, opcode);
                    break;
                case 0x18:
                    op_FX18(vm, opcode);
                    break;
                case 0x1E:
                    op_FX1E(vm, opcode);
                    break;
                case 0x29:
                    op_FX29(vm, opcode);
                    break;
                case 0x33:
                    op_FX33(vm, opcode);
                    break;
                case 0x55:
                    op_FX55(vm, opcode);
                    break;
                case 0x65:
                    op_FX65(vm, opcode);
                    break;
            }
            break;
    }

    return 0;
}

void cpu_cycle(chip8 *vm) {
    uint16_t opcode = fetch(vm);
    printf("PC: 0x%04X | Opcode: 0x%04X\n", vm->pc - 2, opcode);
    decode_execute(vm, opcode);
    
}

void update_timers(chip8 *vm) {
    if (vm->delay_timer > 0) vm->delay_timer--;
    if (vm->sound_timer > 0) vm->sound_timer--;
}
