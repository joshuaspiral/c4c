#include "../includes/render.h"
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


void renderMessage(SDL_Renderer *renderer, char *message) {
    if(TTF_Init()==-1) {
    printf("TTF_Init: %s\n", TTF_GetError());
    exit(2);
    }
    TTF_Font* FiraCode = TTF_OpenFont("./FiraCode-Regular.ttf", 40);
    if (FiraCode == NULL) {
        printf("Error loading font: %s\n", TTF_GetError());
        exit(0);
    }
    SDL_Surface* surfaceMessage =
        TTF_RenderText_Solid(FiraCode, message, WHITE); 

    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_Rect Message_rect; //create a rect
    Message_rect.x = 250;  //controls the rect's x coordinate 
    Message_rect.y = 700; // controls the rect's y coordinte
    Message_rect.w = 400; // controls the width of the rect
    Message_rect.h = 100; // controls the height of the rect

    // (0,0) is on the top left of the window/screen,
    // think a rect as the text's box,
    // that way it would be very simple to understand

    // Now since it's a texture, you have to put RenderCopy
    // in your game loop area, the area where the whole code executes

    // you put the renderer's name first, the Message,
    // the crop size (you can ignore this if you don't want
    // to dabble with cropping), and the rect which is the size
    // and coordinate of your texture
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

    // Don't forget to free your surface and texture
    /* SDL_FreeSurface(surfaceMessage); */
    /* SDL_DestroyTexture(Message); */
}
