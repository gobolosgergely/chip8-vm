#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>
#include <stdint.h>

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    uint8_t display_scale;
    uint16_t width;
    uint16_t height;
} GraphicsContext;

int graphics_init(GraphicsContext *gc, uint8_t scale, uint16_t width, uint16_t height);
void graphics_draw(GraphicsContext *gc, const uint8_t *display);
void graphics_clean(GraphicsContext *gc);

#endif