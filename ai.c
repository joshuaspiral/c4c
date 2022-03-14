#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#define HEIGHT 6
#define WIDTH 7

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

enum State board[HEIGHT][WIDTH] = {
    {UNCLAIMED, UNCLAIMED, UNCLAIMED, UNCLAIMED, UNCLAIMED, UNCLAIMED, UNCLAIMED},
    {UNCLAIMED, UNCLAIMED, UNCLAIMED, UNCLAIMED, UNCLAIMED, UNCLAIMED, UNCLAIMED},
    {UNCLAIMED, UNCLAIMED, UNCLAIMED, UNCLAIMED, UNCLAIMED, UNCLAIMED, UNCLAIMED},
    {UNCLAIMED, UNCLAIMED, UNCLAIMED, UNCLAIMED, UNCLAIMED, UNCLAIMED, UNCLAIMED},
    {UNCLAIMED, UNCLAIMED, UNCLAIMED, UNCLAIMED, UNCLAIMED, UNCLAIMED, UNCLAIMED},
    {UNCLAIMED, UNCLAIMED, UNCLAIMED, UNCLAIMED, UNCLAIMED, UNCLAIMED, UNCLAIMED},
};

enum State currPlayer = RED;


bool dropPiece(int col, enum State piece) {
    if (board[0][col] != UNCLAIMED) {
        return false;
    } else {
        for (int i = HEIGHT; i >= 0; i--) {
            if (board[i][col] == UNCLAIMED) {
                board[i][col] = piece;
                return true;
            }
        }
        return false;
    }
}

void printBoard() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("|");
            if (board[i][j] == UNCLAIMED) {
                printf("   ");
            } else if (board[i][j] == YELLOW) {
                printf(" Y ");
            } else {
                printf(" R ");
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

int getHeuristic(enum State board[HEIGHT][WIDTH], enum State piece) {
    int score;
    for (int i = 0; i < HEIGHT - 3; i++) {
        for (int j = 0; j < WIDTH - 3; j++) {
            if (board[i][j] == board[i + 1][j + 1] && board [i + 1][j + 1] == board[i + 2][j + 2] && board[i][j] == piece) {
                score += 4;
            }
        }
    }

    for (int i = 0; i < HEIGHT - 3; i++) {
        for (int j = 3; j < WIDTH; j++) {
            /* Secondary diagonal check */
            if (board[i][j] == board[i + 1][j - 1] && board [i + 1][j - 1] == board[i + 2][j - 2] && board[i][j] == piece) {
                score += 4;
            }

            /* Vertical check */ 
            if (board[i][j] == board[i + 1][j] && board [i + 1][j] == board[i + 2][j] && board[i][j] == piece) {
                score += 4;
            }

        }
            
    }

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH - 3; j++) {
            /* Horizontal check */
            if (board[i][j] == board[i][j + 1] && board [i][j + 1] == board[i][j + 2] && board[i][j] == piece) {
                score += 4;
            }
                
        }
    }
    return score;
}

enum Result evaluateBoard(enum State board[HEIGHT][WIDTH], enum State piece) {
    for (int i = 0; i < HEIGHT - 3; i++) {
        for (int j = 0; j < WIDTH - 3; j++) {
            /* Primary diagonal check */
            if (board[i][j] == board[i + 1][j + 1] && board [i + 1][j + 1] == board[i + 2][j + 2] && board[i + 2][j + 2] == board[i + 3][j + 3] && board[i][j] != UNCLAIMED) {
                if (board[i][j] == piece) {
                    return WIN;
                } else {
                    return LOSE;
                }
            }

        }
            
    }

    for (int i = 0; i < HEIGHT - 3; i++) {
        for (int j = 3; j < WIDTH; j++) {
            /* Secondary diagonal check */
            if (board[i][j] == board[i + 1][j - 1] && board [i + 1][j - 1] == board[i + 2][j - 2] && board[i + 2][j - 2] == board[i + 3][j - 3] && board[i][j] != UNCLAIMED) {
                if (board[i][j] == piece) {
                    return WIN;
                } else {
                    return LOSE;
                }
            }

            /* Vertical check */ 
            if (board[i][j] == board[i + 1][j] && board [i + 1][j] == board[i + 2][j] && board[i + 2][j] == board[i + 3][j] && board[i][j] != UNCLAIMED) {
                if (board[i][j] == piece) {
                    return WIN;
                } else {
                    return LOSE;
                }
                
            }

        }
            
    }

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH - 3; j++) {
            /* Horizontal check */
            if (board[i][j] == board[i][j + 1] && board [i][j + 1] == board[i][j + 2] && board[i][j + 2] == board[i][j + 3] && board[i][j] != UNCLAIMED) {
                if (board[i][j] == piece) {
                    return WIN;
                } else {
                    return LOSE;
                }
                
            }
        }
    }

    for (int i = 0; i < HEIGHT - 3; i++) {
        for (int j = 0; j < WIDTH; j++) {
        }
    }

    bool draw = true;
    for (int i = 0; i < HEIGHT - 3; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == UNCLAIMED) {
                draw = false;
            }
        }
    }
    if (draw) {
        return DRAW;
    }

    return CONTINUE;

}

void popPiece(int col) {
    for (int i = 0; i < HEIGHT; i++) {
        if (board[i][col] != UNCLAIMED) {
            board[i][col] = UNCLAIMED;
            break;
        }
    }
}

int minimax(int depth, bool isMaximising, double alpha, double beta) {
    enum Result result = evaluateBoard(board, AI);
    if (depth == 0) {
        return getHeuristic(board, AI);
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
            bool dpRes = dropPiece(j, AI);
            if (!dpRes) {
                continue;
            }

            int score = minimax(depth - 1, false, alpha, beta);
            popPiece(j);
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
            bool dpRes = dropPiece(j, HUMAN);
            if (!dpRes) {
                continue;
            }

            int score = minimax(depth - 1, true, alpha, beta);
            popPiece(j);
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


void aiMove(int depth) {
    int bestMove;
    double bestScore = -INFINITY;
    
    for (int j = 0;  j < WIDTH; j++) {
        bool dpRes = dropPiece(j, AI);
        if (!dpRes) {
            continue;
        }

        int score = minimax(depth, false, -INFINITY, INFINITY);
        popPiece(j);
        if (score > bestScore) {
            bestScore = score;
            bestMove = j;
        }
    }
    dropPiece(bestMove, AI);
}


int main() {
    int count = 0;
    printBoard();
    while (true) {
        if (currPlayer == HUMAN) {
            int col;

            printf("HUMAN, enter your column from 1-7: ");
            scanf("%d", &col);
            bool dpResult = dropPiece(col - 1, currPlayer);
            while (!dpResult || col < 1 || col > 7) {
                printf("HUMAN, Column already full or out of bounds. Re-enter your column from 1-7: ");
                scanf("%d", &col);
                dpResult = dropPiece(col - 1, currPlayer);
            }
            currPlayer = AI;
            
        } else if (currPlayer == AI) {
            printf("AI is thinking...\n");
            aiMove(6); // Depth is how many moves the AI looks into the future //
            currPlayer = HUMAN;
        }
        printBoard();

        enum Result result = evaluateBoard(board, AI);
        if (result == WIN) {
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