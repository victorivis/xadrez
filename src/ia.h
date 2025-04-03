#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <cstdio>
#include <climits>
#include "pecas.h"

extern bool exibirMinimax;
const int profundidade=2;

enum valor_pecas{
    peao=10, cavalo=30, bispo=30, torre=50, rainha=90, rei=9000
};

int retorna_valor(char peca);
int avaliacao_posicao(std::vector<std::vector<char>>& pecas_tabuleiro);
Lance minmax(std::vector<FEN>& controle_lances, std::vector<std::vector<char>>& pecas_tabuleiro, int& turno, bool max);
bool executar_lance_ia(std::vector<FEN>& controle_lances, std::vector<std::vector<char>>& pecas_tabuleiro, int& turno);