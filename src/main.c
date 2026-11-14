#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"
#include "graphics.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_timer.h>


int main(int argc, char *argv[]) {
    /* IBM logo in a window */
    chip8 vm;
    uint16_t previous_pc;
    uint16_t clock_rate;    

    if (argc == 1) {
        clock_rate = 700;
    }
    else if (argc == 2) {
        clock_rate = atoi(argv[1]);
        // If atoi cannot convert the input, it returns 0.
        if (clock_rate == 0) clock_rate = 700; 
    }
    else {
        printf("Invalid argument!\n");
        printf("Use: %s [optional: clock rate]\n", __FILE__);
        return EXIT_FAILURE;
    }
    const uint64_t CPU_TICK_NS = 1000000000 / clock_rate;  // 1.000.000.000
    const uint64_t TIMER_TICK_NS = 1000000000 / 60; // 1.000.000.000
    uint64_t last_cpu_time = SDL_GetTicksNS();
    uint64_t last_timer_time = last_cpu_time;
    uint64_t current_time;

    if (init_chip8(&vm) != 0) return EXIT_FAILURE;
    if (read_rom("roms/ibm-logo.ch8", &vm) != 0) return EXIT_FAILURE; 
    if (graphics_init(20) != 0) return EXIT_FAILURE;

    while (1) {
        current_time = SDL_GetTicksNS();
        if (current_time - last_cpu_time >= CPU_TICK_NS) {
            last_cpu_time = current_time;
            previous_pc = vm.pc;

            cpu_cycle(&vm);

            if (vm.pc == previous_pc) {
                printf("Program execution finished. Infinite loop at 0x%04X.\n", vm.pc);
                break;
            }
        }

        if (current_time - last_timer_time >= TIMER_TICK_NS) update_timers(&vm);
        
        draw_display(&vm);
    }

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
    return EXIT_SUCCESS;
}