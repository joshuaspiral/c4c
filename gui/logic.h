#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#define LOGIC_H

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
int evaluateBoard(unsigned long long bitboard, unsigned long long oppBitboard);
