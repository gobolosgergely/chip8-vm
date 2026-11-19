#include <SDL3/SDL.h>
#include <stdio.h>

static SDL_AudioStream *stream;
static uint16_t sample_rate = 44100;

int audio_init() {
    if (!SDL_Init(SDL_INIT_AUDIO)) {
        printf("Error initializing SDL audio: %s\n", SDL_GetError());  
        return -1;
    }
    SDL_AudioSpec spec = {
        .format = SDL_AUDIO_S16,
        .channels = 1,
        .freq = sample_rate
    };

    stream = SDL_OpenAudioDeviceStream(
        SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
        &spec,
        NULL,
        NULL
    );
    if (!stream) {
        SDL_Log("SDL_OpenAudioDeviceStream failed: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_ResumeAudioStreamDevice(stream);

    return 0;
}

void play_audio(uint8_t timer_frequency) {
    uint16_t len = sample_rate/timer_frequency;
    int16_t samples[len];
    static uint32_t phase = 0;
    for (uint32_t i = 0; i < len; i++) {
        if (((phase/50) % 2) == 0) samples[i] = 1500;
        else samples[i] = -1500;

        phase++;
        if (phase >= 100) phase = 0;
    }

    SDL_PutAudioStreamData(stream, samples, sizeof(samples));
}

void audio_clean() {
    SDL_DestroyAudioStream(stream);
    SDL_Quit();
}