#include <stdio.h>
#include <SDL.h>
#include <SDL_video.h>
#include "Chess.h"
#define WIN_W 800
#define WIN_H 600

#define IMG_W 240
#define IMG_H 296

#define WINDOW 0
#define MAIN_W 1
#define A 2
#define B 3
#define C 4
#define D 5


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
	int id;
	widget *child;
	widget *brother;
	button *kid;
	SDL_Surface *sr;
	SDL_Rect rct;
};

struct button_st 
{
	int board_button; //0-normal, 1-board_button 
	char name;
	int active;
	int selected;
	int highlight;
	int special;
	button *up;
	button *down;
	button *left;
	button *right;
	button *next;
	SDL_Surface *sr;
	SDL_Surface *sr2;
	SDL_Surface *sr3;

	SDL_Rect rct;
};
widget *window = NULL;
widget *main_w = NULL;

//---------------------------------------------------------------------------------// 
		/* ----Factories for Widgets---- */
widget *build_panel(int x, int y, int size_w, int size_h, char *filename, int id) 
{
	widget *panel = NULL;
	if ( (panel = (widget*)malloc(sizeof(widget))) == NULL ) quit_allocation_error(); //???
	panel->id = id;
	panel->sr = load_image(filename);
	panel->child = NULL;
	panel->brother = NULL;
	SDL_Rect recta = {x, y, size_w, size_h};
	panel->rct = recta;
	if ( panel->sr == NULL )
	{	
		printf("ERROR: failed to load image: %s\n", SDL_GetError());
		return NULL;
	}
	return panel;	
}

widget *build_window(void)
{	
	printf("%s\n", "0.1");
	fflush(stdout);
	if ( (window = (widget*)malloc(1*sizeof(widget))) == NULL ) quit_allocation_error(); //???
	printf("%s\n", "0.15");
	fflush(stdout);

	window->sr = SDL_SetVideoMode( WIN_W, WIN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF );
	printf("%s\n", "0.2");
	fflush(stdout);
	window->id = 0;
	window->child = NULL;
	window->brother = NULL;	
	return window;
}

button *build_button(int x, int y, int size_w, int size_h, char *filename, char *filename2, char *filename3, char name, int is_board)
{
	button *but = NULL;
	if ( (but = (button*)malloc(sizeof(button))) == NULL ) quit_allocation_error(); //???
	but->sr = load_image(filename);
	but->sr2 = load_image(filename2);
	but->sr3 = load_image(filename3);

	SDL_Rect recta = {x, y, size_w, size_h};
	but->rct = recta;
	if ( but->sr == NULL )
	{	
		printf("ERROR: failed to load image: %s\n", SDL_GetError());
		return NULL;
	}
	but->board_button = is_board; //0-normal, 1-board_button 
	but->name = name;
	but->active = 0;
	but->selected = 0;
	but->highlight = 0;
	but->special = 0;
	but->up = NULL;
	but->down = NULL;
	but->left = NULL;
	but->right = NULL;
	but->next = NULL;
	return but;	
}


//---------------------------------------------------------------------------------//
		/* ----Build Screens---- */
		
widget *init_main(void)
{
	int width_b = 300;
	int height_b = 80;
	int space = 130;

	widget *main_w = build_panel(0, 0, WIN_W, WIN_H, "pics/main_w_test.bmp", 1);

	button *quit_b = build_button(250,200 + (2*space), width_b, height_b, "pics/quit_b1.bmp", "pics/quit_b2.bmp", "pics/quit_b3.bmp",'q', 0);
	button *new_game_b = build_button(250,200 + (0*space), width_b, height_b, "pics/new_b1.bmp", "pics/new_b2.bmp", "pics/new_b3.bmp", 'n', 0);
	button *load_game_b = build_button(250,200 + (1*space), width_b, height_b, "pics/load_b1.bmp", "pics/load_b2.bmp", "pics/load_b3.bmp", 'l',0);

	quit_b->up = load_game_b;
	load_game_b->down = quit_b;
	load_game_b->up = new_game_b;
	new_game_b->down = load_game_b;
	
	new_game_b->active = 1;
	load_game_b->active = 1;
	quit_b->active = 1;
	
	if (quit_b->down == NULL) printf("walla\n");
	else printf("ashkara\n");
	main_w->kid = new_game_b;
	new_game_b->next = load_game_b;
	load_game_b->next = quit_b;
	apply_surface(new_game_b->rct.x, new_game_b->rct.y, new_game_b->sr, main_w->sr);
	apply_surface(load_game_b->rct.x, load_game_b->rct.y, load_game_b->sr, main_w->sr);
	apply_surface(quit_b->rct.x, quit_b->rct.y, quit_b->sr, main_w->sr);

	return main_w;
	
}

//---------------------------------------------------------------------------------//
		/* ----General methods---- */
		
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
	printf("%s\n", "0.0");
	fflush(stdout);

    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return 1;    
    }
	printf("%s\n", "0.01");
	fflush(stdout);

    window = build_window();
    //Set up the screen
    //If there was an error in setting up the screen
	printf("%s\n", "0.5");
	fflush(stdout);

    if( window->sr == NULL )
    {
		printf("ERROR: failed to init main window: %s\n", SDL_GetError());
		return 1;    
    }
    //Set the window caption
    SDL_WM_SetCaption( "Chess Prog", NULL );
    //If everything initialized fine
    return 0;
}

void refresh_button(widget *panel, button *butt)
{
	if ( butt->selected )
	{
		apply_surface(butt->rct.x, butt->rct.y, butt->sr3, panel->sr);
	}
	else if ( butt->highlight )
	{
		apply_surface(butt->rct.x, butt->rct.y, butt->sr2, panel->sr);
	}
	else 
	{
		apply_surface(butt->rct.x, butt->rct.y, butt->sr, panel->sr);
	}
}
int do_job(button *but)
{
	switch (but->name)
	{
		case ('q'):
			return 1;
			break;
		default:
			break;
	}
	return 0;
}
//---------------------------------------------------------------------------------//
	/* ----Main function----*/
	
int play_gui(void) {
	printf("%s\n", "0");
	fflush(stdout);
	if ( init() ) return 1;
	printf("%s\n", "1");
	fflush(stdout);

	SDL_Event event;
	main_w = init_main();

	printf("%s\n", "2");
	fflush(stdout);

	window->child = main_w; 
	int quit = 0;
	
	widget *current = window;
	apply_surface(current->child->rct.x, current->child->rct.y, current->child->sr, current->sr);
	
	printf("%s\n", "3");
	fflush(stdout);

	/* Initialize SDL and make sure it quits*/
/* 	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);
 */	
	button *tmp = NULL;
	button *curr = window->child->kid;
	curr->highlight = 1;
	refresh_button(window->child, curr);
	apply_surface(0, 0, window->child->sr, window->sr);

	while (!quit) {
		apply_surface(0, 0, window->child->sr, window->sr);
		printf("%c\n", curr->name);
		fflush(stdout);
		/* We finished drawing*/
		if (SDL_Flip(window->sr) != 0) {
			printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
			break;
		}
		if (curr->selected) quit = do_job(curr);
		/* Poll for keyboard & mouse events*/
		while (SDL_PollEvent(&event) != 0) {
		
			switch (event.type) {
				case (SDL_QUIT):
					quit = 1;
					break;
				case (SDL_KEYUP):
					if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
					else if ( (event.key.keysym.sym == SDLK_KP_ENTER) || (event.key.keysym.sym == SDLK_RETURN) )
					{
						curr->selected = 1;
						refresh_button(window->child, curr);
					}
					else if ( (event.key.keysym.sym == SDLK_UP) && (curr->up != NULL) )
					{
						curr->highlight = 0;
						curr->selected = 0;
						refresh_button(window->child, curr);
						curr = curr->up;
						curr->highlight = 1;
						refresh_button(window->child, curr);
					}	
					else if ( (event.key.keysym.sym == SDLK_DOWN) && (curr->down != NULL) )
					{
						curr->highlight = 0;
						curr->selected = 0;
						refresh_button(window->child, curr);
						curr = curr->down;
						curr->highlight = 1;
						refresh_button(window->child, curr);
					}	
					else if ( (event.key.keysym.sym == SDLK_LEFT) && (curr->left != NULL) )
					{
						curr->highlight = 0;
						curr->selected = 0;
						refresh_button(window->child, curr);
						curr = curr->left;
						curr->highlight = 1;
						refresh_button(window->child, curr);
					}	
					else if ( (event.key.keysym.sym == SDLK_RIGHT) && (curr->right != NULL) )
					{
						curr->highlight = 0;
						curr->selected = 0;
						refresh_button(window->child, curr);
						curr = curr->right;
						curr->highlight = 1;
						refresh_button(window->child, curr);
					}	
					break;
  				case (SDL_MOUSEBUTTONUP):
					tmp = window->child->kid;
					while (tmp != NULL)
					{
						if ((event.button.x > tmp->rct.x) && (event.button.x < tmp->rct.x + tmp->rct.w) && (event.button.y > tmp->rct.y) && (event.button.y < tmp->rct.y+tmp->rct.h))
						{
						curr->highlight = 0;
						curr->selected = 0;
						refresh_button(window->child, curr);
						curr = tmp;
						curr->selected = 1;
						refresh_button(window->child, curr);
						}
						tmp = tmp->next;
					}
					break;  
				default:
					break;
			}
			
		}

		/* Wait a little before redrawing*/
//		SDL_Delay(1000);
	}
	SDL_FreeSurface(main_w->sr);
	return 0;
}


