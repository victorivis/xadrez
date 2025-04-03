#include "ia.h"

bool exibirMinimax=false;

int retorna_valor(char peca){
    switch(peca){
        case BlackPawn:
        case BlackStaticPawn:
            return -peao;

        case WhiteStaticPawn:
        case WhitePawn:
            return peao;

        case BlackKnight:
            return -cavalo;

        case WhiteKnight:
            return cavalo;

        case BlackBishop:
            return -bispo;
        
        case WhiteBishop:
            return bispo;

        case BlackRook:
        case BlackStaticRook:
            return -torre;

        case WhiteStaticRook:
        case WhiteRook:
            return torre;

        case BlackQueen:
            return -rainha;

        case WhiteQueen:
            return rainha;

        case BlackKing:
        case BlackStaticKing:
            return -rei;

        case WhiteStaticKing:
        case WhiteKing:
            return rei;

        default:
            return 0;
    }
}

int avaliacao_posicao(std::vector<std::vector<char>>& pecas_tabuleiro){
    int total=0;
    for(int i=0; i<pecas_tabuleiro.size(); i++){
        for(int j=0; j<pecas_tabuleiro[i].size(); j++){
			char peca = pecas_tabuleiro[i][j];
            total += retorna_valor(peca);
			switch(peca){
				case BlackKing:
				case BlackStaticKing:
				    break;

				case WhiteStaticKing:
				case WhiteKing:
					break;

				case Vazio:
				case Agua:
				case Borda:
					break;

				case BlackPawn:
				case BlackStaticPawn:
				    total-= std::min(i, j)*5-10;
					break;	

				case WhiteStaticPawn:
				case WhitePawn:
				    total+= std::min(i, j)*5-10;
					break;					

				default:
					if(branco(peca)){
						total+= std::min(i, j)*2-6;
					}
					else{
						total-= std::min(i, j)*2-6;
					}
			}
        }
    }

    return total;
}

Lance minmax(std::vector<FEN>& controle_lances, std::vector<std::vector<char>>& pecas_tabuleiro, int& turno, bool max){
    std::vector<Lance> possibilidades = todos_possiveis_lances(pecas_tabuleiro, turno, &controle_lances);
    if(possibilidades.size()==0){
        return {-1, -1, -1, -1};
    }
    
    int resultado=0;
    int valor_resultado=0;
    int backup_turno = turno;

    for(int i=0; i<possibilidades.size(); i++){
        executar_lance(pecas_tabuleiro, possibilidades[i], &controle_lances);
        int result_iteracao = avaliacao_posicao(pecas_tabuleiro);

        if(turno==White){
            if(valor_resultado < result_iteracao){
                valor_resultado = result_iteracao;
                resultado = i;
            }
        }
        else{
            if(valor_resultado > result_iteracao){
                valor_resultado = result_iteracao;
                resultado = i;
            }
        }

        reverter_lance(controle_lances, pecas_tabuleiro, backup_turno);
    }

    return possibilidades[resultado];
}

void minmax_recusivo(Lance* escolhido, int* pontuacao, int profundidade, std::vector<FEN>& controle_lances, 
    std::vector<std::vector<char>>& pecas_tabuleiro, int& turno, bool max){
        if(profundidade>0){
            std::vector<Lance> possibilidades = todos_possiveis_lances(pecas_tabuleiro, turno, &controle_lances);
            if(possibilidades.size()!=0){
                int backup_turno=turno;
                int melhor_lance=0;

                //calcula o primeiro elemento
                executar_lance(pecas_tabuleiro, possibilidades[0], &controle_lances);
                int melhor_pontuacao=avaliacao_posicao(pecas_tabuleiro);
                reverter_lance(controle_lances, pecas_tabuleiro, backup_turno);

                for(int i=1; i<possibilidades.size(); i++){
                    executar_lance(pecas_tabuleiro, possibilidades[i], &controle_lances);
                    int pontuacao_iteracao=avaliacao_posicao(pecas_tabuleiro);
                    reverter_lance(controle_lances, pecas_tabuleiro, backup_turno);

                    if(turno==White){
                        if(melhor_pontuacao < pontuacao_iteracao){
                            melhor_pontuacao = pontuacao_iteracao;
                            melhor_lance = i;
                        }
                    }
                    else{
                        if(melhor_pontuacao > pontuacao_iteracao){
                            melhor_pontuacao = pontuacao_iteracao;
                            melhor_lance = i;
                        }
                    }
                }
            }
        }
}

int minimax(std::vector<std::vector<char>>& pecas_tabuleiro, std::vector<FEN>& controle_lances, 
    int height, bool ehMax){
    /*
	int resultado = checkWinner();
    if(resultado != NULL){
        return resultado;
    }
	*/
    
    if(height>=profundidade){
        return avaliacao_posicao(pecas_tabuleiro);
    }
    
    if(ehMax){
        int turno=White;
        int backup_turno=White;
        int bestScore = INT_MIN;
        std::vector<Lance> possibilidades = todos_possiveis_lances(pecas_tabuleiro, turno, &controle_lances);
	if((int) possibilidades.size()>0){        
		for(int i=0; i<possibilidades.size(); i++){
		    executar_lance(pecas_tabuleiro, possibilidades[i], &controle_lances);
		    int score = minimax(pecas_tabuleiro, controle_lances, height+1, !ehMax);
		    reverter_lance(controle_lances, pecas_tabuleiro, backup_turno);
		    bestScore = std::max(score, bestScore);
			if(exibirMinimax) printf("White height: %d score: %d\n", height, score);
		}
		if(exibirMinimax) printf("White height: %d bestScore: %d\n\n", height, bestScore);		

		return bestScore;
	}
	printf("Futuro xeque-mate\n");
	return -100000;
    }
    else{
        int turno=Black;
        int backup_turno=Black;
        int bestScore = INT_MAX;
        std::vector<Lance> possibilidades = todos_possiveis_lances(pecas_tabuleiro, turno, &controle_lances);
	if((int) possibilidades.size()>0){  
		for(int i=0; i<possibilidades.size(); i++){
		    executar_lance(pecas_tabuleiro, possibilidades[i], &controle_lances);
		    int score = minimax(pecas_tabuleiro, controle_lances, height+1, !ehMax);
		    reverter_lance(controle_lances, pecas_tabuleiro, backup_turno);
		    bestScore = std::min(score, bestScore);
			if(exibirMinimax) printf("Black height: %d score: %d\n", height, score);
		}
		if(exibirMinimax) printf("Black height: %d bestScore: %d\n\n", height, bestScore);

		return bestScore;
	}
	printf("Futuro xeque-mate\n");
	return 100000;
    }
}

Lance bestMove(std::vector<std::vector<char>>& pecas_tabuleiro, std::vector<FEN>& controle_lances, 
    int turno){
    int bestScore;
	int backup_turno = turno;
	if(turno==White){
		bestScore = INT_MIN;
	}
	else{
		bestScore = INT_MAX;
	}

    Lance move;
    std::vector<Lance> possibilidades = todos_possiveis_lances(pecas_tabuleiro, turno, &controle_lances);
    if(possibilidades.size()>0){
        for(int i=0; i<possibilidades.size(); i++){
			executar_lance(pecas_tabuleiro, possibilidades[i], &controle_lances);
            int score = minimax(pecas_tabuleiro, controle_lances, 0, turno==Black);
			reverter_lance(controle_lances, pecas_tabuleiro, backup_turno);
            if(turno==White && score > bestScore){
				bestScore = score;
				move=possibilidades[i];
			}
			else if(turno==Black && score < bestScore){
				bestScore = score;
				move=possibilidades[i];
			}
		}

        return move;
    }
    else{
        return {-1, -1, -1, -1};
    }
}

bool executar_lance_ia(std::vector<FEN>& controle_lances, std::vector<std::vector<char>>& pecas_tabuleiro, int& turno){
    bool eh_max = turno==White ? true : false;
    turno = turno==White ? Black : White;

    //Lance lance_escolhido = minmax(controle_lances, pecas_tabuleiro, turno, eh_max);
	Lance lance_escolhido = bestMove(pecas_tabuleiro, controle_lances, turno);
    if(lance_escolhido.src_i!=-1){
        executar_lance(pecas_tabuleiro, lance_escolhido, &controle_lances);
        marcarUltimoLance = lance_escolhido;
        return true;
    }
    else{
        printf("Fim de Jogo\n");
        return false;
    }
}