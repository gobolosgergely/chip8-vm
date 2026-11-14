#include "chip8.h"
#ifndef _INSTRUCTIONS_H
#define _INSTRUCTIONS_H

void op_6XNN(chip8 *vm, uint16_t opcode);
void op_7XNN(chip8 *vm, uint16_t opcode);
void op_ANNN(chip8 *vm, uint16_t opcode);
void op_1NNN(chip8 *vm, uint16_t opcode);
void op_00E0(chip8 *vm);
void op_DXYN(chip8 *vm, uint16_t opcode);
#endif