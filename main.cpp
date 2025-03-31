#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

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
    SDL_RenderPresent(renderer);

    SDL_Delay(delay);
}

int main(int argc, char* argv[]){
    //Inicializando Janela
    SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);

    	
    font = TTF_OpenFont("assets/Sans.ttf", 25);

    #ifndef LOCAL
        Context context;
        emscripten_set_main_loop_arg(loopPrincipal, &context, -1, 1);
    #else
        while(rodar){
            loopPrincipal();
        }
    #endif    

    //Liberando memoria
    TTF_CloseFont(font);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

    return 0;
}