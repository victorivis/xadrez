# Libs
WINLIBS = -I ..\..\SDL2\x86_64-w64-mingw32\include\ -L ..\..\SDL2\x86_64-w64-mingw32\lib\ -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -DLOCAL
WEBLIBS = -s WASM=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 --preload-file assets -s EXIT_RUNTIME=1
LINUXLIBS = -std=c++20 -Wno-narrowing $(shell pkg-config --cflags --libs sdl2 ) -lSDL2_ttf
FILES = src/main.cpp src/pecas.cpp src/visual.cpp src/ia.cpp src/audio.cpp

wincpp: $(FILES)
	g++ $(FILES) $(WINLIBS) -o saida.exe

linuxcpp: $(FILES)
	g++ $(FILES) $(LINUXLIBS) -o saida.out

webcpp: $(FILES)
	em++ $(FILES) $(WEBLIBS) -o index.html

winrun: wincpp
	./saida.exe

linuxrun: linuxcpp
	./saida.out

webrun: webcpp
	emrun index.html