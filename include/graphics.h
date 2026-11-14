#include "chip8.h"

#ifndef _GRAPHICS_H
#define _GRAPHICS_H

int graphics_init(uint8_t scale);
void draw_display(chip8 *vm);
void graphics_clean();

#endif