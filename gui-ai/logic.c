#include "logic.h"
#define BOARDLEN 64


// Drops the piece onto a column
bool dropPiece(unsigned long long *yellow, unsigned long long *red, int col, Player player, uint8_t ((*heights))[7]) {
    // Check if height of next piece is in the board boundaries
    if ((*heights)[col] < 16 || col < 0 || col > 6)
        return false;
    // Check piece
    if (player == YELLOW)
        // Calculates new column using the height of the next piece
        *yellow ^= 1ULL << (BOARDLEN - 1 - (*heights)[col]);
    else
        *red ^= 1ULL << (BOARDLEN - 1 - (*heights)[col]);
    // Move for next use
    (*heights)[col] -= 8;
    return true;
}

// Pops the last played piece of a column
void popPiece(unsigned long long *yellow, unsigned long long *red, int col, Player player, uint8_t (*heights)[7]) {
    (*heights)[col] += 8;
    if (player == YELLOW)
        *yellow &= ~(1UL << (BOARDLEN - 1 - (*heights)[col]));
    else
        *red &= ~(1UL << (BOARDLEN - 1 - (*heights)[col]));
}


// Evaluates the board's current state for enum Player piece. Returns integer score.
int evaluateBoard(unsigned long long bitboard, unsigned long long oppBitboard, int depth) {
    int score = 0;
    // Center heuristic TODO
    unsigned long long shifted = 1L << (63 - 19);
    for (int i = 0; i < HEIGHT; i++) {
        if (bitboard & shifted)
            score++;
        shifted >>= 8;
    }
    
    // Vertical check
    if ((bitboard & (bitboard >> 8) & (bitboard >> 16) & (bitboard >> 24)) != 0) {
        return 100000;
    } else if ((oppBitboard & (oppBitboard >> 8) & (oppBitboard >> 16) & (oppBitboard >> 24)) != 0) {
        return -100000; 
    } 

    // Vertical heuristic - 3 in a row
    if ((bitboard & (bitboard >> 8) & (bitboard >> 16)) != 0)
        score += 3;

    // Horizontal check
    if ((bitboard & (bitboard >> 1) & (bitboard >> 2) & (bitboard >> 3)) != 0) {
        return 100000;
    } else if ((oppBitboard & (oppBitboard >> 1) & (oppBitboard >> 2) & (oppBitboard >> 3)) != 0) {
        return -100000;
    }
    
    // Horizontal heuristic - 3 in a row
    if ((bitboard & (bitboard >> 1) & (bitboard >> 2)) != 0)
        score += 3;

    // Primary diagonal check (/)
    if ((bitboard & (bitboard >> 7) & (bitboard >> 14) & (bitboard >> 21)) != 0) {
        return 100000;
    } else if ((oppBitboard & (oppBitboard >> 7) & (oppBitboard >> 14) & (oppBitboard >> 21)) != 0) {
        return -100000;
    }
    
    // Primary diagonal heuristic - 3 in a row
    if ((bitboard & (bitboard >> 7) & (bitboard >> 14) & (bitboard >> 21)) != 0)
        score += 3;

    // Secondary diagonal check (\)
    if ((bitboard & (bitboard >> 9) & (bitboard >> 18) & (bitboard >> 27)) != 0) {
        return 100000;
    } else if ((oppBitboard & (oppBitboard >> 9) & (oppBitboard >> 18) & (oppBitboard >> 27)) != 0) {
        return -100000;
    }
    
    // Secondary diagonal heuristic - 3 in a row
    if ((bitboard & (bitboard >> 9) & (bitboard >> 18) & (bitboard >> 27)) != 0)
        score += 3;


    if ((bitboard | oppBitboard) == 280371153272574) // if board is maxed out (excluding top two rows and last column)
        return 0;

    if (!depth)
        return score;

    return 420;
}

int minimax(unsigned long long *yellow, unsigned long long *red, uint8_t (*heights)[WIDTH], int depth, bool isMaximising, double alpha, double beta) {
    int result = evaluateBoard(*yellow, *red, depth);
    if (depth == 0 || result != 420)
        return result;


    if (isMaximising) {
        double bestScore = -INFINITY;
        for (int j = 0;  j < WIDTH; j++) {
            bool dpRes = dropPiece(yellow, red, j, YELLOW, heights);
            if (!dpRes) {
                continue;
            }

            int score = minimax(yellow, red, heights, depth - 1, false, alpha, beta);
            popPiece(yellow, red, j, YELLOW, heights);

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
            bool dpRes = dropPiece(yellow, red, j, RED, heights);
            if (!dpRes) {
                continue;
            }

            int score = minimax(yellow, red, heights, depth - 1, true, alpha, beta);
            popPiece(yellow, red, j, RED, heights);

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

int aiMove(unsigned long long *yellow, unsigned long long *red, int depth, uint8_t (*heights)[WIDTH]) {
    int bestMove = 0;
    double bestScore = -INFINITY;
    
    for (int j = 0;  j < WIDTH; j++) {
        bool dpRes = dropPiece(yellow, red, j, YELLOW, heights);
        if (!dpRes) {
            continue;
        }

        int score = minimax(yellow, red, heights, depth - 1, false, -INFINITY, INFINITY);
        popPiece(yellow, red, j, YELLOW, heights);
        if (score > bestScore) {
            bestScore = score;
            bestMove = j;
        }
        if (score >= 100000) {
            break;
        }
    }
    return bestMove;
}
