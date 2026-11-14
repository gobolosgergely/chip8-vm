#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"
#include "graphics.h"
#include <SDL3/SDL_events.h>

int main() {
    /* IBM logo in a window */
    chip8 vm;
    uint16_t opcode;
    uint16_t previous_pc;

    if (init_chip8(&vm) != 0) return EXIT_FAILURE;
    if (read_rom("roms/ibm-logo.ch8", &vm) != 0) return EXIT_FAILURE; 
    if (graphics_init(20) != 0) return EXIT_FAILURE;

    while (1) {
        previous_pc = vm.pc;
        opcode = fetch(&vm);
        decode_execute(&vm, opcode);

        if (vm.pc == previous_pc) {
            printf("Program execution finished. Infinite loop at 0x%04X.\n", vm.pc);
            break;
        }
    }
    draw_display(&vm);

    SDL_Event event;
    bool is_running = true;
    while (is_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                is_running = false;
            }
        }
    }

    graphics_clean();

    printf("CHIP-8 VM executed successfully.\n");
    return 0;
}