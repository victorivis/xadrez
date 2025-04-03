#include "pecas.h"

int debugLances=false;
Lance marcarUltimoLance;

char calcular_destino(int direcao, std::pair<char, char> origem, std::vector<std::vector<char>>& tabuleiro, int num_movimentos){
	switch(direcao){
		case Norte:
			return tabuleiro[origem.first-num_movimentos][origem.second];

		case Sul:
			return tabuleiro[origem.first+num_movimentos][origem.second];

		case Leste:
			return tabuleiro[origem.first][origem.second+num_movimentos];

		case Oeste:
			return tabuleiro[origem.first][origem.second-num_movimentos];

		case Nordeste:
			return tabuleiro[origem.first-num_movimentos][origem.second+num_movimentos];

		case Noroeste:
			return tabuleiro[origem.first-num_movimentos][origem.second-num_movimentos];

		case Sudeste:
			return tabuleiro[origem.first+num_movimentos][origem.second+num_movimentos];

		case Sudoeste:
			return tabuleiro[origem.first+num_movimentos][origem.second-num_movimentos];

		case CimaDireita:
			return tabuleiro[origem.first-2][origem.second+1];

		case CimaEsquerda:
			return tabuleiro[origem.first-2][origem.second-1];

		case BaixoDireita:
			return tabuleiro[origem.first+2][origem.second+1];

		case BaixoEsquerda:
			return tabuleiro[origem.first+2][origem.second-1];

		case DireitaCima:
			return tabuleiro[origem.first-1][origem.second+2];

		case DireitaBaixo:
			return tabuleiro[origem.first+1][origem.second+2];

		case EsquerdaCima:
			return tabuleiro[origem.first-1][origem.second-2];

		case EsquerdaBaixo:
			return tabuleiro[origem.first+1][origem.second-2];
		
		default:
			if(debugLances) printf("Erro! direcao impossivel: %d\n", direcao);
			exit(1);
	}

	return -1;
}

void executar_lance(std::vector<std::vector<char>>& tabuleiro, Lance& lance, std::vector<FEN>* controle_lances){
	//Previne de acessar memoria indevida
	if(lance.src_i<0 || lance.src_i>=tabuleiro.size()) if(debugLances) printf("%d %d %d %d\n", lance.src_i, lance.src_j, lance.dst_i, lance.dst_j), exit(404);
	if(lance.src_j<0 || lance.src_j>=tabuleiro.size()) if(debugLances) printf("%d %d %d %d\n", lance.src_i, lance.src_j, lance.dst_i, lance.dst_j), exit(404);
	if(lance.dst_i<0 || lance.dst_i>=tabuleiro.size()) if(debugLances) printf("%d %d %d %d\n", lance.src_i, lance.src_j, lance.dst_i, lance.dst_j), exit(404);
	if(lance.dst_j<0 || lance.dst_j>=tabuleiro.size()) if(debugLances) printf("%d %d %d %d\n", lance.src_i, lance.src_j, lance.dst_i, lance.dst_j), exit(404);
	
	char origem = tabuleiro[lance.src_i][lance.src_j];

	int roque=0;

	if(controle_lances != NULL && (*controle_lances).size()>1){
		int ultima_posicao = (int) (*controle_lances).size()-1;
		char ultima_peca = (*controle_lances)[ultima_posicao].origem;
		if((origem==WhitePawn && ultima_peca==BlackStaticPawn) || (origem==BlackPawn && ultima_peca==WhiteStaticPawn)){
			char i = lance.dst_i;
			char j = lance.dst_j;

			Lance ultimo_lance = (*controle_lances)[ultima_posicao].move;
			//Peao esta do lado
			if(ultimo_lance.src_j == j && ultimo_lance.dst_j == j){
				//Se moveu duas casas
				if(abs(ultimo_lance.src_i-i)==1 && abs(ultimo_lance.dst_i-i)==1){
					Lance mover_lado={lance.src_i, lance.src_j, lance.src_i, j};
					executar_lance(tabuleiro, mover_lado, controle_lances);
					lance.src_j = j;

					if(debugLances) printf("Aconteceu En Passant\n");
				}
			}	
		}
	}

	//Salva lance se for passado um vetor
	if(controle_lances != NULL){
		(*controle_lances).push_back({lance, tabuleiro[lance.dst_i][lance.dst_j], tabuleiro[lance.src_i][lance.src_j]});
	}

	//Detecta se a peca ja se moveu alguma vez
	switch(origem){
		case BlackStaticPawn: tabuleiro[lance.src_i][lance.src_j] = BlackPawn; break;
		case WhiteStaticPawn: tabuleiro[lance.src_i][lance.src_j] = WhitePawn; break;
		case BlackStaticRook: tabuleiro[lance.src_i][lance.src_j] = BlackRook; break;
		case WhiteStaticRook: tabuleiro[lance.src_i][lance.src_j] = WhiteRook; break;

		case BlackStaticKing:
			if(lance.src_j-lance.dst_j==2) roque=Oeste;
			else if(lance.src_j-lance.dst_j==-2) roque=Leste;
			tabuleiro[lance.src_i][lance.src_j] = BlackKing;
			break;

		case WhiteStaticKing:
			if(lance.src_j-lance.dst_j==2) roque=Oeste;
			else if(lance.src_j-lance.dst_j==-2) roque=Leste;
			tabuleiro[lance.src_i][lance.src_j] = WhiteKing;
			break;
	}

	//Propriamente faz o movimento
	tabuleiro[lance.dst_i][lance.dst_j] = tabuleiro[lance.src_i][lance.src_j];
    tabuleiro[lance.src_i][lance.src_j] = Vazio;

	
	if(roque==Oeste){
		std::pair<char, char> origem_rei = {lance.src_i, lance.src_j};
		int contador=1;
		char destino = calcular_destino(Oeste, origem_rei, tabuleiro, contador);
		while((destino!=BlackStaticRook && destino != WhiteStaticRook) && contador<tabuleiro.size()){
			contador++;
			destino = calcular_destino(Oeste, origem_rei, tabuleiro, contador);
		}
		std::pair<char, char> origem_torre = par_mover_direcao(Oeste, origem_rei, contador);
		std::pair<char, char> destino_torre = par_mover_direcao(Oeste, origem_rei, 1);

		Lance torre = {origem_torre.first, origem_torre.second, destino_torre.first, destino_torre.second};
		executar_lance(tabuleiro, torre, controle_lances);
		if(debugLances) printf("Rei Rocou! Esquerda\n");
	}
	else if(roque==Leste){
		std::pair<char, char> origem_rei = {lance.src_i, lance.src_j};
		int contador=1;
		char destino = calcular_destino(Leste, origem_rei, tabuleiro, contador);
		while((destino!=BlackStaticRook && destino != WhiteStaticRook) && contador<tabuleiro.size()){
			contador++;
			destino = calcular_destino(Leste, origem_rei, tabuleiro, contador);
		}
		std::pair<char, char> origem_torre = par_mover_direcao(Leste, origem_rei, contador);
		std::pair<char, char> destino_torre = par_mover_direcao(Leste, origem_rei, 1);

		Lance torre = {origem_torre.first, origem_torre.second, destino_torre.first, destino_torre.second};
		executar_lance(tabuleiro, torre, controle_lances);
		if(debugLances) printf("Rei Rocou! Direita\n");
	}

	//Promocao de peao
	if(lance.dst_i == tabuleiro.size()-1 && tabuleiro[lance.dst_i][lance.dst_j]==BlackPawn){
		tabuleiro[lance.dst_i][lance.dst_j]=BlackQueen;
	}
	else if(lance.dst_i == 0 && tabuleiro[lance.dst_i][lance.dst_j]==WhitePawn){
		tabuleiro[lance.dst_i][lance.dst_j]=WhiteQueen;
	}
}

bool aconteceu_EnPassant(std::vector<FEN>* controle_lances, char* peca, std::pair<char, char>* endereco_origem, char direcao){
	if(controle_lances!=NULL && (*controle_lances).size()>1){
		Lance ultimo_lance;
		bool eh_branca;
		std::pair<char, char> origem;

		int pos = (int) (*controle_lances).size()-1;
		if(endereco_origem==NULL){
			origem.first=(*controle_lances)[pos].move.src_i;
			origem.second=(*controle_lances)[pos].move.src_j;
			eh_branca=(*controle_lances)[pos].origem == WhitePawn;
			pos--;
		}
		else{
			if(peca==NULL) return false;
			origem = (*endereco_origem);
			eh_branca= (*peca)==WhitePawn;
		}
		char ultima_peca = (*controle_lances)[pos].origem;


		if((eh_branca && ultima_peca==BlackStaticPawn) || (!eh_branca && ultima_peca==WhiteStaticPawn)){
			std::pair<char, char> futura_posicao = par_mover_direcao(direcao, origem, 1);
			char i = futura_posicao.first;
			char j = futura_posicao.second;
			Lance ultimo_lance = (*controle_lances)[pos].move;
			//Peao esta do lado
			if(ultimo_lance.src_j == j && ultimo_lance.dst_j == j){
				//Se moveu duas casas
				if(abs(ultimo_lance.src_i-i)==1 && abs(ultimo_lance.dst_i-i)==1){
				return true;
				}
			}	
		}
	}
	return false;
}

bool EstaEmCheque(std::vector<std::vector<char>>& pecas_tabuleiro, int turno){
	int turno_oposto;
	if(turno == Black) turno_oposto = White;
	else if(turno == White) turno_oposto = Black;

	std::pair<char, char> posicao_rei;

	//Encontrar posicao rei
	for(int i=0; i<pecas_tabuleiro.size(); i++){
		for(int j=0; j<pecas_tabuleiro.size(); j++){
			if(turno == Black){
				if(rei_preto(pecas_tabuleiro[i][j])){
					posicao_rei.first = i;
					posicao_rei.second = j;
					goto fora;
				}
			}
			else if(turno == White){
				if(rei_branco(pecas_tabuleiro[i][j])){
					posicao_rei.first = i;
					posicao_rei.second = j;
					goto fora;
				}
			}
		}
	}
	fora:

	return alcanca_destino(pecas_tabuleiro, posicao_rei, turno_oposto);
}

//Refatorar esse codigo
bool movimento_permitido(int direcao, int tipo_lance, std::vector<std::vector<char>>& tabuleiro, std::pair<char, char> origem, int num_movimentos, std::vector<FEN>* controle_lances){
    int tamanho = tabuleiro.size();
    bool saida;
	
	char peca = tabuleiro[origem.first][origem.second];

	bool eh_branca;
	if(branco(peca)) eh_branca=true;
	else if(preto(peca)) eh_branca=false;
	else return false;

	int capturar=tipo_lance;
	if(tipo_lance == Capturar || tipo_lance == MoverCapturar || tipo_lance == Pulo || tipo_lance == Roque){
		capturar=Capturar;
	}

    switch(direcao){
        case Sul:
            (origem.first+num_movimentos < tamanho && (capturar==Capturar || tabuleiro[origem.first+num_movimentos][origem.second]==Vazio)) ? 
                saida=true : saida=false; break;
        
        case Norte:
            (origem.first-num_movimentos >= 0 && (capturar==Capturar || tabuleiro[origem.first-num_movimentos][origem.second]==Vazio)) ? 
                saida=true : saida=false; break;
                
        case Leste:
            origem.second+num_movimentos < tamanho && (capturar==Capturar || tabuleiro[origem.first][origem.second+num_movimentos]==Vazio) ? 
                saida=true : saida=false; break;
        
        case Oeste:
            origem.second-num_movimentos >= 0 && (capturar==Capturar || tabuleiro[origem.first][origem.second-num_movimentos]==Vazio) ? 
                saida=true : saida=false; break;

		case Nordeste:
			(origem.first-num_movimentos >= 0 && origem.second+num_movimentos < tamanho) && (capturar==Capturar || tabuleiro[origem.first-num_movimentos][origem.second+num_movimentos]==Vazio) ? 
                saida=true : saida=false; break;

		case Noroeste:
			(origem.first-num_movimentos >= 0 && origem.second-num_movimentos >= 0) && (capturar==Capturar || tabuleiro[origem.first-num_movimentos][origem.second-num_movimentos]==Vazio) ? 
                saida=true : saida=false; break;

		case Sudeste:
			(origem.first+num_movimentos < tamanho && origem.second+num_movimentos < tamanho) && (capturar==Capturar || tabuleiro[origem.first+num_movimentos][origem.second+num_movimentos]==Vazio) ? 
                saida=true : saida=false; break;

		case Sudoeste:
			(origem.first+num_movimentos < tamanho && origem.second-num_movimentos >= 0) && (capturar==Capturar || tabuleiro[origem.first+num_movimentos][origem.second-num_movimentos]==Vazio) ? 
                saida=true : saida=false; break;
			
		case CimaDireita:
			(origem.first-2 >= 0 && origem.second+1 < tamanho) ?
				saida=true : saida=false; break;

		case CimaEsquerda:
			(origem.first-2 >= 0 && origem.second-1 >= 0) ?
				saida=true : saida=false; break;

		case BaixoDireita:
			(origem.first+2 < tamanho && origem.second+1 < tamanho) ?
				saida = true : saida=false; break;

		case BaixoEsquerda:
			(origem.first+2 < tamanho && origem.second-1 >=0 ) ?
				saida = true : saida=false; break;

		case DireitaCima:
			(origem.first-1 >= 0 && origem.second+2 < tamanho) ?
				saida = true : saida=false; break;

		case DireitaBaixo:
			(origem.first+1 < tamanho && origem.second+2 < tamanho) ?
				saida = true : saida=false; break;

		case EsquerdaCima:
			(origem.first-1 >= 0 && origem.second-2 >= 0) ?
				saida = true : saida = false; break;

		case EsquerdaBaixo:
			(origem.first+1 < tamanho && origem.second-2 >= 0) ?
				saida = true : saida = false; break;
    }
	
	if(tipo_lance==EnPassant && saida){
		return aconteceu_EnPassant(controle_lances, &peca, &origem, direcao);
		/*
		if(controle_lances!=NULL && (*controle_lances).size() > 0){
			int ultima_peca = (*controle_lances)[(int) (*controle_lances).size()-1].origem;
			if((eh_branca && ultima_peca==BlackStaticPawn) || (!eh_branca && ultima_peca==WhiteStaticPawn)){

				std::pair<char, char> futura_posicao = par_mover_direcao(direcao, origem, num_movimentos);
				char i = futura_posicao.first;
				char j = futura_posicao.second;

				Lance ultimo_lance = (*controle_lances)[(int) (*controle_lances).size()-1].move;

				//Peao esta do lado
				if(ultimo_lance.src_j == j && ultimo_lance.dst_j == j){
					//Se moveu duas casas
					if(abs(ultimo_lance.src_i-i)==1 && abs(ultimo_lance.dst_i-i)==1){
						return true;
					}
				}	
			}
		}
		return false;
		*/
	}
	else if(tipo_lance==Roque && saida){
		char destino = calcular_destino(direcao, origem, tabuleiro, num_movimentos);
		if(eh_branca){
			return destino==WhiteStaticRook;
		}
		else{
			return destino==BlackStaticRook;
		}
	}	
    else if(capturar==Capturar && saida){
		//printf("If captura");
		char destino = calcular_destino(direcao, origem, tabuleiro, num_movimentos);
		if(tipo_lance==MoverCapturar || tipo_lance == Pulo){
			if(eh_branca){
				return destino==Vazio || preto(destino);
			}
			else{
				return destino==Vazio || branco(destino);
			}
		}
		else if(tipo_lance==Capturar){
			if(eh_branca){
				return destino!=Vazio && preto(destino);
			}
			else{
				return destino!=Vazio && branco(destino);
			}
		}
		else{
			if(debugLances) printf("Lance nao previsto");
			exit(1);
		}
	}
	else{
		//printf("If mover");
		return saida;
	}
}

std::pair<char, char> par_mover_direcao(int direcao, std::pair<char, char>& origem, int num_movimentos){
	switch(direcao){
		case Sul:
			return {origem.first+num_movimentos, origem.second};
        
		case Norte:
			return {origem.first-num_movimentos, origem.second};
        
		case Leste:
			return {origem.first, origem.second+num_movimentos};

        case Oeste:
			return {origem.first, origem.second-num_movimentos};

		case Nordeste:
			return {origem.first-num_movimentos, origem.second+num_movimentos};

		case Noroeste:
			return {origem.first-num_movimentos, origem.second-num_movimentos};

		case Sudeste:
			return {origem.first+num_movimentos, origem.second+num_movimentos};

		case Sudoeste:
			return {origem.first+num_movimentos, origem.second-num_movimentos};
		
		//Nao depende de num_movimentos ainda
		case CimaDireita:
			return {origem.first-2, origem.second+1};

		case CimaEsquerda:
			return {origem.first-2, origem.second-1};

		case BaixoDireita:
			return {origem.first+2, origem.second+1};

		case BaixoEsquerda:
			return {origem.first+2, origem.second-1};

		case DireitaCima:
			return {origem.first-1, origem.second+2};

		case DireitaBaixo:
			return {origem.first+1, origem.second+2};

		case EsquerdaCima:
			return {origem.first-1, origem.second-2};

		case EsquerdaBaixo:
			return {origem.first+1, origem.second-2};
	}

	//Isso provavelmente quebra o codigo
	exit(1);
	return {-1, -1};
}

Lance mover_direcao(int direcao, std::pair<char, char> origem, int num_movimentos){
	switch(direcao){
		case Sul:
			return {origem.first, origem.second, static_cast<char>(origem.first+num_movimentos), origem.second};
        
		case Norte:
			return {origem.first, origem.second, static_cast<char>(origem.first-num_movimentos), origem.second};
        
		case Leste:
			return {origem.first, origem.second, origem.first, static_cast<char>(origem.second+num_movimentos)};

        case Oeste:
			return {origem.first, origem.second, origem.first, static_cast<char>(origem.second-num_movimentos)};

		case Nordeste:
			return {origem.first, origem.second, static_cast<char>(origem.first-num_movimentos), static_cast<char>(origem.second+num_movimentos)};

		case Noroeste:
			return {origem.first, origem.second, static_cast<char>(origem.first-num_movimentos), static_cast<char>(origem.second-num_movimentos)};

		case Sudeste:
			return {origem.first, origem.second, static_cast<char>(origem.first+num_movimentos), static_cast<char>(origem.second+num_movimentos)};

		case Sudoeste:
			return {origem.first, origem.second, static_cast<char>(origem.first+num_movimentos), static_cast<char>(origem.second-num_movimentos)};

		//Nao depende de num_movimentos ainda
		case CimaDireita:
			return {origem.first, origem.second, static_cast<char>(origem.first-2), static_cast<char>(origem.second+1)};

		case CimaEsquerda:
			return {origem.first, origem.second, static_cast<char>(origem.first-2), static_cast<char>(origem.second-1)};

		case BaixoDireita:
			return {origem.first, origem.second, static_cast<char>(origem.first+2), static_cast<char>(origem.second+1)};

		case BaixoEsquerda:
			return {origem.first, origem.second, static_cast<char>(origem.first+2), static_cast<char>(origem.second-1)};

		case DireitaCima:
			return {origem.first, origem.second, static_cast<char>(origem.first-1), static_cast<char>(origem.second+2)};

		case DireitaBaixo:
			return {origem.first, origem.second, static_cast<char>(origem.first+1), static_cast<char>(origem.second+2)};

		case EsquerdaCima:
			return {origem.first, origem.second, static_cast<char>(origem.first-1), static_cast<char>(origem.second-2)};

		case EsquerdaBaixo:
			return {origem.first, origem.second, static_cast<char>(origem.first+1), static_cast<char>(origem.second-2)};
	}

	//Isso provavelmente quebra o codigo
	exit(1);
	return {-1, -1, -1, -1};
}

void sequencia_lances(std::vector<char>& direcoes, std::pair<char, char> origem, int tipo_lance, std::vector<Lance>& lances, std::vector<std::vector<char>>& tabuleiro, int num_movimentos, std::vector<FEN>* controle_lances){
	int tamanho=(int) lances.size();
	
	for(int i=0; i<direcoes.size(); i++){
		if(tipo_lance == EnPassant){
			int contador=1;
			if(movimento_permitido(direcoes[i], EnPassant, tabuleiro, origem, contador, controle_lances)){
				//printf("Norte: %d, Direcao: %d\n", Norte, direcoes[i]);
				lances.push_back(mover_direcao(direcoes[i], origem, 1));
			}
			bool porEnquantoNao=true;
		}

		else if(tipo_lance == Roque){
			int contador=1;
			while(movimento_permitido(direcoes[i], Mover, tabuleiro, origem, contador, controle_lances)){
				//printf("Origem: {%d %d}, contador: %d\n", origem.first, origem.second, contador);
				//lances.push_back(mover_direcao(direcoes[i], origem, contador));
				contador+=1;
			}
			if(movimento_permitido(direcoes[i], Roque, tabuleiro, origem, contador, controle_lances)){
				if(debugLances) printf("Roque possivel!\n");
				lances.push_back(mover_direcao(direcoes[i], origem, 2));
			}
		}

		else if(tipo_lance == FinalLinha){
			int contador=1;
			while(movimento_permitido(direcoes[i], Mover, tabuleiro, origem, contador, controle_lances)){
				contador+=1;
				if(contador==num_movimentos) break;
			}
			if(contador==num_movimentos && movimento_permitido(direcoes[i], Mover, tabuleiro, origem, contador, controle_lances)){
				lances.push_back(mover_direcao(direcoes[i], origem, contador));
			}
		}

		else if(tipo_lance == Linha){
			int contador=num_movimentos;
			while(movimento_permitido(direcoes[i], Mover, tabuleiro, origem, contador, controle_lances)){
				//printf("Origem: {%d %d}, contador: %d\n", origem.first, origem.second, contador);
				lances.push_back(mover_direcao(direcoes[i], origem, contador));
				contador+=num_movimentos;
			}
			if(movimento_permitido(direcoes[i], Capturar, tabuleiro, origem, contador, controle_lances)){
				lances.push_back(mover_direcao(direcoes[i], origem, contador));
			}
		}
		else{
			if(movimento_permitido(direcoes[i], tipo_lance, tabuleiro, origem, num_movimentos, controle_lances)){
				lances.push_back(mover_direcao(direcoes[i], origem, num_movimentos));
			}
		}
	}

	int turno;
	if(branco(tabuleiro[origem.first][origem.second])){
		turno = White;
	}
	else if(preto(tabuleiro[origem.first][origem.second])){
		turno = Black;
	}
	else{
		turno = Neither;
	}

	int retirados=0;
	if(controle_lances != NULL && turno!=Neither){
		int backup_turno = turno;
		for(int i=tamanho; i<lances.size(); i++){
			Lance copia_lance = lances[i];
			executar_lance(tabuleiro, copia_lance, controle_lances);
			
			if(EstaEmCheque(tabuleiro, turno)){
				if(debugLances) printf("Entrou no Xeque\n");
				lances.erase(lances.begin()+i);
				i--;
			}

			reverter_lance(*controle_lances, tabuleiro, turno);
			turno = backup_turno;
		}
	}
}

std::vector<Lance> possiveis_lances_peca(std::pair<char, char> origem, std::vector<std::vector<char>>& tabuleiro, std::vector<FEN>* controle_lances){
	std::vector<Lance> saida;
	std::vector<char> direcoes;

	switch(tabuleiro[origem.first][origem.second]){
		case BlackStaticPawn:
			direcoes = {Sul};
			sequencia_lances(direcoes, origem, FinalLinha, saida, tabuleiro, 2, controle_lances);
			direcoes.pop_back();

        case BlackPawn:
			direcoes={Sul};
			sequencia_lances(direcoes, origem, Mover, saida, tabuleiro, 1, controle_lances);

			direcoes = {Sudeste, Sudoeste};
			if(controle_lances != NULL){
				sequencia_lances(direcoes, origem, EnPassant, saida, tabuleiro, 1, controle_lances);
			}

			sequencia_lances(direcoes, origem, Capturar, saida, tabuleiro, 1, controle_lances);

			break;

		case WhiteStaticPawn:
			direcoes = {Norte};
			sequencia_lances(direcoes, origem, FinalLinha, saida, tabuleiro, 2, controle_lances);
			direcoes.pop_back();
		
		case WhitePawn:
			direcoes = {Norte};
			sequencia_lances(direcoes, origem, Mover, saida, tabuleiro, 1, controle_lances);

			direcoes = {Nordeste, Noroeste};
			if(controle_lances != NULL){
				sequencia_lances(direcoes, origem, EnPassant, saida, tabuleiro, 1, controle_lances);
			}

			sequencia_lances(direcoes, origem, Capturar, saida, tabuleiro, 1, controle_lances);

			break;

		case BlackStaticKing:
		case WhiteStaticKing:
			direcoes = {Leste, Oeste};
			sequencia_lances(direcoes, origem, Roque, saida, tabuleiro, 2, controle_lances);

		case BlackKing:
		case WhiteKing:
			direcoes = {Norte, Sul, Leste, Oeste, Nordeste, Noroeste, Sudeste, Sudoeste};
			sequencia_lances(direcoes, origem, MoverCapturar, saida, tabuleiro, 1, controle_lances);
			break;

		case BlackStaticRook:
		case WhiteStaticRook:
		case BlackRook:
		case WhiteRook:
			direcoes = {Norte, Sul, Leste, Oeste};
			sequencia_lances(direcoes, origem, Linha, saida, tabuleiro, 1, controle_lances);
			break;
			
		case BlackBishop:
		case WhiteBishop:
			direcoes = {Nordeste, Noroeste, Sudeste, Sudoeste};
			sequencia_lances(direcoes, origem, Linha, saida, tabuleiro, 1, controle_lances);
			break;
			
		case BlackQueen:
		case WhiteQueen:
			direcoes =  {Norte, Sul, Leste, Oeste, Nordeste, Noroeste, Sudeste, Sudoeste};
			sequencia_lances(direcoes, origem, Linha, saida, tabuleiro, 1, controle_lances);
			break;

		case BlackKnight:
		case WhiteKnight:
			direcoes = {CimaDireita, CimaEsquerda, BaixoDireita, BaixoEsquerda, DireitaCima, DireitaBaixo, EsquerdaCima, EsquerdaBaixo};
			sequencia_lances(direcoes, origem, Pulo, saida, tabuleiro, 1, controle_lances);
			break;
	}

	return saida;
}

bool alcanca_destino(std::vector<std::vector<char>>& pecas_tabuleiro, std::pair<char, char>& destino, int turno){
	bool cor_certa=true;

	bool retorno=false;

	for(int i=0; i<pecas_tabuleiro.size(); i++){
		for(int j=0; j<pecas_tabuleiro[i].size(); j++){
			if(turno == Black){
				cor_certa = preto(pecas_tabuleiro[i][j]);
			}
			else if(turno == White){
				cor_certa = branco(pecas_tabuleiro[i][j]);
			}
			
			if(cor_certa){
				std::vector<Lance> possiveis_lances = possiveis_lances_peca({i, j}, pecas_tabuleiro);
				int total = (int) possiveis_lances.size()-1;
				for(int k=total; k>=0; k--){
					if(!retorno && destino_igual(possiveis_lances[k], destino)){
						retorno = true;
					}
					possiveis_lances.pop_back();
				}
			}
		}
	}
	return retorno;
}

std::vector<Lance> todos_possiveis_lances(std::vector<std::vector<char>>& tabuleiro, int cor, std::vector<FEN>* controle_lances){
	std::vector<Lance> saida;
	bool cor_certa=true;
	
	for(uint8_t i=0; i<tabuleiro.size(); i++){
		for(uint8_t j=0; j<tabuleiro[i].size(); j++){
			if(cor==Black){
				cor_certa = preto(tabuleiro[i][j]);
			}
			else if(cor==White){
				cor_certa = branco(tabuleiro[i][j]);
			}
			
			if(cor_certa){
				std::pair<char, char> origem = {i, j};
				std::vector<Lance> atual = possiveis_lances_peca(origem, tabuleiro, controle_lances);
				for(int k=0; k<atual.size(); k++){
					saida.push_back(atual[k]);
				}
			}
		}
	}
	
	return saida;
}


void reverter_lance(std::vector<FEN>& controle_lances, std::vector<std::vector<char>>& pecas_tabuleiro, int& turno){
	int posicao = (int) controle_lances.size()-1;

	if(posicao >= 1 && turno!=Both){
		int pseudo_turno = Both;
		char ultima_peca = controle_lances[posicao].origem;
		char penultima_peca = controle_lances[posicao-1].origem;

		//printf("Ultima peca: %d %d\n", ultima_peca, penultima_peca);
		if(branco(ultima_peca) == branco(penultima_peca)){
			if(debugLances) printf("Reverter Dois Lances\n");
			reverter_lance(controle_lances, pecas_tabuleiro, pseudo_turno);
			posicao--;
		}
	}

	if(posicao!=-1){
		Lance ultimo_lance = controle_lances[posicao].move;
		char peca_capturada = controle_lances[posicao].peca;
		char peca_origem = controle_lances[posicao].origem;

		pecas_tabuleiro[ultimo_lance.src_i][ultimo_lance.src_j] = peca_origem;
		pecas_tabuleiro[ultimo_lance.dst_i][ultimo_lance.dst_j] = peca_capturada;
		
		controle_lances.pop_back();
		
		if(turno == Black) turno = White;
		else if(turno == White) turno = Black;
	}
}

void mostrar_FEN(std::vector<FEN>& controle_lances){
	for(int i=0; i<controle_lances.size(); i++){
		Lance l = controle_lances[i].move;
		char peca = controle_lances[i].peca;
		printf("Origem: {%d, %d} Destino: {%d, %d} peca: %d\n", l.src_i, l.src_j, l.dst_i, l.dst_j, peca);
	}
	printf("\n");
}

void imprimir_tabuleiro(std::vector<std::vector<char>>& tabuleiro){
	for(int i=0; i<tabuleiro.size()/2; i++){
		for(int j=0; j<tabuleiro[i].size()/2; j++){
			printf("%d ", tabuleiro[i][j]);
		}
		printf("\n");
	}
}

void limpar_lances(std::vector<Lance>& lances){
	int total_lances = lances.size();

	for(int i=0; i<total_lances; i++){
		lances.pop_back();
	}
}

void imprimir_lances(std::vector<Lance>& lances){
	int total_lances = (int) lances.size();

	for(int i=0; i<total_lances; i++){
		printf("Origem: %d %d Destino: %d %d\n", lances[i].src_i, lances[i].src_j, lances[i].dst_i, lances[i].dst_j);
	}
	printf("Total de lances possiveis: %d\n\n", total_lances);
}

void swap(char* a, char* b){
	char temp = *a;
	*a = *b;
	*b = temp;	
}

void inverter_tabuleiro(std::vector<std::vector<char>>& tabuleiro, int eixo){
	//1 = horizontal, 2 = vertical, 3 = metade horizontal, 4 = metade vertical

	int dist_x=tabuleiro.size(), dist_y=tabuleiro.size();
	if(eixo==1){
		dist_x/=2;
	}
	else if(eixo==2){
		dist_y/=2;
	}
	else{
		dist_x/=2;
		dist_y/=2;
	}

	for(int i=0; i<dist_x; i++){
		for(int j=0; j<dist_y; j++){
			if(eixo == 1 || eixo == 3) swap(&(tabuleiro[i][j]), &(tabuleiro[tabuleiro.size()-1-i][j]));
			else swap(&(tabuleiro[i][j]), &(tabuleiro[i][tabuleiro.size()-1-j]));
		}
	}
}