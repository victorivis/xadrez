#include "audio.h"
#include <cstdio>

SDL_AudioSpec spec;
Uint8 *audio_buffer;
Uint32 audio_length;
SDL_AudioStream *stream;
bool isPlayingAudio=false;

void audio_callback(void *userdata, Uint8 *stream, int len) {
    if (audio_length == 0){
        SDL_PauseAudio(1);
        isPlayingAudio=false;
        return;
    }

    len = (len > audio_length ? audio_length : len);
    SDL_memcpy(stream, audio_buffer, len);
    audio_buffer += len;
    audio_length -= len;
}

void inicializarAudio(){
    if(SDL_LoadWAV("assets/sound.wav", &spec, &audio_buffer, &audio_length) == NULL){
        printf("Deu ruim!");
    }

    stream = SDL_NewAudioStream(AUDIO_S16, 1, 22050, AUDIO_F32, 2, 48000);
    if (stream == NULL) {
        printf("Erro ao criar stream: %s\n", SDL_GetError());
    }

    spec.callback = audio_callback;
    spec.userdata = NULL;

    if (SDL_OpenAudio(&spec, NULL) < 0) {
        printf("Erro ao abrir o dispositivo de áudio: %s\n", SDL_GetError());
    }
    SDL_PauseAudio(1);
}

void reproduzirAudio(const char* caminhoArquivo){
    if(isPlayingAudio==false){
        SDL_FreeWAV(audio_buffer);
        audio_buffer = NULL;
        audio_length = 0;
        
        if (SDL_LoadWAV(caminhoArquivo, &spec, &audio_buffer, &audio_length) != NULL) {
            SDL_PauseAudio(0);
            isPlayingAudio=true;
        }
        else{
            printf("Erro ao carregar o áudio: %s\n", SDL_GetError());
        }
    }    
}

void finalizarAudio(){
    SDL_FreeWAV(audio_buffer);
    SDL_FreeAudioStream(stream);
}