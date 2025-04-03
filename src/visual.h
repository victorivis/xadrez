#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <cstdio>
#include "pecas.h"

void mostrar(SDL_Rect& rect);
void highlight_ultimo_lance(SDL_Renderer** renderer, SDL_Rect& posicao, SDL_Color cor={255, 255, 0, 120});
void highlight_proximo_lance(SDL_Renderer** renderer, SDL_Rect posicao, bool capturar);
void highlight_possiveis_lances(SDL_Renderer** renderer, std::vector<Lance>& lances, std::vector<std::vector<char>>& pecas_tabuleiro, std::vector<std::vector<SDL_Rect>>& quadrado_tabuleiro, bool inverter);

std::vector<std::vector<SDL_Rect>> criar_tabuleiro(int casas_por_linha, int inicio_x, int inicio_y, int tam_quadrado);
void desenhar_tabuleiro(SDL_Renderer** renderer, std::vector<std::vector<SDL_Rect>>& tabuleiro, bool padrao);
void mostrar_posicoes_tabuleiro(std::vector<std::vector<SDL_Rect>>& tabuleiro);

SDL_Texture* desenhar_peca(SDL_Renderer** renderer, char peca);
void iniciar_imagens(SDL_Renderer** renderer, std::vector<SDL_Texture*>& imagens);
void carregar_imagens(SDL_Renderer** renderer, std::vector<SDL_Texture*>& imagens, SDL_Rect& posicao, char peca);
void destruir_imagens(std::vector<SDL_Texture*>& imagens);