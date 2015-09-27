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

struct widget_st 
{
	widget *child;
	widget *brother;
	SDL_Surface *sr;
	SDL_Rect rct;
};
widget *window = NULL;
widget *background = NULL;

/* widget *build_button()
{
	
}
 */
widget *build_panel(int x, int y, int size_w, int size_h, char *filename) 
{
	widget *panel = NULL;
	if ( (panel = (widget*)malloc(sizeof(widget))) == NULL ) quit_allocation_error(); //???
	panel->sr = load_image(filename);
	panel->child = NULL;
	panel->brother = NULL;
	SDL_Rect recta = {x, y, size_w, size_h};
	panel->rct = recta;
	return panel;	
}

widget *build_window(void)
{
	widget *window = NULL;
	if ( (window = (widget*)malloc(sizeof(widget))) == NULL ) quit_allocation_error(); //???
	window->sr = SDL_SetVideoMode( WIN_W, WIN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF );
	window->child = NULL;
	window->brother = NULL;	
	return window;
}

void quit_allocation_error(void)
{
	perror("Failed to allocate memory.");
    exit(1);
}

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

void apply_surface( int x, int y, SDL_Surface *source, SDL_Surface *destination )
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
	printf("%s\n", "init everybody");
	fflush(stdout);
    window = build_window();
    //Set up the screen
	printf("%s\n", "build every body");
	fflush(stdout);
    //If there was an error in setting up the screen
    if( window == NULL )
    {
        return 1;    
    }
	printf("%s\n", "NULL????????");
	fflush(stdout);
    //Set the window caption
    SDL_WM_SetCaption( "Chess Prog", NULL );
  	printf("%s\n", "NULL????????");
	fflush(stdout);  
    //If everything initialized fine
    return 0;
}


int play_gui(void) {
	printf("%s\n", "hello everybody0");
	fflush(stdout);	
	if ( init() ) return 1;
	printf("%s\n", "init!!!!!!");
	fflush(stdout);
	SDL_Event event;
	background = build_panel(0, 0, WIN_W, WIN_H, "background.bmp");
	window->child = background; 
	// 	SDL_Rect quit_button = {225, 400, 50, 50};
//	SDL_Rect imgrect = {0, 0, IMG_W, IMG_H};
 //*/	
//	SDL_Surface *background = SDL_DisplayFormat(SDL_LoadBMP("background.bmp"));
	SDL_WM_SetCaption("Chess Prog", NULL);
	int quit = 0;
	
	widget *current = window;
	apply_surface(current->child->rct.x, current->child->rct.y, current->child->sr, current->sr);
	
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
		printf("%s\n", "hello everybody");
		fflush(stdout);
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
		if (SDL_Flip(window->sr) != 0) {
			printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
			break;
		}
		printf("%s\n", "hello everybody2");
		fflush(stdout);
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
	printf("%s\n", "hello everybody3");
	fflush(stdout);
	SDL_FreeSurface(background->sr);
	return 0;
}
