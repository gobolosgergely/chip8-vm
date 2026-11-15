#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"
#include "graphics.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_timer.h>

static int poll_events(chip8 *vm, SDL_Event *event) {
        while (SDL_PollEvent(event)) {
        if (event->type == SDL_EVENT_QUIT) {
            return 1;
        }
        else if (event->type == SDL_EVENT_KEY_DOWN) {
            switch (event->key.key) {
            case SDLK_1:
                vm->keypad[0] = 1;
                break;
            case SDLK_2:
                vm->keypad[1] = 1;
                break;
            case SDLK_3:
                vm->keypad[2] = 1;
                break;
            case SDLK_4:
                vm->keypad[3] = 1;
                break;
            case SDLK_Q:
                vm->keypad[4] = 1;
                break;
            case SDLK_W:
                vm->keypad[5] = 1;
                break;
            case SDLK_E:
                vm->keypad[6] = 1;
                break;
            case SDLK_R:
                vm->keypad[7] = 1;
                break;
            case SDLK_A:
                vm->keypad[8] = 1;
                break;
            case SDLK_S:
                vm->keypad[9] = 1;
                break;
            case SDLK_D:
                vm->keypad[10] = 1;
                break;
            case SDLK_F:
                vm->keypad[11] = 1;
                break;
            case SDLK_Z:  // Z instead of Y for QWERTZ layout
                vm->keypad[12] = 1;
                break;
            case SDLK_X:
                vm->keypad[13] = 1;
                break;
            case SDLK_C:
                vm->keypad[14] = 1;
                break;
            case SDLK_V:
                vm->keypad[15] = 1;
                break;
            default:
                printf("Invalid key: %d", event->key.key);
                break;
            }
        }
        else if (event->type == SDL_EVENT_KEY_UP) {
            switch (event->key.key) {
            case SDLK_1:
                vm->keypad[0] = 0;
                break;
            case SDLK_2:
                vm->keypad[1] = 0;
                break;
            case SDLK_3:
                vm->keypad[2] = 0;
                break;
            case SDLK_4:
                vm->keypad[3] = 0;
                break;
            case SDLK_Q:
                vm->keypad[4] = 0;
                break;
            case SDLK_W:
                vm->keypad[5] = 0;
                break;
            case SDLK_E:
                vm->keypad[6] = 0;
                break;
            case SDLK_R:
                vm->keypad[7] = 0;
                break;
            case SDLK_A:
                vm->keypad[8] = 0;
                break;
            case SDLK_S:
                vm->keypad[9] = 0;
                break;
            case SDLK_D:
                vm->keypad[10] = 0;
                break;
            case SDLK_F:
                vm->keypad[11] = 0;
                break;
            case SDLK_Z: // Z instead of Y for QWERTZ layout
                vm->keypad[12] = 0;
                break;
            case SDLK_X:
                vm->keypad[13] = 0;
                break;
            case SDLK_C:
                vm->keypad[14] = 0;
                break;
            case SDLK_V:
                vm->keypad[15] = 0;
                break;
            default:
                break;
            }
        }

    }
    return 0;
}


int main(int argc, char *argv[]) {
    /* IBM logo in a window */
    chip8 vm;
    SDL_Event event;
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
        if (poll_events(&vm, &event) != 0) break;

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

        if (current_time - last_timer_time >= TIMER_TICK_NS) {
            update_timers(&vm);
            draw_display(&vm);
        }
        
    }





    graphics_clean();

    printf("CHIP-8 VM executed successfully.\n");
    return EXIT_SUCCESS;
}

