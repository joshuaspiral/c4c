#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#define HEIGHT 6
#define WIDTH 7
#define BOARDLEN 64
#define ANSI_COLOR_RED     "\x1b[31m" 
#define ANSI_COLOR_YELLOW  "\x1b[33m" 
#define ANSI_COLOR_RESET   "\x1b[0m" 

enum Player {
    YELLOW,
    RED,
};

enum Player const AI = YELLOW;
enum Player const HUMAN = RED;

struct boards {
    unsigned long long yellow;
    unsigned long long red;
};
struct boards Boards = {0b0, 0b0};

uint8_t heights[] = {56, 57, 58, 59, 60, 61, 62};
// Stores top piece for each column

enum Player currPlayer = RED; // Starting player

// Drops the piece onto a column
bool dropPiece(int col, enum Player piece) {
    // Check if height of next piece is in the board boundaries
    if (heights[col] < 16 || col < 0 || col > 6)
        return false;
    // Check piece
    if (piece == YELLOW)
        // Calculates new column using the height of the next piece
        Boards.yellow ^= 1ULL << (BOARDLEN - 1 - heights[col]);
    else
        Boards.red ^= 1ULL << (BOARDLEN - 1 - heights[col]);
    // Move for next use
    heights[col] -= 8;
    return true;
}

// Pops the last played piece of a column
void popPiece(int col, enum Player piece) {
    heights[col] += 8;
    if (piece == YELLOW)
        Boards.yellow &= ~(1UL << (BOARDLEN - 1 - heights[col]));
    else
        Boards.red &= ~(1UL << (BOARDLEN - 1 - heights[col]));
}


// TODO still prints the last column
// Prints and formats the board with ANSI colors.
void printBoard() {
    unsigned short column = 0;
    unsigned long long place = 1UL << 47;
    for (int i = 16; i < 64; i++) {
        column++;
        printf("|");
        if ((Boards.yellow & place) == place)
            printf(ANSI_COLOR_YELLOW " Y " ANSI_COLOR_RESET);
        else if ((Boards.red & place) == place)
            printf(ANSI_COLOR_RED " R " ANSI_COLOR_RESET);
        else
            printf(" . ");

        if (!(column % 8))
            printf("|\n");
        place >>= 1;
    }
    printf("-----------------------------\n ");
    for (int i = 1; i < WIDTH + 1; i++)
        printf(" %d  ", i);
    printf("\n");
}

// Evaluates the board's current state for enum Player piece. Returns integer score.
int evaluateBoard(unsigned long long bitboard, unsigned long long oppBitboard) {
    // Vertical check
    if ((bitboard & (bitboard >> 8) & (bitboard >> 16) & (bitboard >> 24)) != 0) {
        return 100000;
    } else if ((oppBitboard & (oppBitboard >> 8) & (oppBitboard >> 16) & (oppBitboard >> 24)) != 0) {
        return -100000; 
    } 

    // Horizontal check
    if ((bitboard & (bitboard >> 1) & (bitboard >> 2) & (bitboard >> 3)) != 0) {
        return 100000;
    } else if ((oppBitboard & (oppBitboard >> 1) & (oppBitboard >> 2) & (oppBitboard >> 3)) != 0) {
        return -100000;
    }
    
    // Primary diagonal check (/)
    if ((bitboard & (bitboard >> 7) & (bitboard >> 14) & (bitboard >> 21)) != 0) {
        return 100000;
    } else if ((oppBitboard & (oppBitboard >> 7) & (oppBitboard >> 14) & (oppBitboard >> 21)) != 0) {
        return -100000;
    }
    
    // Secondary diagonal check (\)
    if ((bitboard & (bitboard >> 9) & (bitboard >> 18) & (bitboard >> 27)) != 0) {
        return 100000;
    } else if ((oppBitboard & (oppBitboard >> 9) & (oppBitboard >> 18) & (oppBitboard >> 27)) != 0) {
        return -100000;
    }

    if ((Boards.yellow | Boards.red) == 280371153272574) // if board is maxed out (excluding top two rows and last column)
        return 0;

    return 420;
}


// Starts the minimax search.

int main() {
    printf("Welcome to C4C, this is the twoplayer implementation. Try and see if you can beat your friend!\n");
    printBoard();
    while (true) {
        int col;
        char* currPlayerString = ((currPlayer == YELLOW) ? "YELLOW" : "RED");
        printf("%s, enter your column from 1-7: ", currPlayerString);
        scanf("%d", &col);
        while (!dropPiece(col - 1, currPlayer) || col < 1 || col > 7) {
            printf("%s, Column already full or out of bounds. Re-enter your column from 1-7: ", currPlayerString);
            scanf("%d", &col);
        }
        currPlayer = ((currPlayer == YELLOW) ? RED : YELLOW);
        printBoard();
            
        int result = evaluateBoard(Boards.yellow, Boards.red);
        if (result == 100000) {
            printf("YELLOW wins!\n");
            printBoard();
            break;
        } else if (result == -100000){
            printf("RED wins!\n");
            printBoard();
            break;
        } else if (result == 0) {
            printf("DRAW!\n");
            printBoard();
            break;
        } else if (result == 420){
            continue;
        }

    }
}
