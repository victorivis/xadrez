#pragma once
#include <SDL2/SDL.h>

extern SDL_AudioSpec spec;
extern Uint8 *audio_buffer;
extern Uint32 audio_length;
extern SDL_AudioStream *stream;

void inicializarAudio();
void reproduzirAudio(const char* caminhoArquivo);
void finalizarAudio();