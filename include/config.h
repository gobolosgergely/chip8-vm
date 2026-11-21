#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

typedef struct {
    uint32_t clock_rate;
    uint32_t frame_rate;
    char rom_path[256];
} Config;

typedef enum {
    PARSE_OK = 0,
    PARSE_HELP = 1,
    PARSE_ERROR = -1
} ParseStatus;

ParseStatus parse_args(int argc, char *argv[], Config *cfg);

#endif