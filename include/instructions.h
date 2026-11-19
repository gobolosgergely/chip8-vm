#include "chip8.h"
#ifndef _INSTRUCTIONS_H
#define _INSTRUCTIONS_H

void op_00E0(chip8 *vm);
void op_00EE(chip8 *vm);
void op_1NNN(chip8 *vm, uint16_t opcode);
void op_2NNN(chip8 *vm, uint16_t opcode);
void op_3XNN(chip8 *vm, uint16_t opcode);
void op_4XNN(chip8 *vm, uint16_t opcode);
void op_5XY0(chip8 *vm, uint16_t opcode);
void op_6XNN(chip8 *vm, uint16_t opcode);
void op_7XNN(chip8 *vm, uint16_t opcode);
void op_8XY0(chip8 *vm, uint16_t opcode);
void op_8XY1(chip8 *vm, uint16_t opcode);
void op_8XY2(chip8 *vm, uint16_t opcode);
void op_8XY3(chip8 *vm, uint16_t opcode);
void op_8XY4(chip8 *vm, uint16_t opcode);
void op_8XY5(chip8 *vm, uint16_t opcode);
void op_8XY6(chip8 *vm, uint16_t opcode);
void op_8XY7(chip8 *vm, uint16_t opcode);
void op_8XYE(chip8 *vm, uint16_t opcode);
void op_9XY0(chip8 *vm, uint16_t opcode);
void op_ANNN(chip8 *vm, uint16_t opcode);
void op_BNNN(chip8 *vm, uint16_t opcode);
void op_CXNN(chip8 *vm, uint16_t opcode);
void op_DXYN(chip8 *vm, uint16_t opcode);
void op_EX9E(chip8 *vm, uint16_t opcode);
void op_EXA1(chip8 *vm, uint16_t opcode);
void op_FX07(chip8 *vm, uint16_t opcode);
void op_FX0A(chip8 *vm, uint16_t opcode);
void op_FX15(chip8 *vm, uint16_t opcode);
void op_FX18(chip8 *vm, uint16_t opcode);
void op_FX1E(chip8 *vm, uint16_t opcode);
void op_FX29(chip8 *vm, uint16_t opcode);
void op_FX33(chip8 *vm, uint16_t opcode);
void op_FX55(chip8 *vm, uint16_t opcode);
void op_FX65(chip8 *vm, uint16_t opcode);
/* Last ones finished: 8XY5 and 8XY7: Subtract
   19 out of 35 done. 
   16 more to go.. */
#endif