#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define HEIGHT 6
#define WIDTH 7
#define ANSI_COLOR_RED     "\x1b[31m" 
#define ANSI_COLOR_YELLOW  "\x1b[33m" 
#define ANSI_COLOR_RESET   "\x1b[0m" 

enum Player {
    YELLOW,
    RED,
};

enum Player const AI = YELLOW;
enum Player const HUMAN = RED;

uint8_t bitPowers[8] = {1, 2, 4, 8, 16, 32, 64, 128};

uint8_t yBoard[] = {0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0};
uint8_t rBoard[] = {0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0};
// Each 0 is a column
uint8_t heights[] = {0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0};
// Stores top piece for each column

enum Player currPlayer = RED; // Starting player

// Drops the piece onto a column
bool dropPiece(int col, enum Player piece) {
    // Check if height of next piece >= max height
    if (heights[col] >= HEIGHT)
        return false;
    // Check piece
    if (piece == YELLOW)
        // Calculates new column using the height of the next piece
        yBoard[col] ^= 1 << heights[col];
    else
        rBoard[col] ^= 1 << heights[col];
    // Increase for next use
    heights[col]++;
    return true;
}

// Pops the last played piece of a column
void popPiece(int col, enum Player piece) {
    heights[col]--;
    if (piece == YELLOW)
        yBoard[col] &= ~(1 << heights[col]);
    else
        rBoard[col] &= ~(1 << heights[col]);
}


// TODO still wrong direction
// Prints and formats the board with ANSI colors.
void printBoard() {
    for (int j = 0; j < WIDTH; j++) {
        for (int i = 0; i < 6; i++) {
            printf("|");
            if (yBoard[j] & bitPowers[i])
                printf(ANSI_COLOR_YELLOW " Y " ANSI_COLOR_RESET);
            else if (rBoard[j] & bitPowers[i])
                printf(ANSI_COLOR_RED " R " ANSI_COLOR_RESET);
            else
                printf(" . ");
        }
        printf("|\n");
    }
}


// Evaluates the board's current state for enum Player piece. Returns integer score.
int evaluateBoard(uint8_t board[], uint8_t oppBoard[], enum Player piece) {
    int score = 0;
    int centerCol = WIDTH / 2;
    for (int i = 0; i < 6; i++)
        if (board[centerCol] & bitPowers[i])
            score++;
    
    // Column check
    for (int j = 0; j < WIDTH; j++) {
        if (piece == YELLOW) {
            // Checks if the column has 1111 or
            if (board[j] == (0b1111) || board[j] == (0b1111 << 1) || board[j] == (0b1111 << 2)) {
                return 100000;
            } else if ((oppBoard[j] == (0b1111) || oppBoard[j] == (0b1111 << 1) || oppBoard[j] == (0b1111 << 2))) {
                return -100000;
            } else if (board[j] == (0b111) || board[j] == (0b111 << 1) || board[j] == (0b111 << 2) || board[j] == (0b111 << 3)) {
                score += 3;
            }
        }
    }
    
    // Horizontal check
    for (int j = 0; j < WIDTH - 3; j++) {
        if (board[j] & (board[j + 1]) & (board[j + 2]) & (board[j + 3])) {
            return 100000;
        } else if (oppBoard[j] & (oppBoard[j + 1]) & (oppBoard[j + 2]) & (oppBoard[j + 3])) {
            return -100000;
        } else if ((board[j] & (board[j + 1]) & (board[j + 2])) || (board[j + 1] & (board[j + 2]) & (board[j + 3]))) {
            score += 3;
        }
    }

    // Primary diagonal check
    for (int j = 0; j < WIDTH - 3; j++) {
        if (board[j] & (board[j + 1] << 1) & (board[j + 2] << 2) & (board[j + 3] << 3)) {
            return 100000;
        } else if (oppBoard[j] & (oppBoard[j + 1] << 1) & (oppBoard[j + 2] << 2) & (oppBoard[j + 3] << 3)) {
            return -100000;
        } else if ((board[j] & (board[j + 1] << 1) & (board[j + 2] << 2)) || (board[j + 1] << 1) & (board[j + 2] << 2) & (board[j + 3] << 3)) {
            score += 3;
        }

        // Secondary diagonal check
        if (board[j] & (board[j + 1] >> 1) & (board[j + 2] >> 2) & (board[j + 3] >> 3)) {
            return 100000;
        } else if (oppBoard[j] & (oppBoard[j + 1] >> 1) & (oppBoard[j + 2] >> 2) & (oppBoard[j + 3] >> 3)) {
            return -100000;
        } else if ((board[j] & (board[j + 1] >> 1) & (board[j + 2] >> 2)) || (board[j + 1] >> 1) & (board[j + 2] >> 2) & (board[j + 3] >> 3)) {
            score += 3;
        }
    }


    bool draw = true;
    for (int j = 0; j < WIDTH; j++) {
        // Check if current column is full
        draw = (yBoard[j] | rBoard[j]) == 255;
    }

    if (draw)
        return 0;

    if (score > 0)
        return score;

    return 420;
}


// The minimax algorithm.
int minimax(int depth, bool isMaximising, double alpha, double beta) {
    int result = evaluateBoard(yBoard, rBoard, AI);
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
        printf("Score for column %d is %d\n", j + 1, score);
        popPiece(j, AI);
        if (score > bestScore) {
            bestScore = score;
            bestMove = j;
        }
        if (score >= 100000) {
            break;
        }
    }
    printf("\nBest score is %d\nBest move is %d\n", (int)bestScore, bestMove + 1);
    return bestMove;
}


int main() {
    int difficulty;
    printf("AI difficulty? 1-7 (1 looks 1 move ahead and 7 looks 7 moves ahead, but 7 is very slow.): ");
    scanf("%d", &difficulty);
    int aiFirst;
    printf("Do you want the AI to go first? 0 for no and 1 for yes: ");
    scanf("%d", &aiFirst);
    if (aiFirst) {
        currPlayer = AI;
    }
    printBoard();
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

        int result = evaluateBoard(yBoard, rBoard, AI);
        printf("Result: %d\n", result);
        if (result == 100000) {
            printf("AI wins!\n");
            printBoard();
            break;
        } else if (result == -100000){
            printf("HUMAN wins!\n");
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
