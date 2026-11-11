#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"

int main(int argc, char *argv[]) {

    (void)argc;
    (void)argv;

    chip8 vm;
    if (init_chip8(&vm) != 0) { return EXIT_FAILURE; }
    if (read_rom("roms/ibm-logo.ch8", &vm) != 0) { return EXIT_FAILURE; }

    printf("CHIP-8 VM executed successfully.\n");
    return 0;
}