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
unsigned long long count = 0;

enum Player {
    YELLOW,
    RED,
};

enum Player const AI = YELLOW;
enum Player const HUMAN = RED;

uint8_t bitPowers[8] = {1, 2, 4, 8, 16, 32, 64, 128};

unsigned long long boards[2] = {0b0, 0b0};
// boards[0] is YELLOW and boards[1] is RED

uint8_t heights[] = {56, 57, 58, 59, 60, 61, 62};
// Stores top piece for each column

enum Player currPlayer = RED; // Starting player

// Drops the piece onto a column
bool dropPiece(int col, enum Player piece) {
    // Check if height of next piece is in the board boundaries
    if (heights[col] < 16 || col < 0 || col > 6)
        return false;
    // Check piece
    if (piece == YELLOW) {
        // Calculates new column using the height of the next piece
        boards[0] = boards[0] | (unsigned long long)(1UL << (BOARDLEN - 1 - heights[col]));
    }
    else {
        boards[1] = boards[1] | (unsigned long long)(1UL << (BOARDLEN - 1 - heights[col]));
    }
    // Move for next use
    heights[col] -= 8;
    return true;
}

// Pops the last played piece of a column
void popPiece(int col, enum Player piece) {
    heights[col] += 8;
    if (piece == YELLOW)
        boards[0] &= ~(1UL << (BOARDLEN - 1 - heights[col]));
    else
        boards[1] &= ~(1UL << (BOARDLEN - 1 - heights[col]));
}


// TODO
// Prints and formats the board with ANSI colors.
void printBoard() {
    for (unsigned long long i = 1; i < 64; i++) {
        /* if (i % 8 == 0 || i < 16) { */
        /*     continue; */
        /* } */
        printf("|");
        if ((boards[0] & (unsigned long long)pow(2.0, 63-i)) == (unsigned long long)pow(2.0, 63-i)) {
            printf(ANSI_COLOR_YELLOW " Y " ANSI_COLOR_RESET);
        } if ((boards[1] & (unsigned long long)pow(2.0, 63-i)) == (unsigned long long)pow(2.0, 63-i)) {
            printf(ANSI_COLOR_RED " R " ANSI_COLOR_RESET);
        } else {
            printf(" . ");
        }
        if (i % 8 == 0) {
            printf("|\n");
        }
    }
    /* for (int i = 6; i >= 0; i--) { */
    /*     for (int offset = 0; offset < WIDTH; offset++) { */
    /*         printf("|"); */
    /*         if (yBoard[offset] & bitPowers[i]) */
    /*             printf(ANSI_COLOR_YELLOW " Y " ANSI_COLOR_RESET); */
    /*         else if (rBoard[offset] & bitPowers[i]) */
    /*             printf(ANSI_COLOR_RED " R " ANSI_COLOR_RESET); */
    /*         else */
    /*         printf(" . "); */
    /*     } */
    /*     printf("|\n"); */
    /* } */
    /* printf("-----------------------------\n "); */
    /* for (int i = 1; i < WIDTH + 1; i++) { */
    /*     printf(" %d  ", i); */
    /* } */
    /* printf("\n"); */
}

// Evaluates the board's current state for enum Player piece. Returns integer score.
int evaluateBoard(unsigned long long bitboard, unsigned long long oppBitboard, int depth) {
    int score = 0;
    // Center heuristic TODO
    /* int centerCol = WIDTH / 2; */
    /* for (int i = 0; i < 6; i++) */
    /*     if (board[centerCol] & bitPowers[i]) */
    /*         score++; */
    
    // Vertical check
    if (((bitboard >> 8) & (bitboard >> 16) & (bitboard >> 24) & (bitboard >> 32)) != 0) {
        return 100000;
    } else if (((oppBitboard >> 8) & (oppBitboard >> 16) & (oppBitboard >> 24)) != 0) {
        return -100000; 
    } 

    // Vertical heuristic - 3 in a row
    if (((bitboard >> 8) & (bitboard >> 16)) != 0)
        score += 3;

    // Horizontal check
    if (((bitboard >> 1) & (bitboard >> 2) & (bitboard >> 3) & (bitboard >> 4)) != 0) {
        return 100000;
    } else if (((oppBitboard >> 1) & (oppBitboard >> 2) & (oppBitboard >> 3) & (oppBitboard >> 4)) != 0) {
        return -100000;
    }
    
    // Horizontal heuristic - 3 in a row
    if (((bitboard >> 1) & (bitboard >> 2) & (bitboard >> 3)) != 0)
        score += 3;

    // Primary diagonal check
    if (((bitboard >> 7) & (bitboard >> 14) & (bitboard >> 21) & (bitboard >> 28)) != 0) {
        return 100000;
    } else if (((oppBitboard >> 7) & (oppBitboard >> 14) & (oppBitboard >> 21) & (bitboard >> 28)) != 0) {
        return -100000;
    }
    
    // Primary diagonal heuristic - 3 in a row
    if (((bitboard >> 7) & (bitboard >> 14) & (bitboard >> 21)) != 0)
        score += 3;

    // Secondary diagonal check
    if (((bitboard >> 9) & (bitboard >> 18) & (bitboard >> 27) & (bitboard >> 36)) != 0) {
        return 100000;
    } else if (((oppBitboard >> 9) & (oppBitboard >> 18) & (oppBitboard >> 27) & (bitboard >> 36)) != 0) {
        return -100000;
    }
    
    // Secondary diagonal heuristic - 3 in a row
    if (((bitboard >> 9) & (bitboard >> 18) & (bitboard >> 27)) != 0)
        score += 3;


    if ((boards[0] | boards[1]) == 280371153272574)
        return 0;

    if (!depth)
        return score;

    return 420;
}


// The minimax algorithm
int minimax(int depth, bool isMaximising, double alpha, double beta) {
    count++;
    int result = evaluateBoard(boards[0], boards[1], depth);
    if (depth == 0 || result != 420)
        return result;


    if (isMaximising) {
        double bestScore = -INFINITY;
        for (int j = 0;  j < WIDTH; j++) {
            bool dpRes = dropPiece(j, YELLOW);
            if (!dpRes) {
                continue;
            }

            int score = minimax(depth - 1, false, alpha, beta);
            popPiece(j, YELLOW);
            if (score > bestScore) {
                bestScore = score;
            }
            if (alpha > score) {
                alpha = score;
            }
            if (beta <= alpha) {
                break;
            }
        }
        return bestScore;
    } else {
        double bestScore = INFINITY;
        for (int j = 0;  j < WIDTH; j++) {
            bool dpRes = dropPiece(j, RED);
            if (!dpRes) {
                continue;
            }

            int score = minimax(depth - 1, true, alpha, beta);
            popPiece(j, RED);
            if (score < bestScore) {
                bestScore = score;
            }
            if (beta < score) {
                beta = score;
            }
            if (beta <= alpha) {
                break;
            }
        }
        return bestScore;
    }
}


// Starts the minimax search.
int aiMove(int depth) {
    int bestMove = 0;
    double bestScore = -INFINITY;
    
    for (int j = 0;  j < WIDTH; j++) {
        bool dpRes = dropPiece(j, AI);
        if (!dpRes) {
            continue;
        }

        int score = minimax(depth, false, -INFINITY, INFINITY);
        popPiece(j, AI);
        if (score > bestScore) {
            bestScore = score;
            bestMove = j;
        }
        if (score >= 100000) {
            break;
        }
    }
    printf("Visited nodes: %llu\n", count);
    printf("\nBest score is %d\nBest move is %d\n", (int)bestScore, bestMove + 1);
    return bestMove;
}


int main() {
    printBoard();
    printf("Welcome to C4C, this is the bitboard AI implementation. Try and see if you can beat the AI!\n");
    int difficulty;
    printf("AI difficulty? 1-7 (1 looks 1 move ahead and 7 looks 7 moves ahead, but 7 is very slow.): ");
    scanf("%d", &difficulty);
    int aiFirst;
    printf("Do you want the AI to go first? 0 for no and 1 for yes: ");
    scanf("%d", &aiFirst);
    if (aiFirst) {
        currPlayer = AI;
    }
    while (true) {
        int move;
        int col;
        if (currPlayer == HUMAN) {
            printf("HUMAN, enter your column from 1-7: ");
            scanf("%d", &col);
            while (!dropPiece(col - 1, currPlayer) || col < 1 || col > 7) {
                printf("HUMAN, Column already full or out of bounds. Re-enter your column from 1-7: ");
                scanf("%d", &col);
            }
            currPlayer = AI;
            
        } else {
            clock_t start = clock();
            printf("AI is thinking...\n");
            move = aiMove(difficulty); // Depth is how many moves the AI looks into the future //
            clock_t end = clock();
            printf("Time took for AI's move: %lf\n", ((double)end - start) / CLOCKS_PER_SEC);
            dropPiece(move, AI);
            currPlayer = HUMAN;
        }
        printBoard();

        int result = evaluateBoard(boards[0], boards[1], 1);
        if (result == 100000) {
            printf("AI wins!\n");
            /* printBoard(); */
            break;
        } else if (result == -100000){
            printf("HUMAN wins!\n");
            /* printBoard(); */
            break;
        } else if (result == 0) {
            printf("DRAW!\n");
            /* printBoard(); */
            break;
        } else if (result == 420){
            continue;
        }

    }
}
