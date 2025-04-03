#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <cstdio>
#include "pecas.h"
#include "visual.h"
#include "audio.h"
#include "ia.h"

#ifndef LOCAL

#include <emscripten.h>
struct Context{
    int existe;
};

#endif

SDL_Event evento;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

const int WIDTH = 1000;
const int HEIGHT = 600;

int delay = 80;
bool rodar=true;

enum Modo{
    Player, Computador, FimDeJogo
};

enum Operacao{
    Continuar, Sair
};

enum Clique{
	Selecionar, Executar
};

std::vector<std::vector<SDL_Rect>> tabuleiro;
std::vector<std::vector<char>> pecas_tabuleiro;

std::vector<Lance> lances;
std::vector<Lance> lances_clicado;
std::vector<FEN> controle_lances;
std::vector<SDL_Texture*> imagens;

std::pair<SDL_Rect, SDL_Rect> ultimo_lance;
SDL_Rect lanceAtual;
SDL_Rect rectVazio;

bool inverter=false;
bool sentido_brancas=true;
int turno = White;
int lance_da_ia=0;
int ModoDeJogo=Computador;

//Acho que isso da para deixar local
int inicio_x;
int inicio_y;
int casas_por_linha;
int tam_quadrado;

int final_x;
int final_y;

int operacoes_clicar(int i, int j, std::vector<Lance>& lances_clicado, 
	std::vector<std::vector<char>>& pecas_tabuleiro, std::vector<FEN>& controle_lances, int& turno){
	static int clique = Selecionar;
	int peca = pecas_tabuleiro[i][j];

	bool cor_valida=true;

	if(turno == White){
		cor_valida = branco(peca);
	}
	else if(turno == Black){
		cor_valida = preto(peca);
	}

	printf("i: %d j: %d\n", i, j);  
	
	if(clique == Selecionar){
		printf("Selecionar\n");
		limpar_lances(lances_clicado);
		if(cor_valida && peca!=Vazio && peca!=Agua){
            lanceAtual = tabuleiro[i][j];
			lances_clicado = possiveis_lances_peca({i, j}, pecas_tabuleiro, &controle_lances);
			imprimir_lances(lances_clicado);
			clique = Executar;
		}
        else{
            lanceAtual = rectVazio;
        }        
	}
	else if(clique == Executar){
		printf("Executar\n");
		printf("total lances_clicado: %d\n", (int) lances_clicado.size());
		bool executado=false;
		for(int contador=0; contador<lances_clicado.size(); contador++){
			if(lances_clicado[contador].dst_i == i && lances_clicado[contador].dst_j == j){
				printf("Execuntando lance\n");
				executado = true;

				//int org_i = lances_clicado[contador].src_i;
				//int org_j = lances_clicado[contador].src_j;
				//controle_lances.push_back({lances_clicado[contador], pecas_tabuleiro[i][j], pecas_tabuleiro[org_i][org_j]});

				executar_lance(pecas_tabuleiro, lances_clicado[contador], &controle_lances);
                ultimo_lance.second = tabuleiro[i][j];
				if(turno==White) turno=Black;
				else if(turno==Black) turno=White;

				if(EstaEmCheque(pecas_tabuleiro, turno)){
					printf("Xeque\n");
				}
				break;
			}
		}

		limpar_lances(lances_clicado);
		clique = Selecionar;

		//Seleciona peca clicada se nao tiver executado um lance no ultimo clique
		if(executado==false){
			operacoes_clicar(i, j, lances_clicado, pecas_tabuleiro, controle_lances, turno);
		}
        else{
            ultimo_lance.first = lanceAtual;
            ultimo_lance.second = tabuleiro[i][j];
            lanceAtual = rectVazio;

            if(ModoDeJogo==Computador){
                int pseudo_turno = turno==White ? Black : White;
                turno = turno==White ? Black : White;
                return 1;
            }
		}
	}
    return 0;
}

void voltarLance(){
    if(ModoDeJogo==FimDeJogo) ModoDeJogo=Player;
    reverter_lance(controle_lances, pecas_tabuleiro, turno); 
    if(ModoDeJogo==Computador) reverter_lance(controle_lances, pecas_tabuleiro, turno);

    int tam = controle_lances.size();
    if(tam==0){
        ultimo_lance.first = ultimo_lance.second = rectVazio;
    }
    else{
        Lance* l = &controle_lances[tam-1].move;
        ultimo_lance.second = tabuleiro[l->dst_i][l->dst_j];

        //No caso de dois lances da mesma cor, como no En Passant
        if(tam-2>0 && branco(controle_lances[tam-1].peca) == branco(controle_lances[tam-2].peca)){
            l = &controle_lances[tam-2].move;
        }
        ultimo_lance.first = tabuleiro[l->src_i][l->src_j];
    }
}

void loopPrincipal(void* arg){
    if(lance_da_ia==1){
        int pseudo_turno= turno;
        bool jogoContinua = executar_lance_ia(controle_lances, pecas_tabuleiro, pseudo_turno);

        if(jogoContinua==false){
            ModoDeJogo=FimDeJogo;
        }
        lance_da_ia = 0;

        ultimo_lance.first = tabuleiro[marcarUltimoLance.src_i][marcarUltimoLance.src_j];
        ultimo_lance.second = tabuleiro[marcarUltimoLance.dst_i][marcarUltimoLance.dst_j];
    }

    while(SDL_PollEvent(&evento)){
        if(evento.type == SDL_QUIT){
            #ifndef LOCAL
                exit(0);
            #else
                rodar=false;
            #endif
        }
        else if(evento.type == SDL_KEYDOWN){
            switch(evento.key.keysym.sym){
                case SDLK_ESCAPE:
                    rodar=false;
                    break;
                case 'q': reproduzirAudio("assets/sound.wav"); break;
                case 'w': inverter_tabuleiro(pecas_tabuleiro, 3); imprimir_tabuleiro(pecas_tabuleiro); break;
                case 's': inverter_tabuleiro(pecas_tabuleiro, 4); imprimir_tabuleiro(pecas_tabuleiro); break;
                case 'a': inverter_tabuleiro(pecas_tabuleiro, 1); imprimir_tabuleiro(pecas_tabuleiro); break;
                case 'd': inverter_tabuleiro(pecas_tabuleiro, 2); imprimir_tabuleiro(pecas_tabuleiro); break;
                case 'l':
                    lances = todos_possiveis_lances(pecas_tabuleiro, turno, &controle_lances);
                    imprimir_lances(lances);
                    limpar_lances(lances);
                    printf("\n");
                    break;
                
                case 'k':
                    lances = todos_possiveis_lances(pecas_tabuleiro, turno, &controle_lances);
                    if(lances.size()!=0){
                        executar_lance(pecas_tabuleiro, lances[rand()%lances.size()], &controle_lances);
                    }

                    if(ModoDeJogo==Player){
                        if(turno == White) turno = Black;
                        else if(turno == Black) turno = White;
                    }
                    else if(ModoDeJogo==Computador){
                        lance_da_ia=1;
                    }
                    
                    imprimir_lances(lances);
                    limpar_lances(lances);
                    printf("\n");
                    break;
                
                case 'o': sentido_brancas = !sentido_brancas; inverter = !inverter; break;
                case 'z':
                    voltarLance();

                    break;
                case 'x': mostrar_FEN(controle_lances); break;
                case 'i': 
                    if(ModoDeJogo==Player){
                        ModoDeJogo=Computador;
                        printf("Jogando contra Maquina\n");
                    }
                    else{
                        ModoDeJogo=Player;
                        printf("Jogando contra Player\n");
                    }
                break;

                case 'c': ModoDeJogo=Computador; lance_da_ia=1; break;
                case 'v': ModoDeJogo=Computador; lance_da_ia=1; turno = turno==White? Black : White; break;
                case 'e':
                    printf("Valor posicao: %d\n", avaliacao_posicao(pecas_tabuleiro)); break;
                case 'r': printf("valor da posicao: %d\n", avaliacao_posicao(pecas_tabuleiro)); break;
            }
        }
        //Verificar se foi clicado no tabuleiro
        else if(evento.type == SDL_MOUSEBUTTONDOWN){
            int pos_x, pos_y;
            SDL_GetMouseState(&pos_x, &pos_y);

            int bool_x=0, bool_y=0;
            if(pos_x <= final_x && pos_x >= inicio_x) bool_x=1;
            if(pos_y <= final_y && pos_y >= inicio_y) bool_y=1;

            if(bool_x && bool_y){
                int j = (pos_x-inicio_x)/tam_quadrado;
                int i = (pos_y-inicio_y)/tam_quadrado;
                if(inverter) i = pecas_tabuleiro.size()-1-i;
                
                if (ModoDeJogo!=FimDeJogo) lance_da_ia = operacoes_clicar(i, j, lances_clicado, pecas_tabuleiro, controle_lances, turno);
            }
        }
    }    

    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_RenderClear(renderer);
    desenhar_tabuleiro(&renderer, tabuleiro, false);

    highlight_ultimo_lance(&renderer, ultimo_lance.first);
    highlight_ultimo_lance(&renderer, ultimo_lance.second);
    highlight_ultimo_lance(&renderer, lanceAtual, {255, 255, 0, 160});

    if(sentido_brancas){
        for(int i=0; i<pecas_tabuleiro.size(); i++){
            for(int j=0; j<pecas_tabuleiro[i].size(); j++){
                carregar_imagens(&renderer, imagens, tabuleiro[i][j], pecas_tabuleiro[i][j]);
            }
        }
    }
    else{
        for(int i=0; i<pecas_tabuleiro.size(); i++){
            for(int j=0; j<pecas_tabuleiro[i].size(); j++){
                carregar_imagens(&renderer, imagens, tabuleiro[i][j], pecas_tabuleiro[(int) pecas_tabuleiro[i].size()-1-i][j]);
            }
        }
    }

    if(lances_clicado.size()!=0){
        highlight_possiveis_lances(&renderer, lances_clicado, pecas_tabuleiro, tabuleiro, inverter);
    }
    
    SDL_RenderPresent(renderer);

    if(lance_da_ia==0) SDL_Delay(delay);
    else SDL_Delay(delay/4);
}

int main(int argc, char* argv[]){
    //Inicializando Janela
    SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); //Permite transparencia
    iniciar_imagens(&renderer, imagens);   

    inicio_x=80;
	inicio_y=10;
	casas_por_linha = 8;
	tam_quadrado = 60;

	final_x = inicio_x + (casas_por_linha)*tam_quadrado;
	final_y = inicio_y + (casas_por_linha)*tam_quadrado;

    tabuleiro = criar_tabuleiro(casas_por_linha, inicio_x, inicio_y, tam_quadrado);

    pecas_tabuleiro = {
		{BlackStaticRook, BlackKnight, BlackBishop, BlackQueen, BlackStaticKing, BlackBishop, BlackKnight, BlackStaticRook},
		{BlackStaticPawn, BlackStaticPawn, BlackStaticPawn, BlackStaticPawn, BlackStaticPawn, BlackStaticPawn, BlackStaticPawn, BlackStaticPawn},
		{Vazio, Vazio, Vazio, Vazio, Vazio, Vazio, Vazio, Vazio},
		{Vazio, Vazio, Vazio, Vazio, Vazio, Vazio, Vazio, Vazio},
		{Vazio, Vazio, Vazio, Vazio, Vazio, Vazio, Vazio, Vazio},
		{Vazio, Vazio, Vazio, Vazio, Vazio, Vazio, Vazio, Vazio},
		{WhiteStaticPawn, WhiteStaticPawn, WhiteStaticPawn, WhiteStaticPawn, WhiteStaticPawn, WhiteStaticPawn, WhiteStaticPawn, WhiteStaticPawn},
		{WhiteStaticRook, WhiteKnight, WhiteBishop, WhiteQueen, WhiteStaticKing, WhiteBishop, WhiteKnight, WhiteStaticRook},
	};

    printf("Seu turno\n");
    inicializarAudio();
    
    #ifndef LOCAL        
        Context context;
        emscripten_set_main_loop_arg(loopPrincipal, &context, -1, 1);
    #else
        int nada;
        while(rodar){
            loopPrincipal(&nada);
        }
    #endif

    //Liberando memoria
    finalizarAudio();
    destruir_imagens(imagens);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

    return 0;
}