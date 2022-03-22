INCLUDES := -I includes/
CC := cc
CFLAGS := -Wall -Wextra -Werror -ggdb -O3
LIBS := -lm

make all: ai.c twoplayer.c logic.c includes/logic.h
	${CC} ${CFLAGS} ${INCLUDES} -o ai ai.c logic.c ${LIBS}
	${CC} ${CFLAGS} ${INCLUDES} -o twoplayer twoplayer.c logic.c ${LIBS}

# make ai: ai.c logic.c includes/logic.h
# 	${CC} ${CFLAGS} ${INCLUDES} -o ai ai.c logic.c ${LIBS}

# make twoplayer: twoplayer.c logic.c 
# 	${CC} ${CFLAGS} ${INCLUDES} -o twoplayer twoplayer.c logic.c ${LIBS}

clean:
	rm twoplayer ai
