#include "visual.h"

void highlight_proximo_lance(SDL_Renderer** renderer, SDL_Rect posicao, bool capturar){
	if(capturar){
		SDL_SetRenderDrawColor(*renderer, 255, 0, 0, 180);
	}
	else{
		SDL_SetRenderDrawColor(*renderer, 180, 180, 180, 180);
	}

	posicao.x += posicao.w/4;
	posicao.y += posicao.h/4;
	posicao.w/=2;
	posicao.h/=2;

	SDL_RenderFillRect(*renderer, &posicao);
}

void highlight_possiveis_lances(SDL_Renderer** renderer, std::vector<Lance>& lances, std::vector<std::vector<char>>& pecas_tabuleiro, std::vector<std::vector<SDL_Rect>>& quadrado_tabuleiro, bool inverter){
	for(int i=0; i<lances.size(); i++){
		if(!inverter){
			char peca = pecas_tabuleiro[lances[i].dst_i][lances[i].dst_j];
			highlight_proximo_lance(renderer, quadrado_tabuleiro[lances[i].dst_i][lances[i].dst_j], peca!=Vazio && peca!=Agua);
		}
		else{
			char peca = pecas_tabuleiro[lances[i].dst_i][lances[i].dst_j];
			highlight_proximo_lance(renderer, quadrado_tabuleiro[pecas_tabuleiro.size()-1-lances[i].dst_i][lances[i].dst_j], peca!=Vazio && peca!=Agua);
		}
	}
}

std::vector<std::vector<SDL_Rect>> criar_tabuleiro(int casas_por_linha, int inicio_x, int inicio_y, int tam_quadrado){
	
	std::vector<std::vector<SDL_Rect>> tabuleiro(casas_por_linha);
	for(int j=0; j<casas_por_linha; j++){
		for(int i=0; i<casas_por_linha; i++){
			tabuleiro[j].push_back({i*tam_quadrado+inicio_x, j*tam_quadrado+inicio_y, tam_quadrado, tam_quadrado});
		}
	}

	return tabuleiro;
}

void desenhar_tabuleiro(SDL_Renderer** renderer, std::vector<std::vector<SDL_Rect>>& tabuleiro, bool topoEsquerdoBranco){
	for(int i=0; i<tabuleiro.size(); i++){
		for(int j=0; j<tabuleiro[i].size(); j++){
			topoEsquerdoBranco ? SDL_SetRenderDrawColor(*renderer, 0, 180, 0, 255) : SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(*renderer, &(tabuleiro[i][j]));
			topoEsquerdoBranco = !topoEsquerdoBranco;
		}
		if(tabuleiro[i].size()%2==0) topoEsquerdoBranco = !topoEsquerdoBranco;
	}
}

void mostrar_posicoes_tabuleiro(std::vector<std::vector<SDL_Rect>>& tabuleiro){
	for(int i=0; i<tabuleiro.size(); i++){
		for(int j=0; j<tabuleiro[i].size(); j++){
			printf("x%d: %d y%d: %d  ", i, tabuleiro[i][j].x, j, tabuleiro[i][j].y);
		}
		printf("\n");
	}
}

SDL_Texture* desenhar_peca(SDL_Renderer** renderer, char peca){
    std::string caminho = "assets/pecasBMP/";
    std::string extensao = ".bmp";
    std::string nome_peca;

    switch(peca){
		case Vazio:           nome_peca = "Vazio";           break;
        case Agua:            nome_peca = "Agua";            break;
        case Borda:           nome_peca = "Borda";           break;

		case BlackStaticPawn:
		case BlackPawn:       nome_peca = "BlackPawn";       break;
        case BlackKnight:     nome_peca = "BlackKnight";     break;
        case BlackBishop:     nome_peca = "BlackBishop";     break;
		case BlackStaticRook:
        case BlackRook:       nome_peca = "BlackRook";       break;
        case BlackQueen:      nome_peca = "BlackQueen";      break;
		case BlackStaticKing:
        case BlackKing:       nome_peca = "BlackKing";       break;

		case WhiteStaticPawn:
        case WhitePawn:       nome_peca = "WhitePawn";       break;
        case WhiteKnight:     nome_peca = "WhiteKnight";     break;
        case WhiteBishop:     nome_peca = "WhiteBishop";     break;
		case WhiteStaticRook:
        case WhiteRook:       nome_peca = "WhiteRook";       break;
        case WhiteQueen:      nome_peca = "WhiteQueen";      break;
		case WhiteStaticKing:
        case WhiteKing:       nome_peca = "WhiteKing";       break;
		default:
			printf("Erro ao desenhar peca: %d\n", peca); exit(1);
    }

	if(nome_peca.compare("Vazio")!=0 && nome_peca.compare("Borda")!=0){
		std::string caminho_completo = caminho + nome_peca + extensao;
		SDL_Surface* peca_sur = SDL_LoadBMP(caminho_completo.c_str());
		SDL_Texture* peca_tex = SDL_CreateTextureFromSurface(*renderer, peca_sur);
		//SDL_RenderCopy(*renderer, peca_tex, NULL, &retangulo);

		SDL_FreeSurface(peca_sur);
		//SDL_DestroyTexture(peca_tex);
		return peca_tex;
	}
	else{
		return nullptr;
	}
}

void iniciar_imagens(SDL_Renderer** renderer, std::vector<SDL_Texture*>& imagens){
	for(char i=Agua; i<=WhiteKing; i++){
		imagens.push_back(desenhar_peca(renderer, i));
	}
}

void carregar_imagens(SDL_Renderer** renderer, std::vector<SDL_Texture*>& imagens, SDL_Rect& posicao, char peca){
	if(peca>=0 && peca<imagens.size()){
		SDL_RenderCopy(*renderer, imagens[peca], NULL, &posicao);
	}
}

void destruir_imagens(std::vector<SDL_Texture*>& imagens){
	int total = (int) imagens.size()-1;
	for(int i=total; i>=0; i--){
		SDL_DestroyTexture(imagens[i]);
		imagens.pop_back();
	}
}
