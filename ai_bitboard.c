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

enum State {
    YELLOW,
    RED,
    UNCLAIMED
};

enum State const AI = YELLOW;
enum State const HUMAN = RED;

enum Result {
    WIN,
    LOSE,
    DRAW,
    CONTINUE
};


uint8_t yBoard[] = {0};
uint8_t rBoard[] = {0};
// Each 0 is a column

enum State currPlayer = YELLOW; // Starting player

bool dropPiece(int col, enum State piece, uint8_t board[]) {
    if (yBoard[col] >> 5 || rBoard[col] >> 5)
        return false;
    // Check if column is full
    for (int i = 0; i <= 7; i++) {
        // Check from down up if placing it there is ok
        if ((1 << i) & 0) {
            board[col] = ((1 << i) ^ 1);
            return true;
        }
    }
    // I don't know how you got here...
    return false;
}

// TODO
void popPiece(int col, uint8_t board[]) {
    for (int i = 0; i < HEIGHT; i++) {
        if (board[i][col] != UNCLAIMED) {
            board[i][col] = UNCLAIMED;
            break;
        }
    }
}

void printBoard() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("|");
            if (board[i][j] == UNCLAIMED) {
                printf("   ");
            } else if (board[i][j] == YELLOW) {
                printf(ANSI_COLOR_YELLOW " Y " ANSI_COLOR_RESET);
            } else {
                printf(ANSI_COLOR_RED " R " ANSI_COLOR_RESET);
            }
        }
        printf("|");
        printf("\n");
    }
    printf("-----------------------------\n");
    for (int j = 1; j < WIDTH + 1; j++) {
        printf("  %d ", j);
    }
    printf("\n\n");
}

int getHeuristic(enum State piece) {
    return 1;
}

// I really did not want to use oppBoard :(
enum Result evaluateBoard(uint8_t board[], uint8_t oppBoard[], enum State piece) {
    // Column check
    for (int j = 0; j < WIDTH; j++) {
        if (piece == YELLOW) {
            // Checks if the column has 1111 or
            if (board[j] == (0b1111) || board[j] == (0b1111 << 1) || board[j] == (0b1111 << 2)) {
                return WIN;
            } else if ((oppBoard[j] == (0b1111) || oppBoard[j] == (0b1111 << 1) || oppBoard[j] == (0b1111 << 2))) {
                return LOSE;
            }
                
        }
    }
    
    // Horizontal check TODO

    // Primary diagonal check
    for (int j = 0; j < WIDTH - 1; j+=3) {
        if ((board[j] & (board[j + 1] << 1) & (board[j + 2] << 2) & (board[j + 3] << 3)) {
            return WIN;
        } else if ((oppBoard[0] & (oppBoard[j + 1] << 1) & (oppBoard[j + 2] << 2) & (oppBoard[j + 3] << 3)) {
            return LOSE;
        }
    }

    // Secondary diagonal check
    for (int j = 0; j < WIDTH - 1; j+=3) {
        if ((board[0] & (board[j + 1] >> 1) & (board[j + 2] >> 2) & (board[j + 3] >> 3)) {
            return WIN;
        } else if ((oppBoard[0] & (oppBoard[j + 1] >> 1) & (oppBoard[j + 2] >> 2) & (oppBoard[j + 3] >> 3)) {
            return LOSE;
        }
    }


    bool draw = true;
    for (int j = 0; j < WIDTH; j++) {
        // Check if current column is full
        draw = ((yBoard[j] | rBoard[j]) == 255);
    }

    if (draw) {
        return DRAW;
    }

    return CONTINUE;

}


int minimax(int depth, bool isMaximising, double alpha, double beta) {
    enum Result result = evaluateBoard(yBoard, rBoard, AI);
    if (depth == 0) {
        return getHeuristic(AI);
    }
    if (result == WIN) {
        return 100000;
    } else if (result == LOSE) {
        return -100000;
    } else if (result == DRAW) {
        return 0;
    } 


    if (isMaximising) {
        double bestScore = -INFINITY;
        for (int j = 0;  j < WIDTH; j++) {
            bool dpRes = dropPiece(j, YELLOW, yBoard);
            if (!dpRes) {
                continue;
            }

            int score = minimax(depth - 1, false, alpha, beta);
            popPiece(j, yBoard);
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
            bool dpRes = dropPiece(j, RED, rBoard);
            if (!dpRes) {
                continue;
            }

            int score = minimax(depth - 1, true, alpha, beta);
            popPiece(j, rBoard);
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


int aiMove(int depth) {
    int bestMove;
    double bestScore = -INFINITY;
    
    for (int j = 0;  j < WIDTH; j++) {
        bool dpRes = dropPiece(j, YELLOW, yBoard);
        if (!dpRes) {
            continue;
        }

        int score = minimax(depth, false, -INFINITY, INFINITY);
        printf("Score for column %d is %d\n", j + 1, score);
        popPiece(j, yBoard);
        if (score > bestScore) {
            bestScore = score;
            bestMove = j;
        }
        if (score >= 100000) {
            break;
        }
    }
    printf("\nBest score is %d\nBest move is %d\n", (int)bestScore, bestMove);
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
            while (!dropPiece(col - 1, currPlayer, rBoard) || col < 1 || col > 7) {
                printf("HUMAN, Column already full or out of bounds. Re-enter your column from 1-7: ");
                scanf("%d", &col);
            }
            currPlayer = AI;
            
        } else if (currPlayer == AI) {
            clock_t start = clock();
            printf("AI is thinking...\n");
            move = aiMove(difficulty); // Depth is how many moves the AI looks into the future //
            clock_t end = clock();
            printf("Time took for AI's move: %lf\n", ((double)end - start) / CLOCKS_PER_SEC);
            dropPiece(move, AI, yBoard);
            currPlayer = HUMAN;
        }
        printBoard();

        enum Result result = evaluateBoard(AI);
        if (result == WIN || result == DOUBLEWIN) {
            printf("AI wins!\n");
            printBoard();
            break;
        } else if (result == LOSE){
            printf("HUMAN wins!\n");
            printBoard();
            break;
        } else if (result == DRAW) {
            printf("DRAW!\n");
            printBoard();
            break;
        } else {
            continue;
        }

    }
}
