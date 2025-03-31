em++ main.cpp -s WASM=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' --preload-file assets -s EXIT_RUNTIME=1 -s USE_SDL_TTF=2 -o index.js

em++ main.cpp -s WASM=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 --preload-file assets -s EXIT_RUNTIME=1 -o index.html