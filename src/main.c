#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_init.h>
#include <stdint.h>
#include "chip8.h"
#include "graphics.h"
#include "audio.h"

static const SDL_Scancode keymap[16] = {
    SDL_SCANCODE_X,
    SDL_SCANCODE_1,
    SDL_SCANCODE_2,
    SDL_SCANCODE_3,
    SDL_SCANCODE_Q,
    SDL_SCANCODE_W,
    SDL_SCANCODE_E,
    SDL_SCANCODE_A,
    SDL_SCANCODE_S,
    SDL_SCANCODE_D,
    SDL_SCANCODE_Z, // On hungarian QWERTZ layout it is the Y key.
    SDL_SCANCODE_C,
    SDL_SCANCODE_4,
    SDL_SCANCODE_R,
    SDL_SCANCODE_F,
    SDL_SCANCODE_V
};

static int poll_events(chip8 *vm) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            return 1;
        }
        if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
            uint8_t pressed = (event.type == SDL_EVENT_KEY_DOWN) ? 1 : 0;
            for (int i = 0; i < 16; i++) {
                if (event.key.scancode == keymap[i]) {
                    vm->keypad[i] = pressed;
                    break;
                }
            }
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    chip8 vm;
    GraphicsContext gc;
    AudioContext ac;
    uint16_t clock_rate = 700;
    const uint8_t timer_frequency = 60;
    char rom_path[256] = "roms/ibm-logo-test";

    if (argc == 1) ; 
    else if (argc == 2) {
        snprintf(rom_path, 256, "roms/%s", argv[1]);
    }
    else if (argc == 3) {
        snprintf(rom_path, 256, "roms/%s", argv[1]);

        clock_rate = atoi(argv[2]);
        // If atoi cannot convert the input, it returns 0.
        if (clock_rate == 0) clock_rate = 700;         
    }
    else {
        printf("Invalid argument!\n");
        printf("Use: %s [ROM] [optional: clock rate]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (init_chip8(&vm) != 0) return EXIT_FAILURE;
    if (read_rom(rom_path, &vm) != 0) return EXIT_FAILURE; 
    if (graphics_init(&gc, 20, 64, 32) != 0) return EXIT_FAILURE;
    if (audio_init(&ac) != 0) {
        graphics_clean(&gc);
        SDL_Quit();
        return EXIT_FAILURE;
    } 

    const uint64_t CPU_TICK_NS = 1000000000 / clock_rate;  // 1.000.000.000
    const uint64_t TIMER_TICK_NS = 1000000000 / timer_frequency; // 1.000.000.000
    uint64_t current_time;
    uint64_t last_cpu_time = SDL_GetTicksNS();
    uint64_t last_timer_time = last_cpu_time;

    while (1) {
        if (poll_events(&vm) != 0) break;

        current_time = SDL_GetTicksNS();
        if (current_time - last_cpu_time >= CPU_TICK_NS) {
            last_cpu_time = current_time;
            cpu_cycle(&vm);
        }

        if (current_time - last_timer_time >= TIMER_TICK_NS) {
            last_timer_time = current_time;
            vm.vblank_ready = 1;
            update_timers(&vm);
            
            if (vm.sound_timer > 0) audio_play(&ac, 1000/timer_frequency);       
            else audio_stop(&ac);

            graphics_draw(&gc, vm.display);
        }
    }


    graphics_clean(&gc);
    audio_clean(&ac);
    SDL_Quit();
    printf("CHIP-8 VM executed successfully.\n");
    return EXIT_SUCCESS;
}

