#include "audio.h"
#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <stdint.h>
#include <string.h>

#define FIX_ARRAY_LENGTH 512

int audio_init(AudioContext *ac) {
    memset(ac, 0, sizeof(AudioContext));
    if (!SDL_Init(SDL_INIT_AUDIO)) {
        SDL_Log("Error initializing SDL audio: %s\n", SDL_GetError());  
        return -1;
    }
    ac->sample_rate = 44100;
    SDL_AudioSpec spec = {
        .format = SDL_AUDIO_S16,
        .channels = 1,
        .freq = ac->sample_rate
    };

    ac->stream = SDL_OpenAudioDeviceStream(
        SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
        &spec,
        NULL,
        NULL
    );
    if (!ac->stream) {
        SDL_Log("SDL_OpenAudioDeviceStream failed: %s\n", SDL_GetError());
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        return -1;
    }

    SDL_ResumeAudioStreamDevice(ac->stream);

    return 0;
}

void audio_play(AudioContext *ac, uint16_t duration_ms) {
    int queued = SDL_GetAudioStreamQueued(ac->stream);
    if (queued < 0) {
        SDL_Log("SDL_GetAudioStreamQueued failed: %s\n", SDL_GetError());
        return;
    }
    uint32_t samples_in_queue = queued/2;
    uint64_t buffer_size = ((uint64_t)ac->sample_rate*(duration_ms + 50) / 1000);
    if (samples_in_queue >= buffer_size) {
        return;
    }
    uint64_t len = buffer_size - samples_in_queue;
    int16_t samples[FIX_ARRAY_LENGTH];
    uint32_t current_len;
    while (len > 0) {
        current_len = (len < FIX_ARRAY_LENGTH) ? len : FIX_ARRAY_LENGTH;
        len -= current_len;
        for (uint32_t i = 0; i < current_len; i++) {
            if (((ac->phase/50) % 2) == 0) samples[i] = 500;
            else samples[i] = -500;

            ac->phase++;
            if (ac->phase >= 100) ac->phase = 0;
        }
        SDL_PutAudioStreamData(ac->stream, samples,
            current_len * sizeof(int16_t));
    }
    
}

void audio_stop(AudioContext *ac) {
    SDL_ClearAudioStream(ac->stream);
    ac->phase = 0;
}
void audio_clean(AudioContext *ac) {
    if (!ac || !ac->stream) return;
    SDL_DestroyAudioStream(ac->stream);
    ac->stream = NULL;
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}