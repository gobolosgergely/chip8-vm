#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_init.h>
#include <stdint.h>
#include "chip8.h"
#include "graphics.h"
#include "audio.h"
#include "config.h"

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
    Config cfg;

    ParseStatus status = parse_args(argc, argv, &cfg);
    if (status != PARSE_OK) {
        return (status == PARSE_HELP) ? EXIT_SUCCESS : EXIT_FAILURE;
    }
    if (init_chip8(&vm) != 0) return EXIT_FAILURE;
    if (read_rom(cfg.rom_path, &vm) != 0) return EXIT_FAILURE; 
    if (graphics_init(&gc, 20, 64, 32) != 0) return EXIT_FAILURE;
    if (audio_init(&ac) != 0) {
        graphics_clean(&gc);
        SDL_Quit();
        return EXIT_FAILURE;
    } 

    uint32_t cpu_cycle_per_frame = (cfg.clock_rate + (cfg.frame_rate / 2)) / cfg.frame_rate;
    const uint64_t timer_tick_ns = SDL_NS_PER_SECOND / cfg.frame_rate;
    uint64_t current_time;
    uint64_t next_frame_time = SDL_GetTicksNS();

    if (cpu_cycle_per_frame == 0) {
        cpu_cycle_per_frame = 1;
    }
    while (1) {
        if (poll_events(&vm) != 0) break;

        vm.vblank_ready = 1;
        for (uint32_t i = 0; i < cpu_cycle_per_frame; i++) {
            cpu_cycle(&vm);
        }

        update_timers(&vm);
        
        if (vm.sound_timer > 0) audio_play(&ac, 1000/cfg.frame_rate);       
        else audio_stop(&ac);

        graphics_draw(&gc, vm.display);

        next_frame_time += timer_tick_ns;
        current_time = SDL_GetTicksNS();
        if (current_time < next_frame_time) {
            SDL_DelayPrecise(next_frame_time - current_time);
        }
        else {
            next_frame_time = current_time;
        }
    }


    graphics_clean(&gc);
    audio_clean(&ac);
    SDL_Quit();
    printf("CHIP-8 VM executed successfully.\n");
    return EXIT_SUCCESS;
}

