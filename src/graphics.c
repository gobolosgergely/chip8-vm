#include "graphics.h"
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <stdint.h>
#include <string.h>

int graphics_init(GraphicsContext *gc, uint8_t scale, uint16_t width, uint16_t height) {
    memset(gc, 0, sizeof(GraphicsContext));
    gc->display_scale = (scale > 0) ? scale : 15;
    gc->width = (width > 0) ? width : 64;
    gc->height = (height > 0) ? height : 32;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Error initializing SDL video: %s\n", SDL_GetError());  
        return -1;
    }
    if (!(gc->window = SDL_CreateWindow("CHIP-8", gc->width * gc->display_scale,
        gc->height * gc->display_scale, 0))) {
        SDL_Log("Error initializing SDL window: %s\n", SDL_GetError());
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        return -1;
    }
    if (!(gc->renderer = SDL_CreateRenderer(gc->window, NULL))) {
        SDL_Log("Error initializing SDL renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(gc->window);
        gc->window = NULL;
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        return -1;
    }

    return 0;
}

void graphics_clean(GraphicsContext *gc) {
    if (!gc || (!gc->renderer && !gc->window)) return;
    if (gc->renderer) {
        SDL_DestroyRenderer(gc->renderer);
        gc->renderer = NULL;
    }
    if (gc->window) {
        SDL_DestroyWindow(gc->window);
        gc->window = NULL;
    }
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void graphics_draw(GraphicsContext *gc, const uint8_t *display) {
    SDL_SetRenderDrawColor(gc->renderer,0,0,0,255);
    SDL_RenderClear(gc->renderer);
    SDL_SetRenderDrawColor(gc->renderer, 255,255,255,255);

    SDL_FRect rect;
    rect.w = gc->display_scale; rect.h = gc->display_scale;
    for (int row = 0; row < gc->height; row++) {
        for (int col = 0; col < gc->width; col++) {
            if (display[(row*gc->width) + col] == 1) {
                rect.x = col*gc->display_scale;
                rect.y = row*gc->display_scale;
                SDL_RenderFillRect(gc->renderer, &rect);
            }
        }
    }
    SDL_RenderPresent(gc->renderer);
}