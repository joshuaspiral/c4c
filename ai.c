#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
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
    DOUBLEWIN,
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

enum State currPlayer = HUMAN;


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

int getHeuristic(enum State board[HEIGHT][WIDTH], enum State piece) {
    int score;
    int center_col = WIDTH / 2;
    for (int i = 0; i < HEIGHT - 2; i++) {
        if (board[i][center_col] == piece) {
            score++;
        }
    }

    for (int i = 0; i < HEIGHT - 2; i++) {
        for (int j = 0; j < WIDTH - 2; j++) {
            if (board[i][j] == board[i + 1][j + 1]  && board[i][j] == piece) {
                if (board [i + 1][j + 1] == board[i + 2][j + 2]) {
                    score += 3;
                } else {
                    score += 2;
                }
            }
        }
    }

    for (int i = 0; i < HEIGHT - 2; i++) {
        for (int j = 2; j < WIDTH; j++) {
            /* Secondary diagonal check */
            if (board[i][j] == board[i + 1][j - 1] && board[i][j] == piece) {
                if (board [i + 1][j - 1] == board[i + 2][j - 2]) {
                    score += 3;
                } else {
                    score += 2;
                }
            }

            /* Vertical check */ 

        }
            
    }
    for (int i = 0; i < HEIGHT - 2; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == board[i + 1][j] && board[i][j] == piece) {
                if (board [i + 1][j] == board[i + 2][j] ) {
                    score += 3;
                } else {
                    score += 2;
                }
            }
        }
    }


    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH - 2; j++) {
            /* Horizontal check */
            if (board[i][j] == board[i][j + 1] && board[i][j + 1] == board[i][j + 2] && board[i][j] == piece) {
                if (board [i][j + 1] == board[i][j + 2]) {
                    score += 2;
                } else {
                    score += 3;
                }
            }
                
        }
    }
    return score;
}

enum Result evaluateBoard(enum State board[HEIGHT][WIDTH], enum State piece) {
    int wins = 0;
    for (int i = 0; i < HEIGHT - 3; i++) {
        for (int j = 0; j < WIDTH - 3; j++) {
            /* Primary diagonal check */
            if (board[i][j] == board[i + 1][j + 1] && board [i + 1][j + 1] == board[i + 2][j + 2] && board[i + 2][j + 2] == board[i + 3][j + 3] && board[i][j] != UNCLAIMED) {
                if (board[i][j] == piece) {
                    wins++;
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
                    wins++;
                } else {
                    return LOSE;
                }
            }


        }
            
    }
    for (int i = 0; i < HEIGHT - 3; i++) {
        for (int j = 0; j < WIDTH; j++) {
            /* Vertical check */ 
            if (board[i][j] == board[i + 1][j] && board [i + 1][j] == board[i + 2][j] && board[i + 2][j] == board[i + 3][j] && board[i][j] != UNCLAIMED) {
                if (board[i][j] == piece) {
                    wins++;
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
                    wins++;
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
    if (wins > 1) {
        return DOUBLEWIN;
    } else if (wins == 1){
        return WIN;
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
    } else if (result == DOUBLEWIN) {
        return 1000000;
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


int aiMove(int depth) {
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
            
        } else if (currPlayer == AI) {
            clock_t start = clock();
            printf("AI is thinking...\n");
            move = aiMove(difficulty); // Depth is how many moves the AI looks into the future //
            clock_t end = clock();
            printf("Time took for AI's move: %lf\n", ((double)end - start) / CLOCKS_PER_SEC);
            dropPiece(move, AI);
            currPlayer = HUMAN;
        }
        printBoard();

        enum Result result = evaluateBoard(board, AI);
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
