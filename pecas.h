#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <cstdio>

#define branco(peca) (peca>=WhitePawn && peca<=WhiteKing)
#define preto(peca) (peca>=BlackPawn && peca<=BlackKing)
#define rei_preto(peca) (peca==BlackKing || peca==BlackStaticKing)
#define rei_branco(peca) (peca==WhiteKing || peca==WhiteStaticKing)
#define origem_igual(lance, par) (lance.src_i == par.first && lance.src_j == par.second)
#define destino_igual(lance, par) (lance.dst_i == par.first && lance.dst_j == par.second)
#define par_igual(par1, par2) (par1.first == par2.first && par1.second == par2.second)

extern int debugLances;

struct Lance{
	char src_i;
	char src_j;
	char dst_i;
	char dst_j;
};

struct FEN{
	Lance move;
	char peca;
	char origem;
};

enum Cor{
	Black, White, Both, Neither
};

enum Pecas{
	Vazio=-2, Borda, Agua,
	BlackPawn, BlackStaticPawn, BlackKnight, BlackBishop, BlackRook, BlackStaticRook, BlackQueen, BlackStaticKing, BlackKing,
	WhitePawn, WhiteStaticPawn, WhiteKnight, WhiteBishop, WhiteRook, WhiteStaticRook, WhiteQueen, WhiteStaticKing, WhiteKing
};

enum Direcao{
    Norte, Sul, Leste, Oeste, Nordeste, Sudeste, Sudoeste, Noroeste, CimaDireita, CimaEsquerda, 
	BaixoDireita, BaixoEsquerda, DireitaCima, DireitaBaixo, EsquerdaCima, EsquerdaBaixo
};

enum Movimento{
	Linha=20, FinalLinha, Pulo, Capturar, Mover, MoverCapturar, Seguro, Roque, EnPassant
};

char calcular_destino(int direcao, std::pair<char, char> origem, std::vector<std::vector<char>>& tabuleiro, int num_movimentos=1);
void executar_lance(std::vector<std::vector<char>>& tabuleiro, Lance& lance, std::vector<FEN>* controle_lances=NULL);
bool aconteceu_EnPassant(std::vector<FEN>* controle_lances, char* peca=NULL, std::pair<char, char>* endereco_origem=NULL, char direcao=-1);
bool EstaEmCheque(std::vector<std::vector<char>>& pecas_tabuleiro, int turno);
bool movimento_permitido(int direcao, int tipo_lance, std::vector<std::vector<char>>& tabuleiro, std::pair<char, char> origem, int num_movimentos=1, std::vector<FEN>* controle_lances=NULL);
std::pair<char, char> par_mover_direcao(int direcao, std::pair<char, char>& origem, int num_movimentos=1);
Lance mover_direcao(int direcao, std::pair<char, char> origem, int num_movimentos=1);
void sequencia_lances(std::vector<char>& direcoes, std::pair<char, char> origem, int tipo_lance, std::vector<Lance>& lances, std::vector<std::vector<char>>& tabuleiro, int num_movimentos, std::vector<FEN>* controle_lances=NULL);
std::vector<Lance> possiveis_lances_peca(std::pair<char, char> origem, std::vector<std::vector<char>>& tabuleiro, std::vector<FEN>* controle_lances=NULL);
std::vector<Lance> todos_possiveis_lances(std::vector<std::vector<char>>& tabuleiro, int cor, std::vector<FEN>* controle_lances=NULL);
bool alcanca_destino(std::vector<std::vector<char>>& pecas_tabuleiro, std::pair<char, char>& destino, int turno);

void reverter_lance(std::vector<FEN>& controle_lances, std::vector<std::vector<char>>& pecas_tabuleiro, int& turno);
void mostrar_FEN(std::vector<FEN>& controle_lances);
void imprimir_tabuleiro(std::vector<std::vector<char>>& tabuleiro);
void limpar_lances(std::vector<Lance>& lances);
void imprimir_lances(std::vector<Lance>& lances);

void swap(char* a, char* b);
void inverter_tabuleiro(std::vector<std::vector<char>>& tabuleiro, int eixo);