# Diretório onde estão os arquivos fonte
SRC_DIR = .

# Diretório onde serão gerados os arquivos objeto
OBJ_DIR = obj

# Libs
LIBS = -I ..\..\SDL2\x86_64-w64-mingw32\include\ -L ..\..\SDL2\x86_64-w64-mingw32\lib\ -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -DLOCAL
WEBLIBS = -s WASM=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 --preload-file assets -s EXIT_RUNTIME=1
FILES = main.cpp pecas.cpp visual.cpp

wincpp: $(FILES)
	g++ $(FILES) $(LIBS) -o build/saida.exe

webcpp: $(FILES)
	em++ $(FILES) $(WEBLIBS) -o build/index.js

winrun: wincpp
	build/saida.exe

webrun: webcpp
	emrun build/index.html