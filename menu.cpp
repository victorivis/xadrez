#include "menu.h"

int ModoDeJogo = Computador;

const char font_path[]="assets/TrueType/Sans.ttf";
const int font_size=50;


int rodar_menu_pausa(SDL_Renderer*& renderer, int width){
    //Inicializando bibliotecas SDL_ttf
    TTF_Init();

    //Criando variaveis do menu
    int retorno;
    TTF_Font* fonte = TTF_OpenFont("assets/TrueType/Sans.ttf", 64);
    const int NUMMENU=2;
    const char rotulo[NUMMENU][15] = {"Continuar", "Sair"};

    SDL_Surface* molde[NUMMENU];
    SDL_Texture* textura[NUMMENU];
    bool selecionado[NUMMENU] = {false, false};
    
    //Cores preto e vermelho
    SDL_Color cor[3] = {{255, 255, 255, 255}, {255, 0, 0, 255}, {200, 200, 200, 255}};
    //SDL_Color cor_renderer = cor[2];

    for(int i=0; i<NUMMENU; i++){
        molde[i] = TTF_RenderText_Solid(fonte, rotulo[i], cor[0]);
        textura[i] = SDL_CreateTextureFromSurface(renderer, molde[i]);
    }

    SDL_Rect pos_inicial={width/2, 50, 100, 100};
    SDL_Rect posicao[NUMMENU];
    for(int i=0; i<NUMMENU; i++){
        SDL_QueryTexture(textura[i], NULL, NULL, &pos_inicial.w, &pos_inicial.h);
        pos_inicial.x = width/2-pos_inicial.w/2;

        posicao[i]=pos_inicial;
        pos_inicial.y += pos_inicial.h;
    }

    //Main loop
    bool rodar_menu=true;
    SDL_Event evento_menu;
    do{
        while(SDL_PollEvent(&evento_menu)){
            if(evento_menu.type == SDL_QUIT){
                retorno=Sair;
                rodar_menu=false;
            }

            else if(evento_menu.type == SDL_KEYDOWN){
				switch(evento_menu.key.keysym.sym){
					case SDLK_ESCAPE:
                        retorno=Continuar;
                        rodar_menu=false;
                        break;
				}
			}
			else if(evento_menu.type == SDL_MOUSEMOTION){
                int mouse_x=0;
                int mouse_y=0;
                SDL_GetMouseState(&mouse_x, &mouse_y);

                for(int i=0; i<NUMMENU; i++){
                    bool eixo_x = (posicao[i].x <= mouse_x && posicao[i].x+posicao[i].w >= mouse_x);
                    bool eixo_y = (posicao[i].y <= mouse_y && posicao[i].y+posicao[i].h >= mouse_y);

                    if(eixo_x && eixo_y){
                        if(selecionado[i]==false){
                            selecionado[i]=true;
                            SDL_DestroyTexture(textura[i]);
                            SDL_FreeSurface(molde[i]);

                            molde[i] = TTF_RenderText_Solid(fonte, rotulo[i], cor[1]);
                            textura[i] = SDL_CreateTextureFromSurface(renderer, molde[i]);
                        }
                    }
                    else{
                        if(selecionado[i]==true){
                            selecionado[i]=false;
                            SDL_DestroyTexture(textura[i]);
                            SDL_FreeSurface(molde[i]);

                            molde[i] = TTF_RenderText_Solid(fonte, rotulo[i], cor[0]);
                            textura[i] = SDL_CreateTextureFromSurface(renderer, molde[i]);
                        }
                    }
                }
            }
            else if(evento_menu.type == SDL_MOUSEBUTTONDOWN){
                for(int i=0; i<NUMMENU; i++){
                    if(selecionado[i]){
                        retorno = i;
                        rodar_menu=false;
                        break;
                    }
                }
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
		SDL_RenderClear(renderer);
		
        for(int i=0; i<NUMMENU; i++){
            SDL_RenderCopy(renderer, textura[i], NULL, &posicao[i]);
        }

		SDL_RenderPresent(renderer);
        SDL_Delay(120);
    } while(rodar_menu);

    //Liberando os ponteiros
    TTF_CloseFont(fonte);
    for(int i=0; i<NUMMENU; i++){
        SDL_FreeSurface(molde[i]);
	    SDL_DestroyTexture(textura[i]);   
    }
    //Liberando os subsistemas de SDL_ttf
    TTF_Quit();
    
    return retorno;
}

int rodar_menu_principal(SDL_Renderer*& renderer, int width){
    //Inicializando bibliotecas SDL_ttf
    TTF_Init();

    //Criando variaveis do menu
    int retorno;
    TTF_Font* fonte = TTF_OpenFont("assets/TrueType/Sans.ttf", 64);
    const int NUMMENU=3;
    const char rotulo[NUMMENU][30] = {"Player x Player", "Configuracoes", "Sair"};

    SDL_Surface* molde[NUMMENU];
    SDL_Texture* textura[NUMMENU];
    bool selecionado[NUMMENU] = {false, false, false};
    
    //Cores preto e vermelho
    SDL_Color cor[3] = {{255, 255, 255, 255}, {255, 0, 0, 255}, {200, 200, 200, 255}};
    //SDL_Color cor_renderer = cor[2];

    for(int i=0; i<NUMMENU; i++){
        molde[i] = TTF_RenderText_Solid(fonte, rotulo[i], cor[0]);
        textura[i] = SDL_CreateTextureFromSurface(renderer, molde[i]);
    }

    SDL_Rect pos_inicial={width/2, 50, 100, 100};
    SDL_Rect posicao[NUMMENU];
    for(int i=0; i<NUMMENU; i++){
        SDL_QueryTexture(textura[i], NULL, NULL, &pos_inicial.w, &pos_inicial.h);
        pos_inicial.x = width/2-pos_inicial.w/2;

        posicao[i]=pos_inicial;
        pos_inicial.y += pos_inicial.h;
    }

    //Main loop
    bool rodar_menu=true;
    SDL_Event evento_menu;
    do{
        while(SDL_PollEvent(&evento_menu)){
            if(evento_menu.type == SDL_QUIT){
                retorno=Sair;
                rodar_menu=false;
            }

            else if(evento_menu.type == SDL_KEYDOWN){
				switch(evento_menu.key.keysym.sym){
					case SDLK_ESCAPE:
                        retorno=Continuar;
                        rodar_menu=false;
                        break;
				}
			}
			else if(evento_menu.type == SDL_MOUSEMOTION){
                int mouse_x=0;
                int mouse_y=0;
                SDL_GetMouseState(&mouse_x, &mouse_y);

                for(int i=0; i<NUMMENU; i++){
                    bool eixo_x = (posicao[i].x <= mouse_x && posicao[i].x+posicao[i].w >= mouse_x);
                    bool eixo_y = (posicao[i].y <= mouse_y && posicao[i].y+posicao[i].h >= mouse_y);

                    if(eixo_x && eixo_y){
                        if(selecionado[i]==false){
                            selecionado[i]=true;
                            SDL_DestroyTexture(textura[i]);
                            SDL_FreeSurface(molde[i]);

                            molde[i] = TTF_RenderText_Solid(fonte, rotulo[i], cor[1]);
                            textura[i] = SDL_CreateTextureFromSurface(renderer, molde[i]);
                        }
                    }
                    else{
                        if(selecionado[i]==true){
                            selecionado[i]=false;
                            SDL_DestroyTexture(textura[i]);
                            SDL_FreeSurface(molde[i]);

                            molde[i] = TTF_RenderText_Solid(fonte, rotulo[i], cor[0]);
                            textura[i] = SDL_CreateTextureFromSurface(renderer, molde[i]);
                        }
                    }
                }
            }
            else if(evento_menu.type == SDL_MOUSEBUTTONDOWN){
                for(int i=0; i<NUMMENU; i++){
                    if(selecionado[i]){
                        retorno = i;
                        rodar_menu=false;
                        break;
                    }
                }
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
		SDL_RenderClear(renderer);
		
        for(int i=0; i<NUMMENU; i++){
            SDL_RenderCopy(renderer, textura[i], NULL, &posicao[i]);
        }

		SDL_RenderPresent(renderer);
        SDL_Delay(120);
    } while(rodar_menu);

    //Liberando os ponteiros
    TTF_CloseFont(fonte);
    for(int i=0; i<NUMMENU; i++){
        SDL_FreeSurface(molde[i]);
	    SDL_DestroyTexture(textura[i]);   
    }
    //Liberando os subsistemas de SDL_ttf
    TTF_Quit();
    
    return retorno;
}

Menu::Menu(int w, int h){
    width = w;
    height = h;
    cor = {{255, 255, 255, 255}, {255, 0, 0, 255}, {200, 200, 200, 255}};
}

void Menu::inicializar(SDL_Renderer** renderer, std::vector<std::string>& nomes_para_rotulo, std::string& caminho_fonte, int tamanho_fonte){
    TTF_Init();
    
    rotulo = nomes_para_rotulo;
    fonte = TTF_OpenFont(caminho_fonte.c_str(), tamanho_fonte);

    SDL_Rect pos_inicial{0, 50, 0, 0};
    for(int i=0; i<rotulo.size(); i++){
        molde.push_back(TTF_RenderText_Solid(fonte, rotulo[i].c_str(), cor[0]));
        textura.push_back(SDL_CreateTextureFromSurface(*renderer, molde[i]));

        SDL_QueryTexture(textura[i], NULL, NULL, &pos_inicial.w, &pos_inicial.h);
        pos_inicial.x = width/2-pos_inicial.w/2;

        posicao.push_back(pos_inicial);
        pos_inicial.y += pos_inicial.h;
        selecionado.push_back(false);
    }
}

Menu::~Menu(){
    //Liberando os ponteiros
    TTF_CloseFont(fonte);
    for(int i=(int) rotulo.size()-1; i>=0; i--){
        SDL_FreeSurface(molde[i]);
	    SDL_DestroyTexture(textura[i]);
        
        molde.pop_back();
        textura.pop_back();
        rotulo.pop_back();
        posicao.pop_back();
        selecionado.pop_back();
    }
    //Liberando os subsistemas de SDL_ttf
    TTF_Quit();
}

int Menu::menu_generico(SDL_Renderer** renderer, std::vector<std::string> nomes_para_rotulo, 
            std::string caminho_fonte, int tamanho_fonte){
    inicializar(renderer, nomes_para_rotulo, caminho_fonte, tamanho_fonte);

    //Main loop
    bool rodar_menu=true;
    SDL_Event evento_menu;
    while(rodar_menu){
        while(SDL_PollEvent(&evento_menu)){
            if(evento_menu.type == SDL_QUIT){
                retorno=Sair;
                rodar_menu=false;
            }

            else if(evento_menu.type == SDL_KEYDOWN){
				switch(evento_menu.key.keysym.sym){
					case SDLK_ESCAPE:
                        retorno=Continuar;
                        rodar_menu=false;
                        break;
				}
			}
			else if(evento_menu.type == SDL_MOUSEMOTION){
                int mouse_x=0;
                int mouse_y=0;
                SDL_GetMouseState(&mouse_x, &mouse_y);

                for(int i=0; i<rotulo.size(); i++){
                    bool eixo_x = (posicao[i].x <= mouse_x && posicao[i].x+posicao[i].w >= mouse_x);
                    bool eixo_y = (posicao[i].y <= mouse_y && posicao[i].y+posicao[i].h >= mouse_y);

                    if(eixo_x && eixo_y){
                        if(selecionado[i]==false){
                            selecionado[i]=true;
                            SDL_DestroyTexture(textura[i]);
                            SDL_FreeSurface(molde[i]);

                            molde[i] = TTF_RenderText_Solid(fonte, rotulo[i].c_str(), cor[1]);
                            textura[i] = SDL_CreateTextureFromSurface(*renderer, molde[i]);
                        }
                    }
                    else{
                        if(selecionado[i]==true){
                            selecionado[i]=false;
                            SDL_DestroyTexture(textura[i]);
                            SDL_FreeSurface(molde[i]);

                            molde[i] = TTF_RenderText_Solid(fonte, rotulo[i].c_str(), cor[0]);
                            textura[i] = SDL_CreateTextureFromSurface(*renderer, molde[i]);
                        }
                    }
                }
            }
            else if(evento_menu.type == SDL_MOUSEBUTTONDOWN){
                for(int i=0; i<rotulo.size(); i++){
                    if(selecionado[i]){
                        retorno = i;
                        rodar_menu=false;
                        break;
                    }
                }
            }
        }
        
        SDL_SetRenderDrawColor(*renderer, 100, 100, 100, 255);
		SDL_RenderClear(*renderer);
		
        for(int i=0; i<rotulo.size(); i++){
            SDL_RenderCopy(*renderer, textura[i], NULL, &posicao[i]);
        }

		SDL_RenderPresent(*renderer);
        SDL_Delay(120);
    }

    return retorno;
}

void Menu::menu_principal(SDL_Renderer** renderer, bool* rodar_jogo){
    switch(menu_generico(renderer, {"Jogador x Jogador", "Jogador x Maquina", "Sair"}, font_path, font_size)){
        case 0:
            ModoDeJogo=Player;
            break;

        case 1:
            ModoDeJogo=Computador;
            break;
        
        case 2:
            *rodar_jogo=false;
            break;
    }
}

int Menu::menu_pausa(SDL_Renderer** renderer, bool* rodar_jogo){
    switch(menu_generico(renderer, {"Continuar", "Menu Principal", "Sair"}, font_path, font_size)){
        case 1:
            return 1;
        case 2:
            *rodar_jogo=false;
    }

    return 0;
}
