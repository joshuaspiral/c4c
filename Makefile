CC=clang
CFLAGS=-Wall -Wextra -ggdb -O3

make all: ai_bitboard.c ai.c twoplayer.c
	$(CC) $(CFLAGS) -o ai_bitboard ai_bitboard.c -lm
	$(CC) $(CFLAGS) -o ai ai.c -lm
	$(CC) $(CFLAGS) -o twoplayer twoplayer.c -lm

ai_bitboard: ai_bitboard.c
	$(CC) $(CFLAGS) -o ai_bitboard ai_bitboard.c -lm

ai: ai.c
	$(CC) $(CFLAGS) -o ai ai.c -lm

twoplayer: twoplayer.c
	$(CC) $(CFLAGS) -o twoplayer twoplayer.c -lm

clean:
	rm ai_bitboard ai twoplayer
