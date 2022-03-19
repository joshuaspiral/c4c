#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#define LOGIC_H
#define HEIGHT 6
#define WIDTH 7

typedef enum {
    YELLOW,
    RED,
} Player;

typedef struct{
    unsigned long long yellow;
    unsigned long long red;
} Boards;


// Drops the piece onto a column
bool dropPiece(unsigned long long *yellow, unsigned long long *red, int col, Player player, uint8_t (*heights)[7]);
 
// Pops the last played piece of a column
void popPiece(unsigned long long *yellow, unsigned long long *red, int col, Player player, uint8_t (*heights)[7]);

// Evaluates the board's current state for enum Player piece. Returns integer score.
int evaluateBoard(unsigned long long bitboard, unsigned long long oppBitboard, int depth);

int minimax(unsigned long long *yellow, unsigned long long *red, uint8_t (*heights)[WIDTH], int depth, bool isMaximising, double alpha, double beta);

int aiMove(unsigned long long *yellow, unsigned long long *red, int depth, uint8_t (*heights)[WIDTH]);
