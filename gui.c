#include <stdio.h>
#include <SDL.h>
#include <SDL_video.h>
#include "Chess.h"
#define WIN_W 500
#define WIN_H 500

#define IMG_W 240
#define IMG_H 296

// Genral settings
extern int MINIMAX_DEPTH;
extern int GAME;
extern int SETTINGS;
extern int GUI_MODE; // '0' - application runs in 'Console mode', '1' - 'Gui mode'
extern int TWO_PLAYERS_MODE; // '1' - two players mode, '0' - player vs. AI mode
extern int WHITE_TURN; // 0 - black, 1- white
extern int PLAYER_WHITE; //???
extern char board[BOARD_SIZE][BOARD_SIZE];


// Game setiings
extern int WHITE_CR_ENABLE; // ??? what to do in case the game is loaded?
extern int BLACK_CR_ENABLE;
extern int WHITE_CL_ENABLE;
extern int BLACK_CL_ENABLE;

extern int CHECK_ON_WHITE;
extern int CHECK_ON_BLACK;


SDL_Surface* window = NULL;
SDL_Surface* background = NULL;

SDL_Surface *load_image( char *filename ) 
{
    //Temporary storage for the image that's loaded
    SDL_Surface* loadedImage = NULL;
    
    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;
	  //Load the image
    loadedImage = SDL_LoadBMP(filename);
	 //If nothing went wrong in loading the image
    if( loadedImage != NULL )
    {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat(loadedImage);
        
        //Free the old image
        SDL_FreeSurface(loadedImage);
    }
	//Return the optimized image
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    //Make a temporary rectangle to hold the offsets
    SDL_Rect offset;
    
    //Give the offsets to the rectangle
    offset.x = x;
    offset.y = y;  
	//Blit the surface
    SDL_BlitSurface(source, NULL, destination, &offset);
}

int init(void)
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return 1;    
    }
    
    //Set up the screen
    screen = SDL_SetVideoMode( WIN_W, WIN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF );
    
    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return 1;    
    }
    
    //Set the window caption
    SDL_WM_SetCaption( "Chess Prog", NULL );
    
    //If everything initialized fine
    return 0;
}

int play_gui(void) {

	if ( !init() ) return 1;
	SDL_Event event;
 	SDL_Rect quit_button = {225, 400, 50, 50};
//	SDL_Rect imgrect = {0, 0, IMG_W, IMG_H};
 */	
//	SDL_Surface *background = SDL_DisplayFormat(SDL_LoadBMP("background.bmp"));
	SDL_WM_SetCaption("Chess Prog", NULL);
	int quit = 0;
	
	background = load_image("background.bmp");
	apply_surface(0, 0, background, window);
	
	/* Initialize SDL and make sure it quits*/
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);

	/* Create window surface*/

	if (window == NULL) {
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		return 1;
	}

	/* Define the rects we need*/
	

	/* Load test spritesheet image*/
	
	if (background == NULL) {
		printf("ERROR: failed to load image: %s\n", SDL_GetError());
		return 1;
	}

	/* Set colorkey to BLUE*/
/* 	if (SDL_SetColorKey(img, SDL_SRCCOLORKEY, SDL_MapRGB(img->format, 0, 0, 255)) != 0) {
		printf("ERROR: failed to set color key: %s\n", SDL_GetError());
		SDL_FreeSurface(img);
		return 1;
	}
 */

	while (!quit) {
		/* Clear window to BLACK*/
/* 		if (SDL_FillRect(window,0,0) != 0) {
			printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
			break;
		}
 */
		/* Green rectangle button*/
/* 		if (SDL_FillRect(w, &rect, SDL_MapRGB(w->format, 0, 255, 0)) != 0) {
			printf("ERROR: failed to draw rect: %s\n", SDL_GetError());
			break;
		}
 */
		/* Draw image sprite*/
/* 		if (SDL_BlitSurface(background, NULL, window, 0) != 0) {
			SDL_FreeSurface(background);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			break;
		}
 */
		/* Advance to next sprite*/
/* 		imgrect.x += imgrect.w;
		if (imgrect.x >= img->w) {
			imgrect.x = 0;
			imgrect.y += imgrect.h;
			if (imgrect.y >= img->h) imgrect.y = 0;
		}
 */
		/* We finished drawing*/
		if (SDL_Flip(window) != 0) {
			printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
			break;
		}

		/* Poll for keyboard & mouse events*/

		while (SDL_PollEvent(&event) != 0) {
			switch (event.type) {
				case (SDL_QUIT):
					quit = 1;
					break;
				case (SDL_KEYUP):
					if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
					break;
/* 				case (SDL_MOUSEBUTTONUP):
					if ((event.button.x > rect.x) && (event.button.x < rect.x + rect.w) && (event.button.y > rect.y) && (event.button.y < rect.y+rect.h))
						quit = 1;
					break; */
				default:
					break;
			}
		}

		/* Wait a little before redrawing*/
		SDL_Delay(1000);
	}

	SDL_FreeSurface(background);
	return 0;
}
