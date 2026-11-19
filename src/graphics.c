#include "graphics.h"
#include <SDL3/SDL.h>
#include "chip8.h"
#include <stdio.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static uint8_t display_scale = 10;
 
int graphics_init(uint8_t scale) {
    if (scale > 0) {
        display_scale = scale;
    }

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Error initializing SDL video: %s\n", SDL_GetError());  
        return -1;
    }

    window = SDL_CreateWindow("CHIP-8", 64 * display_scale, 32 * display_scale, 0);
    renderer = SDL_CreateRenderer(window, NULL);

    return 0;
}

void graphics_clean() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void draw_display(chip8 *vm) {
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255,255,255,255);

    SDL_FRect rect;
    rect.w = display_scale; rect.h = display_scale;
    for (int row = 0; row < 32; row++) {
        for (int col = 0; col < 64; col++) {
            if (vm->display[(row*64) + col] == 1) {
                rect.x = col*display_scale;
                rect.y = row*display_scale;
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
    SDL_RenderPresent(renderer);
}