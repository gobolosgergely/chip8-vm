#include <stdint.h>
#ifndef _CHIP8_H
#define _CHIP8_H

typedef struct { 
    uint8_t memory[4096]; // 0x000 to 0xFFF
    uint8_t display[64 * 32];
    uint16_t stack[16];
    uint8_t sp; // stack pointer 
    uint16_t pc; // program counter
    uint16_t i; // index register
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t registers[16]; // general-purpose registers (V0 to VF)
    uint8_t keypad[16]; // keypad state
} chip8;

int init_chip8(chip8 *vm);

#endif