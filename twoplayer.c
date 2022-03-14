#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#define HEIGHT 6
#define WIDTH 7

enum State {
    YELLOW,
    RED,
    UNCLAIMED
};

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

enum State currPlayer = YELLOW;


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

void printBoard(enum State board[HEIGHT][WIDTH]) {
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
    for (int j = 1; j < WIDTH + 1; j++) {
        printf("  %d ", j);
    }
    printf("\n");
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
int main() {
    printBoard(board);
    while (true) {
        int col;

        printf("%c, enter your column from 1-7: ", ((currPlayer == RED) ? 'Y' : 'R'));
        scanf("%d", &col);
        bool dpResult = dropPiece(col - 1, currPlayer);
        while (!dpResult) {
            printf("%c, Column already full, re-enter your column from 1-7: ", ((currPlayer == RED) ? 'Y' : 'R'));
            scanf("%d", &col);
            dpResult = dropPiece(col - 1, currPlayer);
        }


        enum Result result = evaluateBoard(board, YELLOW);
        if (result == WIN) {
            printf("Yellow wins!\n");
            printBoard(board);
            break;
        } else if (result == LOSE){
            printf("Red wins!\n");
            printBoard(board);
            break;
        } else if (result == DRAW) {
            printf("DRAW!\n");
            printBoard(board);
            break;
        }

        currPlayer = ((currPlayer == YELLOW) ? RED : YELLOW);
        printBoard(board);
    }
}
