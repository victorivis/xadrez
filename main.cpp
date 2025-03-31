#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <cstdio>
#include "menu.h"
#include "pecas.h"

#ifndef LOCAL

#include <emscripten.h>
struct Context{
    int existe;
};

#endif

SDL_Event evento;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font * font;
SDL_Color textColor;
SDL_Surface *textSurface;
SDL_Texture *textTexture;

const int WIDTH = 1000;
const int HEIGHT = 600;

int delay = 100;
bool rodar=true;

void loopPrincipal(void* arg){
    static int sequenciaDirecoes[4] = {2,3,1,0};

    while(SDL_PollEvent(&evento)){
        if(evento.type == SDL_QUIT){
            #ifndef LOCAL
                exit(0);
            #else
                rodar=false;
            #endif
        }
        if(evento.type == SDL_MOUSEBUTTONDOWN){
                        
        }
        if(evento.type == SDL_KEYDOWN){
            //executarControles(evento.key.keysym.sym);
        }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_Rect textRect = {50, 50, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_RenderPresent(renderer);

    SDL_Delay(delay);
}

int main(int argc, char* argv[]){
    //Inicializando Janela
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
	SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);

    	
    font = TTF_OpenFont("assets/Sans.ttf", 25);
    textColor = {0, 0, 0, 255};

    if (font == NULL) {
        printf("Erro ao carregar a fonte: %s\n", TTF_GetError());

        int temp;
        scanf("%d", &temp);
    }

    textSurface = TTF_RenderText_Solid(font, "Hello World!", textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

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
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
    TTF_Quit();

    return 0;
}