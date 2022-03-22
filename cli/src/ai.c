#include "../includes/logic.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARDLEN 64
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_RESET "\x1b[0m"

Player HUMAN = RED;
Player AI = YELLOW;

Player currPlayer = RED;    // Starting player
Boards boards = {0b0, 0b0}; // Starting board
uint8_t heights[] = {56, 57, 58, 59,
    60, 61, 62}; // Stores top piece for each column

void printBoard() {
    unsigned short column = 0;
    unsigned long long place = 1UL << 47;
    for (int i = 16; i < 64; i++) {
        if ((i % 8 == 7)) {
            place >>= 1;
            continue;
        }
        column++;
        printf("|");
        if ((boards.yellow & place) == place)
            printf(ANSI_COLOR_YELLOW " \u2584 " ANSI_COLOR_RESET);
        else if ((boards.red & place) == place)
            printf(ANSI_COLOR_RED " \u2584 " ANSI_COLOR_RESET);
        else
            printf(" . ");

        if (!(column % 7))
            printf("|\n");
        place >>= 1;
    }
    printf("-----------------------------\n ");
    for (int i = 1; i < WIDTH + 1; i++)
        printf(" %d  ", i);
    printf("\n");
}

int main() {
    int depth;
    printf("AI depth? (how many moves ahead it thinks): ");
    scanf("%d", &depth);
    printf("Try to beat the AI!\n");
    while (true) {
        if (currPlayer == HUMAN) {
            printBoard();
            int col;
            printf("Enter your move between 1-9: ");
            scanf("%d", &col);
            while (dropPiece(&boards.yellow, &boards.red, col - 1, HUMAN, heights) !=
                    0) {
                printf("Reenter your move between 1-9: ");
                scanf("%d", &col);
            }
        } else {
            clock_t start = clock();
            printf("AI is thinking...\n");
            dropPiece(&boards.yellow, &boards.red,
                    aiMove(&boards.yellow, &boards.red, depth, heights), AI,
                    heights);
            clock_t end = clock();
            printf("Time took for AI's move: %lf\n",
                    (double)(end - start) / CLOCKS_PER_SEC);
        }
        currPlayer = ((currPlayer == YELLOW) ? RED : YELLOW);
        int result = evaluateBoard(boards.yellow, boards.red, 1);
        if (result == 100000) {
            printf(ANSI_COLOR_YELLOW "AI WINS!\n" ANSI_COLOR_RESET);
            printBoard();
            return EXIT_SUCCESS;
        } else if (result == -100000) {
            printf(ANSI_COLOR_RED "HUMAN WINS!\n" ANSI_COLOR_RESET);
            printBoard();
            return EXIT_SUCCESS;
        } else if (result == 0) {
            printf("DRAW!\n");
            printBoard();
            return EXIT_SUCCESS;
        }
        if (result == 420) {
            continue;
        }
    }

    return EXIT_SUCCESS;
}
