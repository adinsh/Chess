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
extern int GAME_STATUS;


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
	int row;
	int column;
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
	SDL_Surface *sr4;
	SDL_Surface *sr5;

	SDL_Rect rct;
};
widget *window = NULL; // id: 0
widget *main_w = NULL; // id: 1
widget *settings_w = NULL; // id: 2
widget *set_diff_w = NULL; // id: 3

//---------------------------------------------------------------------------------// 
		/* ----Factories for Widgets---- */
widget *build_panel(int x, int y, int size_w, int size_h, char *filename, int id) 
{
	widget *panel = NULL;
	if ( (panel = (widget*)malloc(sizeof(widget))) == NULL ) quit_allocation_error(); //???
	panel->id = id;
	panel->sr = load_image(filename, 0);
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

button *build_button(int x, int y, int size_w, int size_h, SDL_Surface *sr, SDL_Surface *sr2, SDL_Surface *sr3,
					 SDL_Surface *sr4, SDL_Surface *sr5, char name, int is_board)
{
	button *but = NULL;
	if ( (but = (button*)malloc(sizeof(button))) == NULL ) quit_allocation_error(); //???
	but->sr = sr;
	but->sr2 = sr2;
	but->sr3 = sr3;
	but->sr4 = sr4;
	but->sr5 = sr5;
	

	SDL_Rect recta = {x, y, size_w, size_h};
	but->rct = recta;
	if ( but->sr == NULL )
	{	
		printf("ERROR: failed to load image: %s\n", SDL_GetError());
		return NULL;
	}
	but->board_button = is_board; //0-normal, 1-board_button 
	but->name = name;
	but->active = 1;
	but->selected = 0;
	but->highlight = 0;
	but->special = 0;
	but->row = -1;
	but->column = -1;
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

	widget *main_w = build_panel(0, 0, WIN_W, WIN_H, "pics/main_w.bmp", 1);

	button *quit_b = build_button(250,200 + (2*space), width_b, height_b, load_image("pics/quit_b1.bmp", 0), load_image("pics/quit_b2.bmp", 0), load_image("pics/quit_b3.bmp", 0), NULL, NULL, 'q', 0);
	button *new_game_b = build_button(250,200 + (0*space), width_b, height_b, load_image("pics/new_b1.bmp", 0), load_image("pics/new_b2.bmp", 0), load_image("pics/new_b3.bmp", 0), NULL, NULL, 'n', 0);
	button *load_game_b = build_button(250,200 + (1*space), width_b, height_b, load_image("pics/load_b1.bmp", 0), load_image("pics/load_b2.bmp", 0), load_image("pics/load_b3.bmp", 0), NULL, NULL, 'l',0);
    
	quit_b->up = load_game_b;
	load_game_b->down = quit_b;
	load_game_b->up = new_game_b;
	new_game_b->down = load_game_b;
	
	main_w->kid = new_game_b;
	new_game_b->next = load_game_b;
	load_game_b->next = quit_b;
	apply_surface(new_game_b->rct.x, new_game_b->rct.y, new_game_b->sr, main_w->sr);
	apply_surface(load_game_b->rct.x, load_game_b->rct.y, load_game_b->sr, main_w->sr);
	apply_surface(quit_b->rct.x, quit_b->rct.y, quit_b->sr, main_w->sr);

	return main_w;
	
}

widget *init_settings(void)
{
	int width_b_1 = 120;
	int width_b_2 = 300;
	int width_b_3 = 180;
	int width_b_4 = 240;
	int height_b = 48;

	widget *settings_w = build_panel(0, 0, WIN_W, WIN_H, "pics/settings/settings_w.bmp", 2);

	button *set_white_b = build_button(42,305, width_b_1, height_b, load_image("pics/settings/set_white_b.bmp", 0),
																	load_image("pics/settings/set_white_b2.bmp", 0), 
																	load_image("pics/settings/set_white_b3.bmp", 0), 
																	load_image("pics/settings/set_white_b4.bmp", 0), 
																	load_image("pics/settings/set_white_b5.bmp", 0),'w', 0);
	button *set_black_b = build_button(42+width_b_1+50,305, width_b_1, height_b, load_image("pics/settings/set_black_b.bmp", 0), 
																				 load_image("pics/settings/set_black_b2.bmp", 0), 
																				 load_image("pics/settings/set_black_b3.bmp", 0), 
																				 load_image("pics/settings/set_black_b4.bmp", 0), 
																				 load_image("pics/settings/set_black_b5.bmp", 0), 'b', 0);
	
	button *set_playervsplayer_b = build_button(42,185, width_b_2, height_b, load_image("pics/settings/set_playervsplayer_b.bmp", 0),
																			 load_image("pics/settings/set_playervsplayer_b2.bmp", 0), 
																			 load_image("pics/settings/set_playervsplayer_b3.bmp", 0), 
																			 load_image("pics/settings/set_playervsplayer_b4.bmp", 0), 
																			 load_image("pics/settings/set_playervsplayer_b5.bmp", 0), 'p',0);
	button *set_playervscomputer_b = build_button(42+width_b_2+50,185, width_b_2, height_b, load_image("pics/settings/set_playervscomputer_b.bmp", 0), 
																							load_image("pics/settings/set_playervscomputer_b2.bmp", 0), 
																							load_image("pics/settings/set_playervscomputer_b3.bmp", 0), 
																							load_image("pics/settings/set_playervscomputer_b4.bmp", 0), 
																							load_image("pics/settings/set_playervscomputer_b5.bmp", 0), 'c', 0);
	button *set_changeboard_b = build_button(250,425, width_b_2, height_b, load_image("pics/settings/set_changeboard_b.bmp", 0), 
																		   load_image("pics/settings/set_changeboard_b2.bmp", 0), 
																		   load_image("pics/settings/set_changeboard_b3.bmp", 0), 
																		   NULL, 
																		   NULL, 's', 0);
	
	button *set_mainmenu_b = build_button(35,545, width_b_3, height_b, load_image("pics/settings/set_mainmenu_b.bmp", 0), 
																	   load_image("pics/settings/set_mainmenu_b2.bmp", 0), 
																	   load_image("pics/settings/set_mainmenu_b3.bmp", 0), 
																	   NULL, 
																	   NULL, 'm', 0);
	button *set_play_b = build_button(525,545, width_b_4, height_b, load_image("pics/settings/set_play_b.bmp", 0), 
																	load_image("pics/settings/set_play_b2.bmp", 0), 
																	load_image("pics/settings/set_play_b3.bmp", 0), 
																	NULL, 
																	NULL, 'g', 0);
	button *set_setdificulty_b = build_button(525,545, width_b_4, height_b, load_image("pics/settings/set_setdifficulty_b.bmp", 0), 
																			load_image("pics/settings/set_setdifficulty_b2.bmp", 0), 
																			load_image("pics/settings/set_setdifficulty_b3.bmp", 0), 
																			NULL, 
																			NULL, 'd', 0);

	//set buttons' next pointers
	settings_w->kid =  set_playervsplayer_b;
	set_playervsplayer_b->next = set_playervscomputer_b;
	set_playervscomputer_b->next = set_white_b;
	set_white_b->next = set_black_b;
	set_black_b->next = set_changeboard_b;
	set_changeboard_b->next = set_mainmenu_b;
	set_mainmenu_b->next = set_play_b;
	set_play_b->next = set_setdificulty_b;
	
	//set buttons' keyboard pointers
	set_playervsplayer_b->right = set_playervscomputer_b;
	set_playervsplayer_b->down = set_white_b;
	set_playervscomputer_b->left = set_playervsplayer_b;
	set_playervscomputer_b->down = set_black_b;
	set_white_b->up = set_playervsplayer_b;
	set_white_b->down = set_changeboard_b;
	set_white_b->right = set_black_b;
	set_black_b->up = set_playervscomputer_b;
	set_black_b->left = set_white_b;
	set_black_b->down = set_changeboard_b;
	set_changeboard_b->up = set_white_b;
	set_changeboard_b->down = set_mainmenu_b;
	set_mainmenu_b->up = set_changeboard_b;
	set_mainmenu_b->right = set_play_b;
	set_play_b->left = set_mainmenu_b;
	set_play_b->up = set_changeboard_b;
	set_setdificulty_b->left = set_mainmenu_b;
	set_setdificulty_b->up = set_changeboard_b;
	
	
	//set buttons' states
	set_playervsplayer_b->special = 1;
	set_playervsplayer_b->highlight = 1;
	set_playervsplayer_b->active = 0;
	set_white_b->special = 1;
	set_white_b->active = 0;
	set_setdificulty_b->active = 0;

	//put the buttons on the screen
	apply_surface(set_white_b->rct.x, set_white_b->rct.y, set_white_b->sr4, settings_w->sr);
	apply_surface(set_black_b->rct.x, set_black_b->rct.y, set_black_b->sr, settings_w->sr);
	apply_surface(set_playervsplayer_b->rct.x, set_playervsplayer_b->rct.y, set_playervsplayer_b->sr5, settings_w->sr);
	apply_surface(set_playervscomputer_b->rct.x, set_playervscomputer_b->rct.y, set_playervscomputer_b->sr, settings_w->sr);
	apply_surface(set_changeboard_b->rct.x, set_changeboard_b->rct.y, set_changeboard_b->sr, settings_w->sr);
	apply_surface(set_mainmenu_b->rct.x, set_mainmenu_b->rct.y, set_mainmenu_b->sr, settings_w->sr);
	apply_surface(set_play_b->rct.x, set_play_b->rct.y, set_play_b->sr, settings_w->sr);


	return settings_w;
}

widget *init_set_diff(void)
{
	widget *set_diff_w = build_panel(0, 0, WIN_W, WIN_H, "pics/set_diff/set_diff_w.bmp", 3);
	int width_b_1 = 120;
	int width_b_2 = 100;
	int width_b_3 = 180;
	int width_b_4 = 240;
	int height_b = 48;

	button *set_white_b = build_button(42,425, width_b_1, height_b, load_image("pics/set_diff/set_white_b.bmp", 0),
																	load_image("pics/set_diff/set_white_b2.bmp", 0), 
																	load_image("pics/set_diff/set_white_b3.bmp", 0), 
																	load_image("pics/set_diff/set_white_b4.bmp", 0), 
																	load_image("pics/set_diff/set_white_b5.bmp", 0),'w', 0);
																	
	button *set_black_b = build_button(42+width_b_1+50,425, width_b_1, height_b, load_image("pics/set_diff/set_black_b.bmp", 0), 
																				 load_image("pics/set_diff/set_black_b2.bmp", 0), 
																				 load_image("pics/set_diff/set_black_b3.bmp", 0), 
																				 load_image("pics/set_diff/set_black_b4.bmp", 0), 
																				 load_image("pics/set_diff/set_black_b5.bmp", 0), 'b', 0);
																				 
	button *set_mainmenu_b = build_button(35,545, width_b_3, height_b, load_image("pics/set_diff/set_mainmenu_b.bmp", 0), 
																	   load_image("pics/set_diff/set_mainmenu_b2.bmp", 0), 
																	   load_image("pics/set_diff/set_mainmenu_b3.bmp", 0), 
																	   NULL, 
																	   NULL, 'm', 0);
																	   
	button *set_play_b = build_button(525,545, width_b_4, height_b, load_image("pics/set_diff/set_play_b.bmp", 0), 
																	load_image("pics/set_diff/set_play_b2.bmp", 0), 
																	load_image("pics/set_diff/set_play_b3.bmp", 0), 
																	NULL, 
																	NULL, 'g', 0);
	
	button *set_diff_1_b = build_button(42,216, width_b_2, height_b, load_image("pics/set_diff/set_diff_1_b.bmp", 0), 
																	load_image("pics/set_diff/set_diff_1_b2.bmp", 0), 
																	load_image("pics/set_diff/set_diff_1_b3.bmp", 0), 
																	load_image("pics/set_diff/set_diff_1_b4.bmp", 0), 
																	load_image("pics/set_diff/set_diff_1_b5.bmp", 0), '1', 0);
	
	button *set_diff_2_b = build_button(42 + width_b_1, 216, width_b_2, height_b, load_image("pics/set_diff/set_diff_2_b.bmp", 0), 
																load_image("pics/set_diff/set_diff_2_b2.bmp", 0), 
																load_image("pics/set_diff/set_diff_2_b3.bmp", 0), 
																load_image("pics/set_diff/set_diff_2_b4.bmp", 0), 
																load_image("pics/set_diff/set_diff_2_b5.bmp", 0), '2', 0);
	
	button *set_diff_3_b = build_button(42 + width_b_1*2, 216, width_b_2, height_b, load_image("pics/set_diff/set_diff_3_b.bmp", 0), 
															load_image("pics/set_diff/set_diff_3_b2.bmp", 0), 
															load_image("pics/set_diff/set_diff_3_b3.bmp", 0), 
															load_image("pics/set_diff/set_diff_3_b4.bmp", 0), 
															load_image("pics/set_diff/set_diff_3_b5.bmp", 0), '3', 0);

	button *set_diff_4_b = build_button(42 + width_b_1*3, 216, width_b_2, height_b, load_image("pics/set_diff/set_diff_4_b.bmp", 0), 
															load_image("pics/set_diff/set_diff_4_b2.bmp", 0), 
															load_image("pics/set_diff/set_diff_4_b3.bmp", 0), 
															load_image("pics/set_diff/set_diff_4_b4.bmp", 0), 
															load_image("pics/set_diff/set_diff_4_b5.bmp", 0), '4', 0);

	button *set_diff_5_b = build_button(42 + width_b_1*4, 216, width_b_2, height_b, load_image("pics/set_diff/set_diff_5_b.bmp", 0), 
															load_image("pics/set_diff/set_diff_5_b2.bmp", 0), 
															load_image("pics/set_diff/set_diff_5_b3.bmp", 0), 
															load_image("pics/set_diff/set_diff_5_b4.bmp", 0), 
															load_image("pics/set_diff/set_diff_5_b5.bmp", 0), '5', 0);

	button *set_diff_back_b = build_button(310,545, width_b_3, height_b, load_image("pics/set_diff/back_b.bmp", 0), 
															load_image("pics/set_diff/back_b2.bmp", 0), 
															load_image("pics/set_diff/back_b3.bmp", 0), 
															NULL, 
															NULL, 'B', 0);

	//set buttons' next pointers
	set_diff_w->kid =  set_diff_1_b;
	set_diff_1_b->next = set_diff_2_b;
	set_diff_2_b->next = set_diff_3_b;
	set_diff_3_b->next = set_diff_4_b;
	set_diff_4_b->next = set_diff_5_b;
	set_diff_5_b->next = set_white_b;
	set_white_b->next = set_black_b;
	set_black_b->next = set_mainmenu_b;
	set_mainmenu_b->next = set_diff_back_b;									
	set_diff_back_b->next = set_play_b;									
	
	//set buttons' keyboard pointers
	set_white_b->right = set_black_b;
	set_white_b->down =  set_mainmenu_b;
	set_white_b->up = set_diff_1_b;
	
	set_black_b->left = set_white_b;
	set_black_b->down = set_diff_back_b;
	set_black_b->up = set_diff_3_b;
	
	set_diff_1_b->right = set_diff_2_b;
	set_diff_1_b->down = set_white_b;
	
	set_diff_2_b->right = set_diff_3_b;
	set_diff_2_b->left = set_diff_1_b;
	set_diff_2_b->down = set_white_b;
	
	set_diff_3_b->right = set_diff_4_b;
	set_diff_3_b->left = set_diff_2_b;
	set_diff_3_b->down = set_black_b;

	set_diff_4_b->right = set_diff_5_b;
	set_diff_4_b->left = set_diff_3_b;
	set_diff_4_b->down = set_black_b;

	set_diff_5_b->left = set_diff_4_b;
	set_diff_5_b->down = set_black_b;
	
	set_mainmenu_b->up = set_white_b;
	set_mainmenu_b->right = set_diff_back_b;
	
	set_diff_back_b->up = set_white_b;
	set_diff_back_b->right = set_play_b;
	set_diff_back_b->left = set_mainmenu_b;
	
	set_play_b->up = set_black_b;
	set_play_b->left = set_diff_back_b;

	//set buttons' states
	set_diff_1_b->special = 1;
	set_diff_1_b->active = 0;
	set_white_b->special = 1;
	set_white_b->active = 0;
	set_white_b->highlight = 1;
	
	//put the buttons on the screen
	apply_surface(set_white_b->rct.x, set_white_b->rct.y, set_white_b->sr4, set_diff_w->sr);
	apply_surface(set_black_b->rct.x, set_black_b->rct.y, set_black_b->sr, set_diff_w->sr);
	apply_surface(set_diff_1_b->rct.x, set_diff_1_b->rct.y, set_diff_1_b->sr5, set_diff_w->sr);
	apply_surface(set_diff_2_b->rct.x, set_diff_2_b->rct.y, set_diff_2_b->sr, set_diff_w->sr);
	apply_surface(set_diff_3_b->rct.x, set_diff_3_b->rct.y, set_diff_3_b->sr, set_diff_w->sr);
	apply_surface(set_diff_4_b->rct.x, set_diff_4_b->rct.y, set_diff_4_b->sr, set_diff_w->sr);
	apply_surface(set_diff_5_b->rct.x, set_diff_5_b->rct.y, set_diff_5_b->sr, set_diff_w->sr);
	apply_surface(set_mainmenu_b->rct.x, set_mainmenu_b->rct.y, set_mainmenu_b->sr, set_diff_w->sr);
	apply_surface(set_play_b->rct.x, set_play_b->rct.y, set_play_b->sr, set_diff_w->sr);
	apply_surface(set_diff_back_b->rct.x, set_diff_back_b->rct.y, set_diff_back_b->sr, set_diff_w->sr);

	return set_diff_w;																	
}
//---------------------------------------------------------------------------------//
		/* ----General methods---- */
		
void quit_allocation_error(void)
{
	perror("Failed to allocate memory.");
    exit(1);
}

SDL_Surface *load_image( char *filename, int transper ) 
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
		
        if ( transper && (optimizedImage != NULL) )
		{
			//Map the color key
			Uint32 colorkey = SDL_MapRGB(optimizedImage->format, 0, 0, 0);
			SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, colorkey);

		}
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
	if ( butt->special)
	{
		if ( butt->highlight ) apply_surface(butt->rct.x, butt->rct.y, butt->sr5, panel->sr);
		else apply_surface(butt->rct.x, butt->rct.y, butt->sr4, panel->sr);
	}
	else if ( butt->selected )
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

void reset_panel(widget * panel)
{
	int w_id = panel->id;
	button *but = panel->kid;
	switch(w_id)
	{
		case (1): //rest main menu
			while(but != NULL)
			{
				but->active = 1;
				but->selected = 0;
				but->highlight = 0;
				but->special = 0;
				refresh_button(panel, but);
				but = but->next;
			}
			break;
		case (2): // reset settings
			while(but != NULL)
				{
					but->active = (but->name == 'w' || but->name == 'd' || but->name == 'p') ? 0 : 1;
					but->selected = 0;
					but->highlight = 0;
					but->special  = (but->name == 'w' ||  but->name == 'p') ? 1: 0;
					if ( but->next != NULL ) refresh_button(panel, but);
					if ( but->name == 'm' )
					{
						but->right->active = 0;
						but->right = but->next;
						but->right->active = 1;
						refresh_button(panel, but->right);
					}
					but = but->next;
				}
			break;
		case (3): // reset set_diff
			reset_panel(settings_w);
			while(but != NULL)
				{
					but->active = (but->name == 'w' || but->name == '1') ? 0 : 1;
					but->selected = 0;
					but->highlight = 0;
					but->special  = (but->name == 'w' ||  but->name == '1') ? 1: 0;
					refresh_button(panel, but);
					but = but->next;
				}
			break;
		default:
			break;
	
	}
	panel->kid->highlight = 1;
	refresh_button(panel, panel->kid);
}


void back_to_default(void) // we need to check if update is needed???
{
	reset_panel(main_w);
	reset_panel(window->child);
	window->child = main_w;
	apply_surface(window->child->rct.x, window->child->rct.y, window->child->sr, window->sr);
	init_board(board);
	MINIMAX_DEPTH = 1;
	GAME = 0;
	SETTINGS = 1;
	GAME_STATUS = 0; 
	GUI_MODE = 0; 
	TWO_PLAYERS_MODE = 1; 
	WHITE_TURN = 1; 
	PLAYER_WHITE = 1;
	WHITE_CR_ENABLE = 1; 
	BLACK_CR_ENABLE = 1;
	WHITE_CL_ENABLE = 1;
	BLACK_CL_ENABLE = 1;
	CHECK_ON_WHITE = 0;
	CHECK_ON_BLACK = 0;
}


int activate_buttons_function(button *but)
{
	int win_index = window->child->id;
	but->selected = 0;
	refresh_button(window->child, but);
	switch (win_index)
	{
		case (1):
			return activate_button_main_w(but);
			break;
		case (2):
			activate_button_settings_w(but);
			break;
		case (3):
			activate_button_set_diff_w(but);
			break;
	}
	return 0;
}

int activate_button_main_w(button *but)
{
	switch (but->name)
	{
		case ('q'): 
			return 1; //quit button
			break;
		case ('n'): 
			window->child = settings_w; //new game button
			break;
		case ('l'): //load game button
			//???
			break;
	}
	return 0;
}

void activate_button_settings_w(button *but)
{
	switch (but->name)
	{
		case ('m'): 
			back_to_default(); //main menu button
			break;
		case ('w'): // white player start
			WHITE_TURN = 1;
			but->special = 1;
			but->active = 0;
			refresh_button(window->child, but);
			but->right->special = 0;
			but->right->active =1;
			refresh_button(window->child, but->right);
			break;
		case ('b'): // black player start 
			WHITE_TURN  = 0;
			but->special = 1;
			but->active = 0;
			refresh_button(window->child, but);
			but->left->special = 0;
			but->left->active =1;
			refresh_button(window->child, but->left);
			break;
		case ('p'): // player vs player
			TWO_PLAYERS_MODE = 1;
			but->special = 1;
			but->active = 0;
			refresh_button(window->child, but);
			but->right->special = 0;
			but->right->active =1;
			refresh_button(window->child, but->right);
			but->down->down->down->right->active = 0;
			but->down->down->down->right = but->down->down->down->next;
			refresh_button(window->child, but->down->down->down->right);
			but->down->down->down->right->active = 1;
			break;
		case ('c'): // player vs computer
			TWO_PLAYERS_MODE = 0;
			but->special = 1;
			but->active = 0;
			refresh_button(window->child, but);
			but->left->special = 0;
			but->left->active =1;
			refresh_button(window->child, but->left);
			but->down->down->down->right->active = 0;
			but->down->down->down->right = but->down->down->down->right->next;
			refresh_button(window->child, but->down->down->down->right);
			but->down->down->down->right->active = 1;
			break;
		case ('d'): //change to set difficulty window
			but->highlight = 0;
			refresh_button(window->child, but);
			window->child->kid->highlight = 1;
			refresh_button(window->child, window->child->kid);
			window->child = set_diff_w;
			break;
		case ('g'): // ???
			printf("%s\n","!!!!!!!!!!play!!!!!!!!!!!!!!!!!!");
			break;
		case ('s'): // ???
			print_board(board);
			break;
			
	}
}

void activate_button_set_diff_w(button *but)
{
	button *tmp = window->child->kid;
	switch (but->name)
	{
		case ('m'): 
			back_to_default(); //main menu button
			break;
		case ('w'): // player is white
			PLAYER_WHITE = 1;
			but->special = 1;
			but->active = 0;
			refresh_button(window->child, but);
			but->right->special = 0;
			but->right->active =1;
			refresh_button(window->child, but->right);
			break;
		case ('b'): // player is black
			PLAYER_WHITE = 0;
			but->special = 1;
			but->active = 0;
			refresh_button(window->child, but);
			but->left->special = 0;
			but->left->active =1;
			refresh_button(window->child, but->left);
			break;
		case ('1'):
			MINIMAX_DEPTH = 1;
			break;
		case ('2'):
			MINIMAX_DEPTH = 2;
			break;
		case ('3'):
			MINIMAX_DEPTH = 3;
			break;
		case ('4'):
			MINIMAX_DEPTH = 4;
			break;
		case ('5'):
			MINIMAX_DEPTH = -1;
			break;
		case ('B'): //back to settings
			but->highlight = 0;
			refresh_button(window->child, but);
			window->child->kid->highlight = 1;
			refresh_button(window->child, window->child->kid);
			window->child = settings_w;
			break;
		case ('g'): // ???
			printf("%s\n","!!!!!!!!!!play!!!!!!!!!!!!!!!!!!");
			break;
	}
	if ( (but->name >= '1') && (but->name <= '5') )
	{
		while(tmp != NULL)
		{
			tmp->special = 0;
			tmp->active =1;
			refresh_button(window->child, tmp);
			tmp = tmp->right;
		}
		but->special = 1;
		but->active = 0;
		refresh_button(window->child, but);
	}

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
	settings_w = init_settings();
	set_diff_w = init_set_diff();
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
	int w_id = window->child->id;
	while (!quit) {
		if ( window->child->id != w_id )
		{
			curr = window->child->kid;
			w_id = window->child->id;
		}
		apply_surface(0, 0, window->child->sr, window->sr);
		printf("%c\n", curr->name);
		fflush(stdout);
		/* We finished drawing*/
		if (SDL_Flip(window->sr) != 0) {
			printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
			break;
		}
		if (curr->selected) quit = activate_buttons_function(curr);
		
		/* Poll for keyboard & mouse events*/
		while (SDL_PollEvent(&event) != 0) {
		
			switch (event.type) {
				case (SDL_QUIT):
					quit = 1;
					break;
				case (SDL_KEYUP):
					if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
					else if ( (curr->active == 1) && ((event.key.keysym.sym == SDLK_KP_ENTER) || (event.key.keysym.sym == SDLK_RETURN)) ) 
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
						if ( (tmp->active == 1) && (event.button.x > tmp->rct.x) && (event.button.x < tmp->rct.x + tmp->rct.w) && (event.button.y > tmp->rct.y) && (event.button.y < tmp->rct.y+tmp->rct.h))
						{
							curr->highlight = 0;
							curr->selected = 0;
							refresh_button(window->child, curr);
							curr = tmp;
							curr->selected = 1;
							curr->highlight = 1;
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


