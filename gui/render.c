#include "render.h"
const unsigned short COLUMN_WIDTH = SCREENBOARD_WIDTH / WIDTH;
SDL_Color WHITE = {.r = 255, .g = 255, .b = 255, 255};

void set_pixel(SDL_Renderer *rend, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(rend, r,g,b,a);
	SDL_RenderDrawPoint(rend, x, y);
}

void draw_circle(SDL_Renderer *surface, int n_cx, int n_cy, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	// if the first pixel in the screen is represented by (0,0) (which is in sdl)
	// remember that the beginning of the circle is not in the middle of the pixel
	// but to the left-top from it:

	double error = (double)-radius;
	double x = (double)radius - 0.5;
	double y = (double)0.5;
	double cx = n_cx - 0.5;
	double cy = n_cy - 0.5;

	while (x >= y)
	{
		set_pixel(surface, (int)(cx + x), (int)(cy + y), r, g, b, a);
		set_pixel(surface, (int)(cx + y), (int)(cy + x), r, g, b, a);

		if (x != 0)
		{
			set_pixel(surface, (int)(cx - x), (int)(cy + y), r, g, b, a);
			set_pixel(surface, (int)(cx + y), (int)(cy - x), r, g, b, a);
		}

		if (y != 0)
		{
			set_pixel(surface, (int)(cx + x), (int)(cy - y), r, g, b, a);
			set_pixel(surface, (int)(cx - y), (int)(cy + x), r, g, b, a);
		}

		if (x != 0 && y != 0)
		{
			set_pixel(surface, (int)(cx - x), (int)(cy - y), r, g, b, a);
			set_pixel(surface, (int)(cx - y), (int)(cy - x), r, g, b, a);
		}

		error += y;
		++y;
		error += y;

		if (error >= 0)
		{
			--x;
			error -= x;
			error -= x;
		}
		/*
		// sleep for debug
		SDL_RenderPresent(gRenderer);
		std::this_thread::sleep_for(std::chrono::milliseconds{ 100 });
		*/
	}
}
// Two functions from here: https://gist.github.com/derofim/912cfc9161269336f722

void renderGrid(SDL_Renderer *renderer, SDL_Color *color) {
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);
    SDL_RenderDrawLine(renderer, COLUMN_WIDTH, (SCREEN_HEIGHT - SCREENBOARD_HEIGHT) / 2, COLUMN_WIDTH, SCREEN_HEIGHT - ((SCREEN_HEIGHT - SCREENBOARD_HEIGHT) / 2));
    for (int j = 2; j < WIDTH + 2; j++) {
        SDL_RenderDrawLine(renderer, j * COLUMN_WIDTH, (SCREEN_HEIGHT - SCREENBOARD_HEIGHT) / 2, j * COLUMN_WIDTH, SCREEN_HEIGHT - ((SCREEN_HEIGHT - SCREENBOARD_HEIGHT) / 2));
    }
}

void renderPiece(SDL_Renderer *renderer, Player player, int row, int col) {
    if (player == YELLOW)
        draw_circle(renderer, (col + 1) * COLUMN_WIDTH + 50, ((row + 1) * COLUMN_WIDTH + 50), COLUMN_WIDTH / 2 - 10, 255, 255, 0, 255);
    else
        draw_circle(renderer, (col + 1) * COLUMN_WIDTH + 50, ((row + 1) * COLUMN_WIDTH + 50), COLUMN_WIDTH / 2 - 10, 255, 0, 0, 255);
}

void renderBoard(SDL_Renderer *renderer, unsigned long long yellow, unsigned long long red) {
    renderGrid(renderer, &WHITE);
    unsigned short column = 0;
    unsigned long long place = 1UL << 47;
    for (int i = 16; i < 64; i++) {
        if ((i % 8 == 7)) {
            place >>= 1;
            continue;
        }
        column++;
        if ((yellow & place) == place)
            renderPiece(renderer, YELLOW, (i / 8) - 2, (i % 8));
        else if ((red & place) == place)
            renderPiece(renderer, RED, (i / 8) - 2, (i % 8));

        place >>= 1;
    }
}

void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text,
        TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect) {
    int text_width;
    int text_height;
    SDL_Surface *surface;
    SDL_Color textColor = {255, 255, 255, 0};

    surface = TTF_RenderText_Solid(font, text, textColor);
    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    text_width = surface->w;
    text_height = surface->h;
    SDL_FreeSurface(surface);
    rect->x = x;
    rect->y = y;
    rect->w = text_width;
    rect->h = text_height;
}

void renderMessage(SDL_Renderer *renderer, char *message) {
    SDL_Rect rect;
    SDL_Texture *texture;
    TTF_Font* FiraCode = TTF_OpenFont("FiraCode-Regular.ttf", 12);
    get_text_and_rect(renderer, 600, 700, message, FiraCode, &texture, &rect);
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}
