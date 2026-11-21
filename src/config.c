#include "config.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#define CLOCK_RATE_MIN 1
#define CLOCK_RATE_MAX 100000
#define FPS_MIN 1
#define FPS_MAX 1000

static int string_to_uint32(const char *str, uint32_t min, uint32_t max, uint32_t *result) {
    char *end = NULL;
    errno = 0;

    long long val = strtoll(str, &end, 10);

    if (errno != 0 || str == end || *end != '\0'
            || val < (long long)min || val > (long long)max) {
        return -1;
    }

    *result = (uint32_t)val;
    return 0;
}

static const char *next_arg(int argc, char *argv[], int *i) {
    if (*i + 1 >= argc) {
        fprintf(stderr, "Missing value for %s\n", argv[*i]);
        return NULL;
    }
    return argv[++(*i)];
}

ParseStatus parse_args(int argc, char *argv[], Config *cfg) {
    snprintf(cfg->rom_path, sizeof(cfg->rom_path), "roms/ibm-logo-test");
    cfg->clock_rate = 700;
    cfg->frame_rate = 60;
    
    for (int i = 1; i < argc; i++) {

        if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--clock") == 0) {
            const char *val = next_arg(argc, argv, &i);
            if (val == NULL) return PARSE_ERROR;
            if ((string_to_uint32(val, CLOCK_RATE_MIN, CLOCK_RATE_MAX, &cfg->clock_rate)) != 0) {
                fprintf(stderr, "Invalid clock rate: %s Must be in [%d;%d].\n",
                    argv[i], CLOCK_RATE_MIN, CLOCK_RATE_MAX);
                return PARSE_ERROR;
            }
        }
        else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--fps") == 0) {
            const char *val = next_arg(argc, argv, &i);
            if (val == NULL) return PARSE_ERROR;
            if ((string_to_uint32(val, FPS_MIN, FPS_MAX, &cfg->frame_rate)) != 0) {
                fprintf(stderr, "Invalid frame rate: %s Must be in [%d;%d].\n", argv[i], FPS_MIN, FPS_MAX);
                return PARSE_ERROR;
            }
        }
        else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--rom") == 0) {
            const char *val = next_arg(argc, argv, &i);
            if (val == NULL) return PARSE_ERROR;
            snprintf(cfg->rom_path, sizeof(cfg->rom_path), "%s", val);
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printf("Usage: %s [ROM] [-r|--rom path] [-c|--clock Hz] [-f|--fps Hz]\n", argv[0]);
            return PARSE_HELP;
        }
        else if (argv[i][0] == '-') {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            return PARSE_ERROR;
        }
        else {
            snprintf(cfg->rom_path, sizeof(cfg->rom_path),"%s", argv[i]);
        }
    }

    return PARSE_OK;
}