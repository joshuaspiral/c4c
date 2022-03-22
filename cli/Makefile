INCLUDES := -I includes/
CC := cc
CFLAGS := -Wall -Wextra -Werror -ggdb -O3
LIBS := -lm

make all: src/ includes/
	${CC} ${CFLAGS} ${INCLUDES} -o ai src/ai.c src/logic.c ${LIBS}
	${CC} ${CFLAGS} ${INCLUDES} -o twoplayer src/twoplayer.c src/logic.c ${LIBS}

clean:
	rm twoplayer ai
