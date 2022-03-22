INCLUDES := -I includes/
CC := cc
CFLAGS := -Wall -Wextra -Werror -ggdb -O3 `pkg-config sdl2 --cflags` `pkg-config SDL2_ttf --cflags`
LIBS := `pkg-config sdl2 --libs` -lSDL2_ttf -lm

make all: src/ includes/
	${CC} ${CFLAGS} ${INCLUDES} -o ai src/ai.c src/logic.c src/render.c ${LIBS}
	${CC} ${CFLAGS} ${INCLUDES} -o twoplayer src/twoplayer.c src/logic.c src/render.c ${LIBS}

clean:
	rm twoplayer ai
