CC = 
CFLAGS = 

SRC = $(shell find src/* -name *.c -type f)
HTML = index.html

all: $(HTML)

$(HTML): $(SRC)
	emcc -Wall -O2 -s USE_SDL=2 -s USE_SDL_TTF=2 --preload-file assets -s ALLOW_MEMORY_GROWTH=1 $^ -o $@

future: $(SRC)
	gcc -Wall -O3 -Os $^ -o $@ `pkg-config sdl2 --libs --static` `pkg-config sdl2 --cflags` -lSDL2_ttf

clean:
	-rm $(HTML)
