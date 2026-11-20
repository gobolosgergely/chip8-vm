#ifndef AUDIO_H
#define AUDIO_H

#include <SDL3/SDL_audio.h>
#include <stdint.h>

typedef struct {
    SDL_AudioStream *stream;
    uint16_t sample_rate;
    uint32_t phase;
} AudioContext; 

int audio_init(AudioContext *ac);
void audio_play(AudioContext *ac, uint16_t duration_ms);
void audio_stop(AudioContext *ac);
void audio_clean(AudioContext *ac);

#endif