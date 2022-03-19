#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <unistd.h>
#include "logic.h"
#include "render.h"
#define BOARDLEN 64

Player HUMAN = RED;
Player AI = YELLOW;

Player currPlayer = RED; // Starting player
Boards boards = {0b0, 0b0}; // Starting board
uint8_t heights[] = {56, 57, 58, 59, 60, 61, 62}; // Stores top piece for each column

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Could not initialise SDL2. Error:  %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window = SDL_CreateWindow("Connect Four", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error:  %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer Error:  %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Event e;
    int gameover = 0;
    while (true) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    exit(0);
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (!gameover) {
                        if (currPlayer == HUMAN) {
                            if (!dropPiece(&boards.yellow, &boards.red, e.button.x / (SCREENBOARD_WIDTH / WIDTH) - 1, HUMAN, &heights)) {
                                break;
                            }
                        }                         currPlayer = ((currPlayer == YELLOW) ? RED : YELLOW);
                    } else {
                        SDL_DestroyWindow(window);
                        SDL_Quit();
                        return EXIT_SUCCESS;
                    }
                    break;

                default: 
                    {
                        if (currPlayer == AI) {
                            dropPiece(&boards.yellow, &boards.red, aiMove(&boards.yellow, &boards.red, 7, &heights), AI, &heights);
                            currPlayer = ((currPlayer == YELLOW) ? RED : YELLOW);
                        }
                    }
            }
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);
        renderBoard(renderer, boards.yellow, boards.red);
            
        int result = evaluateBoard(boards.yellow, boards.red, 1);
        if (result != 420)
            gameover = 1;
        if (result == 100000) {
            renderMessage(renderer, "AI WINS!");
            SDL_RenderPresent(renderer);
        } else if (result == -100000){
            renderMessage(renderer, "HUMAN WINS!");
            SDL_RenderPresent(renderer);
        } else if (result == 0) {
            renderMessage(renderer, "DRAW!");
            SDL_RenderPresent(renderer);
        }         
        SDL_RenderPresent(renderer);
        if (result == 420){
            continue;
        }

    }

    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
