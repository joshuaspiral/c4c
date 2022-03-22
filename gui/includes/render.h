#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "logic.h"
#define RENDER_H
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 800
#define SCREENBOARD_WIDTH 700
#define SCREENBOARD_HEIGHT 600
#define ANSI_COLOR_RED     "\x1b[31m" 
#define ANSI_COLOR_YELLOW  "\x1b[33m" 
#define ANSI_COLOR_RESET   "\x1b[0m" 
#define WIDTH 7
#define HEIGHT 6


void set_pixel(SDL_Renderer *rend, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void draw_circle(SDL_Renderer *surface, int n_cx, int n_cy, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void renderGrid(SDL_Renderer *renderer, SDL_Color *color);
void renderPiece(SDL_Renderer *renderer, Player player, int row, int col);
void renderBoard(SDL_Renderer *renderer, unsigned long long yellow, unsigned long long red);
void renderMessage(SDL_Renderer *renderer, char *message);
