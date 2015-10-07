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
#define SETTINGS_W 2
#define SET_DIFFICULTY_W 3
#define CHANGE_BOARD_W 4
#define LOAD_GAME_W 5
#define PLAY_GAME_W 6
#define SAVE_GAME_W 7
#define PROMOTION_W 8
#define CHOOSE_DEPTH_W 9
#define DECLARE_WINNER_W 10


#define MEMORY_SLOTS 7 //how many possible load/save slots (a value between 1 to 12) 

// Genral settings
extern int MINIMAX_DEPTH;
extern int GAME;
extern int SETTINGS;
extern int GUI_MODE; // '0' - application runs in 'Console mode', '1' - 'Gui mode'
extern int TWO_PLAYERS_MODE; // '1' - two players mode, '0' - player vs. AI mode
extern int WHITE_TURN; // 0 - black, 1- white
extern int PLAYER_WHITE; 
extern char board[BOARD_SIZE][BOARD_SIZE];
extern int GAME_STATUS;

// Game setiings

extern int CHECK_ON_WHITE;
extern int CHECK_ON_BLACK;

int is_remove_cmd = 0;
int is_set_cmd = 0;
int is_best_move = 0;

int gui_board_from[2] = {-1,-1};
int gui_board_to[2] = {-1,-1};
char selected_piece = EMPTY; 
char gui_promote_to = EMPTY;

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

SDL_Surface *icon_pic = NULL;

widget *window = NULL; // id: 0
widget *welcome_to_chess_w = NULL; // id: -1
widget *main_w = NULL; // id: 1
widget *settings_w = NULL; // id: 2
widget *set_diff_w = NULL; // id: 3
widget *change_board_w = NULL; // id: 4
widget *load_game_w = NULL; // id: 5
widget *play_game_w = NULL; // id: 6
widget *save_game_w = NULL; // id: 7
widget *promotion_w = NULL; // id: 8
widget *choose_depth_w = NULL; // id: 9
widget *declare_winner_w = NULL; // id 10

//all the pieces
SDL_Surface *piece_empty = NULL;
SDL_Surface *piece_white_p = NULL;
SDL_Surface *piece_white_b = NULL;
SDL_Surface *piece_white_n = NULL;
SDL_Surface *piece_white_r = NULL;
SDL_Surface *piece_white_q = NULL;
SDL_Surface *piece_white_k = NULL;
SDL_Surface *piece_black_p = NULL;
SDL_Surface *piece_black_b = NULL;
SDL_Surface *piece_black_n = NULL;
SDL_Surface *piece_black_r = NULL;
SDL_Surface *piece_black_q = NULL;
SDL_Surface *piece_black_k = NULL;

//


button *gui_board_set[BOARD_SIZE][BOARD_SIZE];
button *gui_board_game[BOARD_SIZE][BOARD_SIZE];
//---------------------------------------------------------------------------------// 
		/* ----Factories for Widgets---- */
widget *build_panel(int x, int y, int size_w, int size_h, char *filename, int id) 
{
	widget *panel = NULL;
	if ( (panel = (widget*)malloc(sizeof(widget))) == NULL ) quit_allocation_error(); 
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
	if ( (window = (widget*)malloc(1*sizeof(widget))) == NULL ) quit_allocation_error(); 
	window->sr = SDL_SetVideoMode( WIN_W, WIN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF );
	window->id = 0;
	window->child = NULL;
	window->brother = NULL;	
	return window;
}

button *build_button(int x, int y, int size_w, int size_h, SDL_Surface *sr, SDL_Surface *sr2, SDL_Surface *sr3,
					 SDL_Surface *sr4, SDL_Surface *sr5, char name, int is_board)
{
	button *but = NULL;
	if ( (but = (button*)malloc(sizeof(button))) == NULL ) quit_allocation_error(); 
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
widget *init_welcome(void)
{
	widget *welcome_w = build_panel(0, 0, WIN_W, WIN_H, "pics/welcome_w.bmp", -1);
	return welcome_w;
}
widget *init_main(void)
{
	int width_b = 203;
	int height_b = 54;
	int space = 100;

	widget *main_w = build_panel(0, 0, WIN_W, WIN_H, "pics/main/main_w.bmp", 1);

	button *quit_b = build_button(298,250 + (2*space), width_b, height_b, load_image("pics/main/quit_b1.bmp", 0), load_image("pics/main/quit_b2.bmp", 0), load_image("pics/main/quit_b3.bmp", 0), NULL, NULL, 'q', 0);
	button *new_game_b = build_button(298,250 + (0*space), width_b, height_b, load_image("pics/main/new_b1.bmp", 0), load_image("pics/main/new_b2.bmp", 0), load_image("pics/main/new_b3.bmp", 0), NULL, NULL, 'n', 0);
	button *load_game_b = build_button(298,250 + (1*space), width_b, height_b, load_image("pics/main/load_b1.bmp", 0), load_image("pics/main/load_b2.bmp", 0), load_image("pics/main/load_b3.bmp", 0), NULL, NULL, 'l',0);
    
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
	int width_b = 203;
	int height_b = 54;
	int start_x_main_b = 35;

	widget *settings_w = build_panel(0, 0, WIN_W, WIN_H, "pics/settings/settings_w.bmp", 2);

	button *set_white_b = build_button(136,394, width_b, height_b, load_image("pics/settings/set_white_b.bmp", 0),
																	load_image("pics/settings/set_white_b2.bmp", 0), 
																	load_image("pics/settings/set_white_b3.bmp", 0), 
																	load_image("pics/settings/set_white_b4.bmp", 0), 
																	load_image("pics/settings/set_white_b5.bmp", 0),'w', 0);
	button *set_black_b = build_button(461,394, width_b, height_b, load_image("pics/settings/set_black_b.bmp", 0), 
																				 load_image("pics/settings/set_black_b2.bmp", 0), 
																				 load_image("pics/settings/set_black_b3.bmp", 0), 
																				 load_image("pics/settings/set_black_b4.bmp", 0), 
																				 load_image("pics/settings/set_black_b5.bmp", 0), 'b', 0);
	
	button *set_playervsplayer_b = build_button(136,219, width_b, height_b, load_image("pics/settings/set_playervsplayer_b.bmp", 0),
																			 load_image("pics/settings/set_playervsplayer_b2.bmp", 0), 
																			 load_image("pics/settings/set_playervsplayer_b3.bmp", 0), 
																			 load_image("pics/settings/set_playervsplayer_b4.bmp", 0), 
																			 load_image("pics/settings/set_playervsplayer_b5.bmp", 0), 'p',0);
	button *set_playervscomputer_b = build_button(461,219, width_b, height_b, load_image("pics/settings/set_playervscomputer_b.bmp", 0), 
																							load_image("pics/settings/set_playervscomputer_b2.bmp", 0), 
																							load_image("pics/settings/set_playervscomputer_b3.bmp", 0), 
																							load_image("pics/settings/set_playervscomputer_b4.bmp", 0), 
																							load_image("pics/settings/set_playervscomputer_b5.bmp", 0), 'c', 0);
	button *set_changeboard_b = build_button(WIN_W/2 - width_b/2,490, width_b, height_b, load_image("pics/settings/set_changeboard_b.bmp", 0), 
																		   load_image("pics/settings/set_changeboard_b2.bmp", 0), 
																		   load_image("pics/settings/set_changeboard_b3.bmp", 0), 
																		   NULL, 
																		   NULL, 's', 0);
	
	button *set_mainmenu_b = build_button(start_x_main_b,540, width_b, height_b, load_image("pics/settings/set_mainmenu_b.bmp", 0), 
																	   load_image("pics/settings/set_mainmenu_b2.bmp", 0), 
																	   load_image("pics/settings/set_mainmenu_b3.bmp", 0), 
																	   NULL, 
																	   NULL, 'm', 0);
	button *set_play_b = build_button(WIN_W-width_b-start_x_main_b,540, width_b, height_b, load_image("pics/settings/set_play_b.bmp", 0), 
																	load_image("pics/settings/set_play_b2.bmp", 0), 
																	load_image("pics/settings/set_play_b3.bmp", 0), 
																	NULL, 
																	NULL, 'g', 0);
																	
	button *set_setdificulty_b = build_button(WIN_W-width_b-start_x_main_b,540, width_b, height_b, load_image("pics/settings/set_setdifficulty_b.bmp", 0), 
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
	int width_b_2 = 76;
	int width_b_3 = 203;
	int width_b_4 = 90;
	int height_b = 54;

	button *set_white_b = build_button(250,396, width_b_4, height_b, load_image("pics/set_diff/set_white_b.bmp", 0),
																	load_image("pics/set_diff/set_white_b2.bmp", 0), 
																	load_image("pics/set_diff/set_white_b3.bmp", 0), 
																	load_image("pics/set_diff/set_white_b4.bmp", 0), 
																	load_image("pics/set_diff/set_white_b5.bmp", 0),'w', 0);
																	
	button *set_black_b = build_button(470,396, width_b_4, height_b, load_image("pics/set_diff/set_black_b.bmp", 0), 
																				 load_image("pics/set_diff/set_black_b2.bmp", 0), 
																				 load_image("pics/set_diff/set_black_b3.bmp", 0), 
																				 load_image("pics/set_diff/set_black_b4.bmp", 0), 
																				 load_image("pics/set_diff/set_black_b5.bmp", 0), 'b', 0);
																				 
	button *set_mainmenu_b = build_button(35,540, width_b_3, height_b, load_image("pics/set_diff/set_mainmenu_b.bmp", 0), 
																	   load_image("pics/set_diff/set_mainmenu_b2.bmp", 0), 
																	   load_image("pics/set_diff/set_mainmenu_b3.bmp", 0), 
																	   NULL, 
																	   NULL, 'm', 0);
																	   
	button *set_play_b = build_button(562,540, width_b_3, height_b, load_image("pics/set_diff/set_play_b.bmp", 0), 
																	load_image("pics/set_diff/set_play_b2.bmp", 0), 
																	load_image("pics/set_diff/set_play_b3.bmp", 0), 
																	NULL, 
																	NULL, 'g', 0);
	
	button *set_diff_1_b = build_button(122,216, width_b_2, height_b, load_image("pics/set_diff/set_diff_1_b.bmp", 0), 
																	load_image("pics/set_diff/set_diff_1_b2.bmp", 0), 
																	load_image("pics/set_diff/set_diff_1_b3.bmp", 0), 
																	load_image("pics/set_diff/set_diff_1_b4.bmp", 0), 
																	load_image("pics/set_diff/set_diff_1_b5.bmp", 0), '1', 0);
	
	button *set_diff_2_b = build_button(122 + width_b_1, 216, width_b_2, height_b, load_image("pics/set_diff/set_diff_2_b.bmp", 0), 
																load_image("pics/set_diff/set_diff_2_b2.bmp", 0), 
																load_image("pics/set_diff/set_diff_2_b3.bmp", 0), 
																load_image("pics/set_diff/set_diff_2_b4.bmp", 0), 
																load_image("pics/set_diff/set_diff_2_b5.bmp", 0), '2', 0);
	
	button *set_diff_3_b = build_button(122 + width_b_1*2, 216, width_b_2, height_b, load_image("pics/set_diff/set_diff_3_b.bmp", 0), 
															load_image("pics/set_diff/set_diff_3_b2.bmp", 0), 
															load_image("pics/set_diff/set_diff_3_b3.bmp", 0), 
															load_image("pics/set_diff/set_diff_3_b4.bmp", 0), 
															load_image("pics/set_diff/set_diff_3_b5.bmp", 0), '3', 0);

	button *set_diff_4_b = build_button(122 + width_b_1*3, 216, width_b_2, height_b, load_image("pics/set_diff/set_diff_4_b.bmp", 0), 
															load_image("pics/set_diff/set_diff_4_b2.bmp", 0), 
															load_image("pics/set_diff/set_diff_4_b3.bmp", 0), 
															load_image("pics/set_diff/set_diff_4_b4.bmp", 0), 
															load_image("pics/set_diff/set_diff_4_b5.bmp", 0), '4', 0);

	button *set_diff_5_b = build_button(122 + width_b_1*4, 216, width_b_2, height_b, load_image("pics/set_diff/set_diff_5_b.bmp", 0), 
															load_image("pics/set_diff/set_diff_5_b2.bmp", 0), 
															load_image("pics/set_diff/set_diff_5_b3.bmp", 0), 
															load_image("pics/set_diff/set_diff_5_b4.bmp", 0), 
															load_image("pics/set_diff/set_diff_5_b5.bmp", 0), '5', 0);

	button *set_diff_back_b = build_button(253,540, width_b_3, height_b, load_image("pics/set_diff/back_b.bmp", 0), 
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

widget *init_change_board(void)
{
	int width_b_2 = 104;
	int width_b_3 = 203;
	int height_b = 54;
	int sq_size = 50;
	widget *change_board_w = build_panel(0, 0, WIN_W, WIN_H, "pics/change_board/change_board_w.bmp", 4);
	
	button *mainmenu_b = build_button(35,540, width_b_3, height_b, load_image("pics/change_board/mainmenu_b.bmp", 0), 
																	   load_image("pics/change_board/mainmenu_b2.bmp", 0), 
																	   load_image("pics/change_board/mainmenu_b3.bmp", 0), 
																	   NULL, 
																	   NULL, 'm', 0);
	button *done_b = build_button(665,540, width_b_2, height_b, load_image("pics/change_board/done_b.bmp", 0), 
																	   load_image("pics/change_board/done_b2.bmp", 0), 
																	   load_image("pics/change_board/done_b3.bmp", 0), 
																	   NULL, 
																	   NULL, 'd', 0);
	button *set_b = build_button(300,540, width_b_2, height_b, load_image("pics/change_board/set_b.bmp", 0), 
																load_image("pics/change_board/set_b2.bmp", 0), 
																load_image("pics/change_board/set_b3.bmp", 0), 
																load_image("pics/change_board/set_b4.bmp", 0), 
																load_image("pics/change_board/set_b5.bmp", 0), 's', 0);
	button *remove_b = build_button(420,540, width_b_2, height_b, load_image("pics/change_board/remove_b.bmp", 0), 
																load_image("pics/change_board/remove_b2.bmp", 0), 
																load_image("pics/change_board/remove_b3.bmp", 0), 
																load_image("pics/change_board/remove_b4.bmp", 0), 
																load_image("pics/change_board/remove_b5.bmp", 0), 'r', 0);
	button *clear_b = build_button(540,540, width_b_2, height_b, load_image("pics/change_board/clear_b.bmp", 0), 
																load_image("pics/change_board/clear_b2.bmp", 0), 
																load_image("pics/change_board/clear_b3.bmp", 0), 
																NULL, NULL, 'z', 0);

	button *message_b = build_button(186,495, 427, 35, load_image("pics/change_board/msg_chs_st_rmv.bmp", 0), 
																load_image("pics/change_board/msg_chs_plc_on_brd.bmp", 0), 
																load_image("pics/change_board/msg_slct_pc_on_brd.bmp", 0), 
																load_image("pics/change_board/msg_slct_typ_pc_add.bmp", 0), 
																load_image("pics/change_board/msg_wrng_brd_init.bmp", 0), 'M', 0);
	
	button *select_piece_black_p = build_button(700,75, sq_size, sq_size, load_image("pics/change_board/piece_black_p.bmp", 0), 
																load_image("pics/change_board/piece_black_p2.bmp", 0), 
																load_image("pics/change_board/piece_black_p2.bmp", 0), 
																load_image("pics/change_board/piece_black_p3.bmp", 0), 
																load_image("pics/change_board/piece_black_p4.bmp", 0), '1', 0);

	button *select_piece_black_b = build_button(700,75 + 60, sq_size, sq_size, load_image("pics/change_board/piece_black_b.bmp", 0), 
																load_image("pics/change_board/piece_black_b2.bmp", 0), 
																load_image("pics/change_board/piece_black_b2.bmp", 0), 
																load_image("pics/change_board/piece_black_b3.bmp", 0), 
																load_image("pics/change_board/piece_black_b4.bmp", 0), '2', 0);
	button *select_piece_black_n = build_button(700,75 + 60*2, sq_size, sq_size, load_image("pics/change_board/piece_black_n.bmp", 0), 
																load_image("pics/change_board/piece_black_n2.bmp", 0), 
																load_image("pics/change_board/piece_black_n2.bmp", 0), 
																load_image("pics/change_board/piece_black_n3.bmp", 0), 
																load_image("pics/change_board/piece_black_n4.bmp", 0), '3', 0);
	button *select_piece_black_r = build_button(700,75 + 60*3, sq_size, sq_size, load_image("pics/change_board/piece_black_r.bmp", 0), 
																load_image("pics/change_board/piece_black_r2.bmp", 0), 
																load_image("pics/change_board/piece_black_r2.bmp", 0), 
																load_image("pics/change_board/piece_black_r3.bmp", 0), 
																load_image("pics/change_board/piece_black_r4.bmp", 0), '4', 0);
	button *select_piece_black_q = build_button(700,75 + 60*4, sq_size, sq_size, load_image("pics/change_board/piece_black_q.bmp", 0), 
																load_image("pics/change_board/piece_black_q2.bmp", 0), 
																load_image("pics/change_board/piece_black_q2.bmp", 0), 
																load_image("pics/change_board/piece_black_q3.bmp", 0), 
																load_image("pics/change_board/piece_black_q4.bmp", 0), '5', 0);
	button *select_piece_black_k = build_button(700,75 + 60*5, sq_size, sq_size, load_image("pics/change_board/piece_black_k.bmp", 0), 
																load_image("pics/change_board/piece_black_k2.bmp", 0), 
																load_image("pics/change_board/piece_black_k2.bmp", 0), 
																load_image("pics/change_board/piece_black_k3.bmp", 0), 
																load_image("pics/change_board/piece_black_k4.bmp", 0), '6', 0);


	button *select_piece_white_p = build_button(45,75, sq_size, sq_size, load_image("pics/change_board/piece_white_p.bmp", 0), 
																load_image("pics/change_board/piece_white_p2.bmp", 0), 
																load_image("pics/change_board/piece_white_p2.bmp", 0), 
																load_image("pics/change_board/piece_white_p3.bmp", 0), 
																load_image("pics/change_board/piece_white_p4.bmp", 0), 'A', 0);

	button *select_piece_white_b = build_button(45,75 + 60, sq_size, sq_size, load_image("pics/change_board/piece_white_b.bmp", 0), 
																load_image("pics/change_board/piece_white_b2.bmp", 0), 
																load_image("pics/change_board/piece_white_b2.bmp", 0), 
																load_image("pics/change_board/piece_white_b3.bmp", 0), 
																load_image("pics/change_board/piece_white_b4.bmp", 0), 'B', 0);
	button *select_piece_white_n = build_button(45,75 + 60*2, sq_size, sq_size, load_image("pics/change_board/piece_white_n.bmp", 0), 
																load_image("pics/change_board/piece_white_n2.bmp", 0), 
																load_image("pics/change_board/piece_white_n2.bmp", 0), 
																load_image("pics/change_board/piece_white_n3.bmp", 0), 
																load_image("pics/change_board/piece_white_n4.bmp", 0), 'C', 0);
	button *select_piece_white_r = build_button(45,75 + 60*3, sq_size, sq_size, load_image("pics/change_board/piece_white_r.bmp", 0), 
																load_image("pics/change_board/piece_white_r2.bmp", 0), 
																load_image("pics/change_board/piece_white_r2.bmp", 0), 
																load_image("pics/change_board/piece_white_r3.bmp", 0), 
																load_image("pics/change_board/piece_white_r4.bmp", 0), 'D', 0);
	button *select_piece_white_q = build_button(45,75 + 60*4, sq_size, sq_size, load_image("pics/change_board/piece_white_q.bmp", 0), 
																load_image("pics/change_board/piece_white_q2.bmp", 0), 
																load_image("pics/change_board/piece_white_q2.bmp", 0), 
																load_image("pics/change_board/piece_white_q3.bmp", 0), 
																load_image("pics/change_board/piece_white_q4.bmp", 0), 'E', 0);
	button *select_piece_white_k = build_button(45,75 + 60*5, sq_size, sq_size, load_image("pics/change_board/piece_white_k.bmp", 0), 
																load_image("pics/change_board/piece_white_k2.bmp", 0), 
																load_image("pics/change_board/piece_white_k2.bmp", 0), 
																load_image("pics/change_board/piece_white_k3.bmp", 0), 
																load_image("pics/change_board/piece_white_k4.bmp", 0), 'F', 0);
					
	//set buttons' next pointers
	change_board_w->kid = gui_board_set[0][BOARD_SIZE-1];
	gui_board_set[BOARD_SIZE-1][0]->next = select_piece_black_p;
	select_piece_black_p->next = select_piece_black_b;
	select_piece_black_b->next = select_piece_black_n;
	select_piece_black_n->next = select_piece_black_r;
	select_piece_black_r->next = select_piece_black_q;
	select_piece_black_q->next = select_piece_black_k;
	select_piece_black_k->next = select_piece_white_p;
	select_piece_white_p->next = select_piece_white_b;
	select_piece_white_b->next = select_piece_white_n;
	select_piece_white_n->next = select_piece_white_r;
	select_piece_white_r->next = select_piece_white_q;
	select_piece_white_q->next = select_piece_white_k;
	select_piece_white_k->next = mainmenu_b;
	mainmenu_b->next = set_b;
	set_b->next = remove_b;
	remove_b->next = clear_b;
	clear_b->next = done_b;
	done_b->next = message_b;
	button *temp = change_board_w->kid;
	while( temp != NULL )
	{
		refresh_button(change_board_w, temp);
		temp->active = ( temp->name == 'r' || temp->name == 's' || temp->name == 'm' || temp->name == 'd'|| temp->name == 'z' ) ? 1:0;
		temp = temp->next;
		
	}
	
	//set arrows:
	select_piece_white_k->right = gui_board_set[0][0];
	select_piece_black_k->left = gui_board_set[BOARD_SIZE-1][0];
	select_piece_white_q->right = gui_board_set[0][0];
	select_piece_black_q->left = gui_board_set[BOARD_SIZE-1][0];
	select_piece_white_r->right = gui_board_set[0][3];
	select_piece_black_r->left = gui_board_set[BOARD_SIZE-1][3];
	select_piece_white_n->right = gui_board_set[0][3];
	select_piece_black_n->left = gui_board_set[BOARD_SIZE-1][3];
	set_b->up = gui_board_set[3][0];
	select_piece_white_b->right = gui_board_set[0][6];
	select_piece_black_b->left = gui_board_set[BOARD_SIZE-1][6];
	select_piece_white_p->right = gui_board_set[0][7];
	select_piece_black_p->left = gui_board_set[BOARD_SIZE-1][7];
	remove_b->up = gui_board_set[6][0];
	for ( int i = 0; i < BOARD_SIZE; i++ )
	{
		if ( i < 3 )
		{
			gui_board_set[0][i]->left = select_piece_white_k;
			gui_board_set[BOARD_SIZE-1][i]->right = select_piece_black_k;
			gui_board_set[i][0]->down = mainmenu_b;
		}
		else if ( i < 6 )
		{
			gui_board_set[0][i]->left = select_piece_white_r;
			gui_board_set[BOARD_SIZE-1][i]->right = select_piece_black_r;
			gui_board_set[i][0]->down = set_b;			
		}
		else
		{
			gui_board_set[0][i]->left = select_piece_white_p;
			gui_board_set[BOARD_SIZE-1][i]->right = select_piece_black_p;
			gui_board_set[i][0]->down = remove_b;			
			
		}
		
	}
	select_piece_black_p->down = select_piece_black_b;
	select_piece_black_b->down = select_piece_black_n;
	select_piece_black_b->up = select_piece_black_p;
	select_piece_black_n->down = select_piece_black_r;
	select_piece_black_n->up = select_piece_black_b;
	select_piece_black_r->down = select_piece_black_q;
	select_piece_black_r->up = select_piece_black_n;
	select_piece_black_q->down = select_piece_black_k;
	select_piece_black_q->up = select_piece_black_r;
	select_piece_black_k->down = done_b;
	select_piece_black_k->up = select_piece_black_q;
	done_b->up = select_piece_black_k;
	select_piece_white_p->down = select_piece_white_b;
	select_piece_white_b->down = select_piece_white_n;
	select_piece_white_b->up = select_piece_white_p;
	select_piece_white_n->down = select_piece_white_r;
	select_piece_white_n->up = select_piece_white_b;
	select_piece_white_r->down = select_piece_white_q;
	select_piece_white_r->up = select_piece_white_n;
	select_piece_white_q->down = select_piece_white_k;
	select_piece_white_q->up = select_piece_white_r;
	select_piece_white_k->down = mainmenu_b;
	select_piece_white_k->up = select_piece_white_q;
	mainmenu_b->up = select_piece_white_k;
	mainmenu_b->right = set_b;
	set_b->right = remove_b;
	set_b->left = mainmenu_b;
	remove_b->right = clear_b;
	remove_b->left = set_b;
	clear_b->right = done_b;
	clear_b->left = remove_b;
	clear_b->up = select_piece_black_k;
	done_b->left = remove_b;
	
	
	
	change_board_w->kid->highlight = 1;
	refresh_button(change_board_w,change_board_w->kid);
	return change_board_w;
}

widget *init_load_game(void)
{
	button *slots[12];
	int width_b = 203;
	int height_b = 54;
	int width_b2 = 287;
	int height_b2 = 45;
	int offset_w = width_b2+26;
	int offset_h = height_b2+20;
	int x_start = 100;
	int y_start = 150;
	int jump_row = -1;
	int number_of_rows = 0;
	int number_of_buts_last_row = MEMORY_SLOTS%2 ? MEMORY_SLOTS%2 : 2;

	for ( int i = 0; i < 12; slots[i] = NULL, i++ );
	for (int j = MEMORY_SLOTS; j > 0; ++number_of_rows, j = j-2); 

	widget *load_game_w = build_panel(0, 0, WIN_W, WIN_H, "pics/load_game/load_game_w.bmp", 5);
	
	button *mainmenu_b = build_button(35,540, width_b, height_b, load_image("pics/load_game/mainmenu_b.bmp", 0), 
																	   load_image("pics/load_game/mainmenu_b2.bmp", 0), 
																	   load_image("pics/load_game/mainmenu_b3.bmp", 0), 
																	   NULL, 
																	   NULL, 'm', 0);

	load_game_w->kid = mainmenu_b;
	button *temp = load_game_w->kid;
	char filename_sr[] = "pics/load_game/game_A_b.bmp";
	char filename_sr2[] = "pics/load_game/game_A_b2.bmp";
	char filename_sr3[] = "pics/load_game/game_A_b3.bmp";
	
	for ( int i = 0; i < MEMORY_SLOTS; i++)
	{
		filename_sr[20] = 'A' + i;
		filename_sr2[20] = 'A' + i;
		filename_sr3[20] = 'A' + i;

		jump_row = ( i%2 == 0 ) ? jump_row + 1 : jump_row; 
		button *slot_b = build_button(x_start + (offset_w*(i%2)), y_start + (offset_h*jump_row), width_b2, height_b2, 
																	   load_image(filename_sr, 0), 
																	   load_image(filename_sr2, 0), 
																	   load_image(filename_sr3, 0), 
																	   NULL, 
																	   NULL, 'A'+i, 0);
		slots[i] = slot_b;
		temp->next = slot_b;
		temp = temp->next;
	}

	for ( int i = 0; i < MEMORY_SLOTS; i++)
	{
		slots[i]->right = ((i+1 < MEMORY_SLOTS) && (slots[i+1]!=NULL) && ((i+1)%2 != 0)) ? slots[i+1] : NULL; 
		slots[i]->left = ((i-1 >= 0) && (slots[i-1]!=NULL) && ((i-1)%2 != 1)) ? slots[i-1] : NULL;
		slots[i]->up = ((i-2 >= 0) && (slots[i-2]!=NULL)) ? slots[i-2] : NULL;
		slots[i]->down = ((i+2 < MEMORY_SLOTS) && (slots[i+2]!=NULL)) ? slots[i+2] : NULL;
		if ( i >= MEMORY_SLOTS - number_of_buts_last_row  )
		{
			slots[i]->down = mainmenu_b;
			if ( i%2 == 0 ) mainmenu_b->up = slots[i];
		}
	}

	temp = load_game_w->kid;
	while( temp != NULL )
	{
		refresh_button(load_game_w, temp);
		temp = temp->next;
		
	}
	slots[MEMORY_SLOTS-1]->next = load_game_w->kid;
	load_game_w->kid = load_game_w->kid->next;
	mainmenu_b->next = NULL;
	load_game_w->kid->highlight = 1;
	refresh_button(load_game_w, load_game_w->kid);
	return load_game_w;
}
widget *init_save_game(void)
{
	button *slots[12];
	int width_b = 203;
	int height_b = 54;
	int width_b2 = 287;
	int height_b2 = 45;
	int offset_w = width_b2+26;
	int offset_h = height_b2+20;
	int x_start = 100;
	int y_start = 150;
	int jump_row = -1;
	int number_of_rows = 0;
	int number_of_buts_last_row = MEMORY_SLOTS%2 ? MEMORY_SLOTS%2 : 2;

	for ( int i = 0; i < 12; slots[i] = NULL, i++ );
	for (int j = MEMORY_SLOTS; j > 0; ++number_of_rows, j = j-2); 

	widget *save_game_w = build_panel(0, 0, WIN_W, WIN_H, "pics/load_game/save_game_w.bmp", 7);
	
	button *mainmenu_b = build_button(35,540, width_b, height_b, load_image("pics/load_game/mainmenu_b.bmp", 0), 
																	   load_image("pics/load_game/mainmenu_b2.bmp", 0), 
																	   load_image("pics/load_game/mainmenu_b3.bmp", 0), 
																	   NULL, 
																	   NULL, 'm', 0);


																	   
	button *back_b = build_button(253,540, width_b, height_b, load_image("pics/set_diff/back_b.bmp", 0), 
															load_image("pics/set_diff/back_b2.bmp", 0), 
															load_image("pics/set_diff/back_b3.bmp", 0), 
															NULL, 
															NULL, 'B', 0);
																	   
	save_game_w->kid = mainmenu_b;
	mainmenu_b->next = back_b;
	button *temp = save_game_w->kid->next;
	char filename_sr[] = "pics/load_game/game_A_b.bmp";
	char filename_sr2[] = "pics/load_game/game_A_b2.bmp";
	char filename_sr3[] = "pics/load_game/game_A_b3.bmp";
	
	for ( int i = 0; i < MEMORY_SLOTS; i++)
	{
		filename_sr[20] = 'A' + i;
		filename_sr2[20] = 'A' + i;
		filename_sr3[20] = 'A' + i;

		jump_row = ( i%2 == 0 ) ? jump_row + 1 : jump_row; 
		button *slot_b = build_button(x_start + (offset_w*(i%2)), y_start + (offset_h*jump_row), width_b2, height_b2, 
																	   load_image(filename_sr, 0), 
																	   load_image(filename_sr2, 0), 
																	   load_image(filename_sr3, 0), 
																	   NULL, 
																	   NULL, 'A'+i, 0);
		slots[i] = slot_b;
		temp->next = slot_b;
		temp = temp->next;
	}

	for ( int i = 0; i < MEMORY_SLOTS; i++)
	{
		slots[i]->right = ((i+1 < MEMORY_SLOTS) && (slots[i+1]!=NULL) && ((i+1)%2 != 0)) ? slots[i+1] : NULL; 
		slots[i]->left = ((i-1 >= 0) && (slots[i-1]!=NULL) && ((i-1)%2 != 1)) ? slots[i-1] : NULL;
		slots[i]->up = ((i-2 >= 0) && (slots[i-2]!=NULL)) ? slots[i-2] : NULL;
		slots[i]->down = ((i+2 < MEMORY_SLOTS) && (slots[i+2]!=NULL)) ? slots[i+2] : NULL;
		if ( i >= MEMORY_SLOTS - number_of_buts_last_row  )
		{
			slots[i]->down = mainmenu_b;
			if ( i%2 == 0 ) mainmenu_b->up = slots[i];
		}
	}

	temp = save_game_w->kid;
	while( temp != NULL )
	{
		refresh_button(save_game_w, temp);
		temp = temp->next;
		
	}
	mainmenu_b->right = back_b;
	back_b->left = mainmenu_b;
	back_b->up = mainmenu_b->up;
	slots[MEMORY_SLOTS-1]->next = save_game_w->kid;
	save_game_w->kid = save_game_w->kid->next->next;
	back_b->next = NULL;
	save_game_w->kid->highlight = 1;
	refresh_button(save_game_w, save_game_w->kid);
	return save_game_w;
}
widget *init_promotion(void)
{
	button *temp = NULL;
	int width_b = 203;
	int height_b = 54;
	int width_b2 = 164;
	int x_start = 47;
	int y_start = 250;
	int space = 26;
	
	widget *promotion_w = build_panel(0, 0, WIN_W, WIN_H, "pics/promote/promotion_w.bmp", 8);
	
	button *mainmenu_b = build_button(35,540, width_b, height_b, load_image("pics/load_game/mainmenu_b.bmp", 0), 
																	   load_image("pics/load_game/mainmenu_b2.bmp", 0), 
																	   load_image("pics/load_game/mainmenu_b3.bmp", 0), 
																	   NULL, 
																	   NULL, 'm', 0);
	button *q_b = build_button(x_start + (width_b2+space)*0,y_start, width_b2, height_b, load_image("pics/promote/q_b.bmp", 0), 
															load_image("pics/promote/q_b2.bmp", 0), 
															load_image("pics/promote/q_b3.bmp", 0), 
															NULL, 
															NULL, 'Q', 0);
	button *r_b = build_button(x_start + (width_b2+space)*1,y_start, width_b2, height_b, load_image("pics/promote/r_b.bmp", 0), 
															load_image("pics/promote/r_b2.bmp", 0), 
															load_image("pics/promote/r_b3.bmp", 0), 
															NULL, 
															NULL, 'R', 0);
	button *n_b = build_button(x_start + (width_b2+space)*2,y_start, width_b2, height_b, load_image("pics/promote/n_b.bmp", 0), 
															load_image("pics/promote/n_b2.bmp", 0), 
															load_image("pics/promote/n_b3.bmp", 0), 
															NULL, 
															NULL, 'N', 0);
	button *b_b = build_button(x_start + (width_b2+space)*3,y_start, width_b2, height_b, load_image("pics/promote/b_b.bmp", 0), 
															load_image("pics/promote/b_b2.bmp", 0), 
															load_image("pics/promote/b_b3.bmp", 0), 
															NULL, 
															NULL, 'B', 0);															
	promotion_w->kid = q_b;
	q_b->next = r_b; r_b->next = n_b; n_b->next = b_b; b_b->next = mainmenu_b;

	q_b->right = r_b;
	q_b->down = mainmenu_b;
	r_b->left = q_b;
	r_b->right = n_b;
	r_b->down = mainmenu_b;
	n_b->left = r_b;
	n_b->right = b_b;
	n_b->down = mainmenu_b;
	b_b->left = n_b;
	b_b->down = mainmenu_b;
	mainmenu_b->up = q_b;

	temp = promotion_w->kid;
	promotion_w->kid->highlight = 1;
	while( temp != NULL )
	{
		refresh_button(promotion_w, temp);
		temp = temp->next;
		
	}
	return promotion_w;
}

widget *init_choose_depth(void) 
{
	button *temp = NULL;
	int width_b = 203;
	int height_b = 54;
	int width_b_2 = 76;
	int x_start = 122;
	int y_start = 216;
	int space = 120;
	widget *choose_depth_w = build_panel(0, 0, WIN_W, WIN_H, "pics/choose_depth/choose_depth_w.bmp", CHOOSE_DEPTH_W);
	
	button *mainmenu_b = build_button(35,540, width_b, height_b, load_image("pics/load_game/mainmenu_b.bmp", 0), 
																	   load_image("pics/load_game/mainmenu_b2.bmp", 0), 
																	   load_image("pics/load_game/mainmenu_b3.bmp", 0), 
																	   NULL, 
																	   NULL, 'm', 0);
	button *set_diff_1_b = build_button(x_start,y_start, width_b_2, height_b, load_image("pics/set_diff/set_diff_1_b.bmp", 0), 
																	load_image("pics/set_diff/set_diff_1_b2.bmp", 0), 
																	load_image("pics/set_diff/set_diff_1_b3.bmp", 0), 
																	load_image("pics/set_diff/set_diff_1_b4.bmp", 0), 
																	load_image("pics/set_diff/set_diff_1_b5.bmp", 0), '1', 0);
	
	button *set_diff_2_b = build_button(x_start + space, y_start, width_b_2, height_b, load_image("pics/set_diff/set_diff_2_b.bmp", 0), 
																load_image("pics/set_diff/set_diff_2_b2.bmp", 0), 
																load_image("pics/set_diff/set_diff_2_b3.bmp", 0), 
																load_image("pics/set_diff/set_diff_2_b4.bmp", 0), 
																load_image("pics/set_diff/set_diff_2_b5.bmp", 0), '2', 0);
	
	button *set_diff_3_b = build_button(x_start + space*2, y_start, width_b_2, height_b, load_image("pics/set_diff/set_diff_3_b.bmp", 0), 
															load_image("pics/set_diff/set_diff_3_b2.bmp", 0), 
															load_image("pics/set_diff/set_diff_3_b3.bmp", 0), 
															load_image("pics/set_diff/set_diff_3_b4.bmp", 0), 
															load_image("pics/set_diff/set_diff_3_b5.bmp", 0), '3', 0);

	button *set_diff_4_b = build_button(x_start + space*3, y_start, width_b_2, height_b, load_image("pics/set_diff/set_diff_4_b.bmp", 0), 
															load_image("pics/set_diff/set_diff_4_b2.bmp", 0), 
															load_image("pics/set_diff/set_diff_4_b3.bmp", 0), 
															load_image("pics/set_diff/set_diff_4_b4.bmp", 0), 
															load_image("pics/set_diff/set_diff_4_b5.bmp", 0), '4', 0);

	button *set_diff_5_b = build_button(x_start + space*4, y_start, width_b_2, height_b, load_image("pics/set_diff/set_diff_5_b.bmp", 0), 
															load_image("pics/set_diff/set_diff_5_b2.bmp", 0), 
															load_image("pics/set_diff/set_diff_5_b3.bmp", 0), 
															load_image("pics/set_diff/set_diff_5_b4.bmp", 0), 
															load_image("pics/set_diff/set_diff_5_b5.bmp", 0), '5', 0);
	
	choose_depth_w->kid = set_diff_1_b;
	set_diff_1_b->next = set_diff_2_b; 
	set_diff_2_b->next = set_diff_3_b; 
	set_diff_3_b->next = set_diff_4_b; 
	set_diff_4_b->next = set_diff_5_b;
	set_diff_5_b->next = mainmenu_b;
	
	set_diff_1_b->right = set_diff_2_b;
	set_diff_1_b->down = mainmenu_b;
	set_diff_2_b->left = set_diff_1_b;
	set_diff_2_b->right = set_diff_3_b;
	set_diff_2_b->down = mainmenu_b;
	set_diff_3_b->left = set_diff_2_b;
	set_diff_3_b->right = set_diff_4_b;
	set_diff_3_b->down = mainmenu_b;
	set_diff_4_b->left = set_diff_3_b;
	set_diff_4_b->down = mainmenu_b;
	mainmenu_b->up = set_diff_1_b;
	
	temp = choose_depth_w->kid;
	choose_depth_w->kid->highlight = 1;
	while( temp != NULL )
	{
		refresh_button(choose_depth_w, temp);
		temp = temp->next;
		
	}
	return choose_depth_w;
	
}
widget *init_play_game(void)
{
	int width_b_1 = 203;
	int width_b_2 = 104;
	int width_b_3 = 110; //best move w
	int height_b = 54;
	int height_b2 = 100; //best move h
	widget *game_w = build_panel(0, 0, WIN_W, WIN_H, "pics/play_game/play_game_w.bmp", 6);
	
	button *mainmenu_b = build_button(35,540, width_b_1, height_b, load_image("pics/play_game/mainmenu_b.bmp", 0), 
																	   load_image("pics/play_game/mainmenu_b2.bmp", 0), 
																	   load_image("pics/play_game/mainmenu_b3.bmp", 0), 
																	   NULL, 
																	   NULL, 'm', 0);
	button *savegame_b = build_button(562,540, width_b_1, height_b, load_image("pics/play_game/savegame_b.bmp", 0), 
																	   load_image("pics/play_game/savegame_b2.bmp", 0), 
																	   load_image("pics/play_game/savegame_b3.bmp", 0), 
																	   NULL, 
																	   NULL, 's', 0);
	button *bestmove_black_b = build_button(661,420, width_b_3, height_b2, load_image("pics/play_game/bestmove_b.bmp", 0), 
																	   load_image("pics/play_game/bestmove_b2.bmp", 0), 
																	   load_image("pics/play_game/bestmove_b3.bmp", 0), 
																	   load_image("pics/play_game/bestmove_b4.bmp", 0), 
																	   load_image("pics/play_game/bestmove_b4.bmp", 0), 'B', 0);
	button *bestmove_white_b = build_button(35,420, width_b_3, height_b2, load_image("pics/play_game/bestmove_b.bmp", 0), 
																	   load_image("pics/play_game/bestmove_b2.bmp", 0), 
																	   load_image("pics/play_game/bestmove_b3.bmp", 0), 
																	   load_image("pics/play_game/bestmove_b4.bmp", 0), 
																	   load_image("pics/play_game/bestmove_b4.bmp", 0), 'W', 0);
	button *white_turn_b = build_button(35,100, width_b_2, height_b, load_image("pics/play_game/turn_b.bmp", 0), 
																	   NULL, 
																	   NULL, 
																	   load_image("pics/play_game/turn_b2.bmp", 0), 
																	   NULL, 't', 0);
	button *black_turn_b = build_button(661,100, width_b_2, height_b, load_image("pics/play_game/turn_b.bmp", 0), 
																	   NULL, 
																	   NULL, 
																	   load_image("pics/play_game/turn_b2.bmp", 0), 
																	   NULL, 'T', 0);
	button *white_check_b = build_button(35,140, width_b_2, height_b, load_image("pics/play_game/check_b.bmp", 0), 
																	   NULL, 
																	   NULL, 
																	   load_image("pics/play_game/check_b2.bmp", 0), 
																	   NULL, 'c', 0);
	button *black_check_b = build_button(661,140, width_b_2, height_b, load_image("pics/play_game/check_b.bmp", 0), 
																	   NULL, 
																	   NULL, 
																	   load_image("pics/play_game/check_b2.bmp", 0), 
																	   NULL, 'C', 0);
	button *message1_b = build_button(186,495, 427, 35, load_image("pics/play_game/gm_white_trn.bmp", 0), 
																load_image("pics/play_game/gm_black_turn.bmp", 0), 
																load_image("pics/play_game/gm_computer_turn.bmp", 0), 
																load_image("pics/play_game/msg_w.bmp", 0), 
																NULL,'1', 0);
	
	
	//set next pointers
	game_w->kid = gui_board_game[0][BOARD_SIZE-1];
	gui_board_game[BOARD_SIZE-1][0]->next = bestmove_white_b;
	bestmove_white_b->next = bestmove_black_b;
	bestmove_black_b->next = mainmenu_b;
	mainmenu_b->next = savegame_b;
	savegame_b->next = white_turn_b;
	white_turn_b->next = black_turn_b;
	black_turn_b->next = white_check_b;
	white_check_b->next = black_check_b;
	black_check_b->next = message1_b;

	
	//set arrows:
	mainmenu_b->right = savegame_b;
	mainmenu_b->up = bestmove_white_b;
	bestmove_white_b->right = gui_board_game[0][0];
	bestmove_white_b->down = mainmenu_b; 
	bestmove_black_b->left = gui_board_game[BOARD_SIZE-1][0];	
	bestmove_black_b->down = savegame_b;
	savegame_b->left = mainmenu_b;
	savegame_b->up = bestmove_black_b;
	for ( int i = 0; i < BOARD_SIZE; i++ )
	{
		gui_board_game[0][i]->left = bestmove_white_b;
		gui_board_game[BOARD_SIZE-1][i]->right = bestmove_black_b;
		if ( i < 4 )
		{
			
			gui_board_game[i][0]->down = mainmenu_b;
		}
		else
		{
			gui_board_game[i][0]->down = savegame_b;			
			
		}
	}
	button *temp = game_w->kid;
	temp->highlight = 1;
	while( temp != NULL )
	{
		temp->active = ( temp->name == 'W' || temp->name == 's' || temp->name == 'm' || temp->board_button ) ? 1:0; 
		temp->special = ( temp->name == 't' ) ? 1:0;
		refresh_button(game_w, temp);
		temp = temp->next;
		
	}
	return game_w;

}
widget *init_declare_winner(void)
{
	
	int width_b = 203;
	int height_b = 54; 
	widget *declare_w = build_panel(0, 0, WIN_W, WIN_H, "pics/declare_winner/play_game_w.bmp", 10);
	
	button *declare_b = build_button(0, 0, WIN_W, WIN_H, load_image("pics/declare_winner/play_game_w.bmp", 0), 
															load_image("pics/declare_winner/white_wins.bmp", 0),
															load_image("pics/declare_winner/black_wins.bmp", 0), 
															load_image("pics/declare_winner/its_a_tie.bmp", 0), 
															NULL, 'd', 0);
	
	button *mainmenu_b = build_button(35, 540, width_b, height_b, load_image("pics/load_game/mainmenu_b.bmp", 0), 
																	   load_image("pics/load_game/mainmenu_b2.bmp", 0), 
																	   load_image("pics/load_game/mainmenu_b3.bmp", 0), 
																	   NULL, 
																	   NULL, 'm', 0);
	
	button *back_b = build_button(355, 540, 90, height_b, load_image("pics/set_diff/back_b.bmp", 0), 
															load_image("pics/set_diff/back_b2.bmp", 0), 
															load_image("pics/set_diff/back_b3.bmp", 0), 
															NULL, 
															NULL, 'B', 0);
	declare_w->kid = mainmenu_b;
	mainmenu_b->next = back_b;
	back_b->next = declare_b;
	
	declare_b->active = 0;
	mainmenu_b->right = back_b;
	back_b->left = mainmenu_b;
	
	declare_w->kid->highlight = 1;
	refresh_button(declare_w, declare_w->kid->next->next);
	refresh_button(declare_w, declare_w->kid->next);
	refresh_button(declare_w, declare_w->kid);
	return declare_w;
}

//---------------------------------------------------------------------------------//
	/* ----board methods----*/
void init_gui_board(button *gui_board[BOARD_SIZE][BOARD_SIZE])
{
	
	int row;
	int column;
	int cell_size = 60;
	
	for(row = 0; row < BOARD_SIZE; row++ )
	{
		for(column = 0; column < BOARD_SIZE; column++ )
		{	
			if ((column + row) % 2 == 1)
			{
				gui_board[column][row] = build_button(160 + cell_size*column,  5 + cell_size*(BOARD_SIZE-1-row), cell_size, cell_size,
													 load_image("pics/board/white_sq_b.bmp",0),
													 load_image("pics/board/white_sq_b2.bmp",0),
													 load_image("pics/board/white_sq_b2.bmp",0),
													 load_image("pics/board/white_sq_b3.bmp",0),
													 load_image("pics/board/white_sq_b4.bmp",0),'w', 1);
				gui_board[column][row]->row = row;
				gui_board[column][row]->column = column;
			}
			else
			{
				gui_board[column][row] = build_button(160+ cell_size*column,  5 + cell_size*(BOARD_SIZE-1-row), cell_size, cell_size, 
													 load_image("pics/board/black_sq_b.bmp",0),
													 load_image("pics/board/black_sq_b2.bmp",0),
													 load_image("pics/board/black_sq_b2.bmp",0),
													 load_image("pics/board/black_sq_b3.bmp",0),
													 load_image("pics/board/black_sq_b4.bmp",0),'b', 1);
				gui_board[column][row]->row = row;
				gui_board[column][row]->column = column;
			}
		}
	}
	for(row = 0; row < BOARD_SIZE; row++ ) // set cells arrows and next pointers 
	{
		for(column = 0; column < BOARD_SIZE; column++ )
		{	
			if (row != 0 ) gui_board[column][row]->down = gui_board[column][row-1];
			if (column != 0 ) gui_board[column][row]->left = gui_board[column-1][row];
			if (column != BOARD_SIZE-1 ) 
			{
					gui_board[column][row]->right = gui_board[column+1][row];
					gui_board[column][row]->next = gui_board[column+1][row];
					
			}
			else
			{
				if (row != 0 ) gui_board[column][row]->next = gui_board[0][row-1];
			}
			if (row != BOARD_SIZE-1 ) gui_board[column][row]->up = gui_board[column][row+1];
		}
	}
}

void load_board_to_screen(widget *panel , button *gui_board[BOARD_SIZE][BOARD_SIZE])
{
	int row;
	int column;
	for(row = 0; row < BOARD_SIZE; row++ )  
	{
		for(column = 0; column < BOARD_SIZE; column++ )
		{
			refresh_button(panel, gui_board[column][row]);
			
		}
	}
}

SDL_Surface *get_piece_sr(char piece)
{
	switch(piece)
	{
		case (EMPTY):
			return piece_empty;
			break;
		case (WHITE_P):
			return piece_white_p;
			break;
		case (WHITE_B):
			return piece_white_b;
			break;
		case (WHITE_N):
			return piece_white_n;
			break;
		case (WHITE_R):
			return piece_white_r;
			break;
		case (WHITE_Q):
			return piece_white_q;
			break;
		case (WHITE_K):
			return piece_white_k;
			break;
		case (BLACK_P):
			return piece_black_p;
			break;
		case (BLACK_B):
			return piece_black_b;
			break;
		case (BLACK_N):
			return piece_black_n;
			break;
		case (BLACK_R):
			return piece_black_r;
			break;
		case (BLACK_Q):
			return piece_black_q;
			break;
		case (BLACK_K):
			return piece_black_k;
			break;
	}
	return NULL;
}

void toggle_gui_board_active(int on_off, int clean_selected, button *gui_board[BOARD_SIZE][BOARD_SIZE])
{
	int row;
	int column;
	if (gui_board_from[0] != -1)
	{

		gui_board_from[0] = -1;
		gui_board_from[1] = -1;
	}
	if (gui_board_to[0] != -1)
	{
		gui_board_to[0] = -1;
		gui_board_to[1] = -1;
	}
	for(row = 0; row < BOARD_SIZE; row++ )  
	{
		for(column = 0; column < BOARD_SIZE; column++ )
		{
			if(clean_selected) gui_board[column][row]->special = 0;
			refresh_button(window->child,gui_board[column][row]);
			gui_board[column][row]->active = on_off;
		}
	}
}

//---------------------------------------------------------------------------------//
		/* ----General methods---- */
/** frees all the gui's resources. */
void free_gui(void)
{
	free_widget(welcome_to_chess_w); // id: -1
	free_widget(main_w); // id: 1
	free_widget(settings_w); // id: 2
	free_widget(set_diff_w); // id: 3
	free_widget(change_board_w); // id: 4
	free_widget(load_game_w); // id: 5
	free_widget(play_game_w); // id: 6
	free_widget(save_game_w); // id: 7
	free_widget(promotion_w); // id: 8
	free_widget(choose_depth_w); // id: 9
	free_widget(declare_winner_w); // id 10
	free_widget(window); // id: 0
	SDL_FreeSurface(icon_pic);
	SDL_FreeSurface(piece_empty);
	SDL_FreeSurface(piece_white_p);
	SDL_FreeSurface(piece_white_b);
	SDL_FreeSurface(piece_white_n);
	SDL_FreeSurface(piece_white_r);
	SDL_FreeSurface(piece_white_q);
	SDL_FreeSurface(piece_white_k);
	SDL_FreeSurface(piece_black_p);
	SDL_FreeSurface(piece_black_b);
	SDL_FreeSurface(piece_black_n);
	SDL_FreeSurface(piece_black_r);
	SDL_FreeSurface(piece_black_q);
	SDL_FreeSurface(piece_black_k);
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
//	SDL_VideoQuit();
	SDL_Quit();
}
void free_widget(widget *panel)
{
	if (panel != NULL)
	{
	free_button(panel->kid);
	SDL_FreeSurface(panel->sr);
	free(panel);
	}
}
void free_button(button *but)
{
	if ( but != NULL )
	{
		free_button(but->next);
		if ( but->sr != NULL ) SDL_FreeSurface(but->sr);
		if ( but->sr2 != NULL ) SDL_FreeSurface(but->sr2);
		if ( but->sr3 != NULL ) SDL_FreeSurface(but->sr3);
		if ( but->sr4 != NULL ) SDL_FreeSurface(but->sr4);
		if ( but->sr5 != NULL ) SDL_FreeSurface(but->sr5);
		free(but);
	}
} 
void do_move_gui(move *real_move)
{
	do_move(board, real_move);
	gui_board_game[real_move->to->column][real_move->to->row]->special = 0;
	refresh_button(play_game_w, gui_board_game[real_move->to->column][real_move->to->row]);
	gui_board_game[real_move->from->column][real_move->from->row]->special = 0;
	refresh_button(play_game_w, gui_board_game[real_move->from->column][real_move->from->row]);
	gui_board_from[0] = -1;
	gui_board_from[1] = -1;
	gui_board_to[0] = -1;
	gui_board_to[1] = -1;
	switch_turn_gui();
	if ( (GAME_STATUS = game_over()) ) // check if the game is over after the move.
	{
		declare_winner_gui();
	}
	else // the game is not over.
	{
		if (!TWO_PLAYERS_MODE) // the computer turn
		{
			apply_surface(0, 0, window->child->sr, window->sr);
			if (SDL_Flip(window->sr) != 0) 
			{
				printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());	
			}
			
			play_computer_turn();
			toggle_gui_board_active(1, 0, gui_board_game);
			if (SDL_Flip(window->sr) != 0) {
				printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
			}
			if ( (GAME_STATUS = game_over()) ) // check if the game is over after the move.
			{
				declare_winner_gui();
			}
			else switch_turn_gui();// the game is not over, player turn.
		}
	}
}
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
			Uint32 colorkey = SDL_MapRGB(optimizedImage->format, 0, 255, 0); 
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
    //Initialize all SDL subsystems
	if ( SDL_Init(SDL_INIT_VIDEO) == -1 ) return 1;
/*     if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return 1;    
    } */
	icon_pic = SDL_LoadBMP("pics/icon.bmp");
	SDL_WM_SetIcon(icon_pic, NULL);
	
	//build window
    window = build_window();

    //Set up the screen
    //If there was an error in setting up the screen
    if( window->sr == NULL )
    {
		printf("ERROR: failed to init main window: %s\n", SDL_GetError());
		return 1;    
    }
    //Set the window caption
    SDL_WM_SetCaption( "Chess Prog", NULL );
	
	//initialize the pieces;
	piece_empty = load_image("pics/board/piece_empty.bmp",1);
	piece_white_p = load_image("pics/board/piece_white_p.bmp",1);
	piece_white_b = load_image("pics/board/piece_white_b.bmp",1);
	piece_white_n = load_image("pics/board/piece_white_n.bmp",1);
	piece_white_r = load_image("pics/board/piece_white_r.bmp",1);
	piece_white_q = load_image("pics/board/piece_white_q.bmp",1);
	piece_white_k = load_image("pics/board/piece_white_k.bmp",1);
	piece_black_p = load_image("pics/board/piece_black_p.bmp",1);
	piece_black_b = load_image("pics/board/piece_black_b.bmp",1);
	piece_black_n = load_image("pics/board/piece_black_n.bmp",1);
	piece_black_r = load_image("pics/board/piece_black_r.bmp",1);
	piece_black_q = load_image("pics/board/piece_black_q.bmp",1);
	piece_black_k = load_image("pics/board/piece_black_k.bmp",1);
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
	if (butt->board_button)
	{
		apply_surface(butt->rct.x+5, butt->rct.y+5, get_piece_sr(board[butt->column][butt->row]),panel->sr);
	}
}

void reset_panel(widget * panel)
{
	int w_id = panel->id;
	button *but = panel->kid;
	switch(w_id)
	{
		case (MAIN_W): 
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
		case (SETTINGS_W): 
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
		case (SET_DIFFICULTY_W): 
			while(but != NULL)
				{
					but->active = (but->name == 'w' || but->name == '1') ? 0 : 1;
					but->selected = 0;
					but->highlight = 0;
					but->special  = (but->name == 'w' ||  but->name == '1') ? 1: 0;
					refresh_button(panel, but);
					but = but->next;
				}
		case (CHANGE_BOARD_W): 
			while(but != NULL)
				{
					but->active = ( but->name == 'r' || but->name == 's' || but->name == 'm' || but->name == 'd' || but->name == 'z' ) ? 1:0;
					but->selected = 0;
					but->highlight = 0;
					but->special  = 0;
					refresh_button(panel, but);
					but = but->next;
				}
			break;
		case (LOAD_GAME_W):
			while(but != NULL)
				{
					but->highlight = 0;
					refresh_button(panel, but);
					but = but->next;
				}
			break;
		case (SAVE_GAME_W): 
				{
					but->highlight = 0;
					refresh_button(panel, but);
					but = but->next;
				}
			break;
		case (PLAY_GAME_W): 
			while(but != NULL)
				{
				
					
					but->active = ( but->name == 'W' || but->name == 's' || but->name == 'm' || but->board_button ) ? 1:0; 
			
		

					but->highlight = 0;
					refresh_button(panel, but);
					but = but->next;
				}
			break;
		case (PROMOTION_W): 
			while(but != NULL)
				{
					but->highlight = 0;
					refresh_button(panel, but);
					but = but->next;
				}
			break;
		case (CHOOSE_DEPTH_W): 
			while(but != NULL)
				{
					but->highlight = 0;
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

void switch_turn_gui(void)
{
	WHITE_TURN = (WHITE_TURN + 1)%2;
	char best_move_b = WHITE_TURN ?'W':'B';
	char turn_b = WHITE_TURN ?'t':'T';
	char check_b = WHITE_TURN ?'c':'C'; 
	int is_check_now = is_check(board, WHITE_TURN);
	button *temp = play_game_w->kid;
	while( temp != NULL )
	{
		temp->active = ( temp->name == best_move_b || temp->name == 's' || temp->name == 'm' || temp->board_button ) ? 1:0;
		temp->special = (( temp->name == check_b && is_check_now ) || temp->name == turn_b ) ? 1:0;
		if (!TWO_PLAYERS_MODE && ((temp->name == 'W' && !PLAYER_WHITE) || (temp->name == 'B' && PLAYER_WHITE)) )
		{
			temp->active = 0;
			temp->special = 1;
		}
		refresh_button(play_game_w, temp );
		temp = temp->next;
	}
	if(!TWO_PLAYERS_MODE && ((!WHITE_TURN && PLAYER_WHITE) || (WHITE_TURN && !PLAYER_WHITE) ) )
	{
		temp = play_game_w->kid;
		while (temp->next != NULL) temp = temp->next;
		apply_surface(temp->rct.x, temp->rct.y, temp->sr3, play_game_w->sr);
	}
	else if( WHITE_TURN )
	{
		temp = play_game_w->kid;
		while (temp->next != NULL) temp = temp->next;
		apply_surface(temp->rct.x, temp->rct.y, temp->sr, play_game_w->sr);
	}
	else
	{
		temp = play_game_w->kid;
		while (temp->next != NULL) temp = temp->next;
		apply_surface(temp->rct.x, temp->rct.y, temp->sr2, play_game_w->sr);
	}
	
}
void declare_winner_gui(void)
{
	toggle_gui_board_active(0, 1, gui_board_game);
	button *temp = play_game_w->kid;
	while (temp->next != NULL)
	{
		temp->active = ( temp->name == 's' || temp->name == 'm'  ) ? 1:0;
		temp->highlight = 0;
		refresh_button(play_game_w, temp);
		temp = temp->next;
	}
	apply_surface(temp->rct.x, temp->rct.y, temp->sr4, play_game_w->sr);
	
	temp = declare_winner_w->kid->next->next;
	if (GAME_STATUS == 1) apply_surface(temp->rct.x, temp->rct.y, temp->sr4, declare_winner_w->sr); //tie
	if (GAME_STATUS == 2)
	{
		if (WHITE_TURN) apply_surface(temp->rct.x, temp->rct.y, temp->sr2, declare_winner_w->sr); //white wins
		else apply_surface(temp->rct.x, temp->rct.y, temp->sr3, declare_winner_w->sr); //black wins
	}
	if (GAME_STATUS == 3) 
	{
		if (!WHITE_TURN) apply_surface(temp->rct.x, temp->rct.y, temp->sr2, declare_winner_w->sr); //white wins
		else apply_surface(temp->rct.x, temp->rct.y, temp->sr3, declare_winner_w->sr); //black wins
	}
	refresh_button(declare_winner_w, declare_winner_w->kid);
	refresh_button(declare_winner_w, declare_winner_w->kid->next);
	
	play_game_w->kid->highlight = 1;
	refresh_button(play_game_w, play_game_w->kid);
	window->child = declare_winner_w;
	
}
void back_to_default(void) 
{
	init_board(board);
	MINIMAX_DEPTH = 1;
	GAME = 0;
	SETTINGS = 1;
	GAME_STATUS = 0; 
	GUI_MODE = 1; 
	TWO_PLAYERS_MODE = 1; 
	WHITE_TURN = 1; 
	PLAYER_WHITE = 1;
	CHECK_ON_WHITE = 0;
	CHECK_ON_BLACK = 0;
	is_remove_cmd = 0;
	is_set_cmd = 0;
	gui_board_from[0] = -1;
	gui_board_from[1] = -1;
	gui_board_to[0] = -1;
	gui_board_to[1] = -1;
	selected_piece = EMPTY; 
	gui_promote_to = EMPTY;
	
	reset_panel(main_w);
	reset_panel(settings_w);
	reset_panel(set_diff_w);
	reset_panel(change_board_w);
	reset_panel(load_game_w);
	reset_panel(play_game_w);
	reset_panel(save_game_w);
	reset_panel(promotion_w);
	reset_panel(choose_depth_w);

	window->child = main_w;
	apply_surface(window->child->rct.x, window->child->rct.y, window->child->sr, window->sr);
	
}

int activate_buttons_function(button *but)
{
	int win_index = window->child->id;
	but->selected = 0;
	refresh_button(window->child, but);
	switch (win_index)
	{
		case (MAIN_W):
			return activate_button_main_w(but);
			break;
		case (SETTINGS_W):
			activate_button_settings_w(but);
			break;
		case (SET_DIFFICULTY_W):
			activate_button_set_diff_w(but);
			break;
		case (CHANGE_BOARD_W):
			activate_button_change_board_w(but);
			break;
		case (LOAD_GAME_W):
			activate_button_load_game_w(but);
			break;
		case (PLAY_GAME_W):
			activate_button_play_game_w(but);
			break;
		case (SAVE_GAME_W):
			activate_button_save_game_w(but);
			break;
		case (PROMOTION_W):
			activate_button_promotion_w(but);
			break;
		case (CHOOSE_DEPTH_W):
			activate_button_choose_depth_w(but);
			break;
		case (DECLARE_WINNER_W):
			activate_button_declare_winner_w(but);
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
			window->child = load_game_w;
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
		case ('g'):
			but->highlight = 0;
			refresh_button(window->child, but);
			window->child->kid->highlight = 1;
			refresh_button(window->child, window->child->kid);
			window->child = play_game_w;
			toggle_gui_board_active(1, 0, gui_board_game);
			WHITE_TURN = (WHITE_TURN + 1)%2;
			switch_turn_gui();
			gui_board_from[0] = -1;
			gui_board_from[1] = -1;
			gui_board_to[0] = -1;
			gui_board_to[2] = -1;
			break;
		case ('s'): 
			but->highlight = 0;
			refresh_button(window->child, but);
			window->child->kid->highlight = 1;
			refresh_button(window->child, window->child->kid);
			window->child = change_board_w;
			break;
			
	}
}

void activate_button_set_diff_w(button *but)
{
	button *tmp = window->child->kid;
	switch (but->name)
	{
		case ('m'): //main menu button	
			back_to_default();
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
			MINIMAX_DEPTH = BEST_DEPTH_VALUE;
			break;
		case ('B'): //back to settings
			but->highlight = 0;
			refresh_button(window->child, but);
			window->child->kid->highlight = 1;
			refresh_button(window->child, window->child->kid);
			window->child = settings_w;
			break;
		case ('g'):
			but->highlight = 0;
			refresh_button(window->child, but);
			window->child->kid->highlight = 1;
			refresh_button(window->child, window->child->kid);
			window->child = play_game_w;
			toggle_gui_board_active(1, 0, gui_board_game);
			WHITE_TURN = (WHITE_TURN + 1)%2;
			switch_turn_gui();
			gui_board_from[0] = -1;
			gui_board_from[1] = -1;
			gui_board_to[0] = -1;
			gui_board_to[2] = -1;
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

void activate_button_change_board_w(button *but)
{
	button *tmp = change_board_w->kid;
	if(but->board_button)
	{
		but->special = 1;
		location l;
		l.column = but->column;
		l.row = but->row;
		if (is_remove_cmd) remove_location(l); // need to remove from board 
		if (is_set_cmd) // need to set selected_piece on board check if legal. 
		{
			char temp_piece = IS_WHITE(selected_piece) ? selected_piece : selected_piece + ('a'-'A'); 
			if (is_legal_placement(l, selected_piece)) set_location(l, IS_WHITE(selected_piece), temp_piece);
			else but->special = 0;
		}
		refresh_button(window->child, but);
		while(tmp != NULL)
		{
			if(!tmp->board_button)
			{
				tmp->active = (tmp->name  == 'r' || tmp->name  == 'm' || tmp->name  == 's' || tmp->name  == 'd' || tmp->name  == 'z') ? 1:0 ; 
				tmp->special = 0;
				refresh_button(window->child, tmp);
			}
			tmp = tmp->next;
		}
		is_remove_cmd = 0; 
		is_set_cmd = 0;
		toggle_gui_board_active(0,0,gui_board_set);
	}
	else
	{
		switch(but->name)
		{
			case ('m'): // back to main menu
				back_to_default();
				break;
			case ('d'): // done change the board back to settings if board is legal. 
				while(tmp->next != NULL) 
				{
					if (tmp->special)
					{
						tmp->special = 0; 
						refresh_button(change_board_w,tmp);
					}
					tmp = tmp->next;
				}
				if ( !check_settings() ) 
				{
					apply_surface(tmp->rct.x, tmp->rct.y, tmp->sr5, change_board_w->sr);
				}
				else
				{
					apply_surface(tmp->rct.x, tmp->rct.y, tmp->sr, change_board_w->sr);
					but->highlight = 0;
					refresh_button(change_board_w,but);
					change_board_w->kid->highlight = 1; 
					refresh_button(change_board_w,change_board_w->kid);
					window->child = settings_w;
				}
				break;
			case('s'): // initialize set proccess
				is_set_cmd = 1;

				while(tmp != NULL) 
				{
					if (tmp->special)
					{
						tmp->special = 0; 
						refresh_button(change_board_w,tmp);
					}
					tmp->active = ( (tmp->name >= '1' && tmp->name <= '6') || (tmp->name >= 'A' && tmp->name <= 'F') 
										|| (tmp->name == 'm') ) ? 1 : 0;
					if ( tmp->name == 'M' ) apply_surface(tmp->rct.x, tmp->rct.y, tmp->sr4, change_board_w->sr); //choose the appropriate message
					tmp = tmp->next;
				}
				but->special = 1;
				refresh_button(change_board_w, but);
				break;
			case('r'): // initialize remove proccess
				is_remove_cmd = 1;

				while(tmp != NULL) 
				{
					if (tmp->special)
					{
						tmp->special = 0; 
						refresh_button(change_board_w,tmp);
					}
					tmp->active = ( (tmp->board_button) || (tmp->name == 'm') ) ? 1 : 0;
					if ( tmp->name == 'M' ) apply_surface(tmp->rct.x, tmp->rct.y, tmp->sr3, change_board_w->sr); //choose the appropriate message
					tmp = tmp->next;
				}
				but->special = 1;
				refresh_button(change_board_w, but);
				break;
			case('z'): // clear the board
				clear();
				load_board_to_screen(change_board_w, gui_board_set);
				while(tmp != NULL) 
				{
					if (tmp->special)
					{
						tmp->special = 0; 
						refresh_button(change_board_w,tmp);
					}
					tmp->active = (tmp->name  == 'r' || tmp->name  == 'm' || tmp->name  == 's' || tmp->name  == 'd' || tmp->name  == 'z') ? 1:0 ; 
					if ( tmp->name == 'M' ) apply_surface(tmp->rct.x, tmp->rct.y, tmp->sr, change_board_w->sr); //choose the appropriate message
					tmp = tmp->next;
				}
				break;
			case('1'):
				selected_piece = BLACK_P;
				break;
			case('2'):
				selected_piece = BLACK_B;
				break;
			case('3'):
				selected_piece = BLACK_N;
				break;
			case('4'):
				selected_piece = BLACK_R;
				break;
			case('5'):
				selected_piece = BLACK_Q;
				break;
			case('6'):
				selected_piece = BLACK_K;
				break;
			case('A'):
				selected_piece = WHITE_P;
				break;
			case('B'):
				selected_piece = WHITE_B;
				break;
			case('C'):
				selected_piece = WHITE_N;
				break;
			case('D'):
				selected_piece = WHITE_R;
				break;
			case('E'):
				selected_piece = WHITE_Q;
				break;
			case('F'):
				selected_piece = WHITE_K;
				break;				
		}
		if ( (but->name >= '1' && but->name <= '6') || (but->name >= 'A' && but->name <= 'F') )
		{
			while(tmp != NULL) 
			{
				tmp->active = ( (tmp->board_button) || (tmp->name == 'm') ) ? 1 : 0;
				if ( tmp->name == 'M' ) apply_surface(tmp->rct.x, tmp->rct.y, tmp->sr2, change_board_w->sr); //choose the appropriate message
				tmp = tmp->next;
			}
			but->special = 1;
			refresh_button(change_board_w, but);
		}
		
	}
	
}

void activate_button_load_game_w(button *but)
{
	switch (but->name)
	{
		case ('m'): 
			back_to_default(); //main menu button
			break;
		case ('A'): // load this slot
			load_xml("load_save/chess1.xml");
			break;
		case ('B'): // load this slot
			load_xml("load_save/chess2.xml");
			break;
		case ('C'): // load this slot
			load_xml("load_save/chess3.xml");
			break;		
		case ('D'): // load this slot
			load_xml("load_save/chess4.xml");
			break;		
		case ('E'): // load this slot
			load_xml("load_save/chess5.xml");
			break;
		case ('F'): // load this slot
			load_xml("load_save/chess6.xml");
			break;
		case ('G'): // load this slot
			load_xml("load_save/chess7.xml");
			break;
		case ('H'): // load this slot
			load_xml("load_save/chess8.xml");
			break;		
		case ('I'): // load this slot
			load_xml("load_save/chess9.xml");
			break;		
		case ('J'): // load this slot
			load_xml("load_save/chess10.xml");
			break;
		case ('K'): // load this slot
			load_xml("load_save/chess11.xml");
			break;		
		case ('L'): // load this slot
			load_xml("load_save/chess12.xml");
			break;
	}
	if ( but->name != 'm' )
	{
		button *temp = NULL;
		load_board_to_screen(change_board_w, gui_board_set);
		change_board_w->kid->highlight = 1;
		refresh_button(change_board_w, change_board_w->kid);
		temp = settings_w->kid;
		while( temp != NULL )
		{
			switch(temp->name)
			{
				case('c'):
					if ( !TWO_PLAYERS_MODE )
					{
						temp->special = 1;
						temp->active = 0;
						refresh_button(settings_w, temp);
						temp->left->special = 0;
						temp->left->active = 1;
						refresh_button(settings_w, temp->left);
						temp->down->down->down->right->active = 0;
						temp->down->down->down->right = temp->down->down->down->right->next;
						refresh_button(settings_w, temp->down->down->down->right);
						temp->down->down->down->right->active = 1;
					}
					break;
				case('w'):
					if ( !WHITE_TURN ) temp->special = 0;
					if ( !WHITE_TURN ) temp->active = 1;
					refresh_button(settings_w, temp);
					break;
				case('b'):
					if ( !WHITE_TURN ) temp->special = 1;
					if ( !WHITE_TURN ) temp->active = 0;
					refresh_button(settings_w, temp);
					break;
			}
			temp = temp->next;
		}
		temp = set_diff_w->kid;
		while (temp != NULL )
		{
			switch(temp->name)
			{
				case ('b'): // player is black
					if ( !PLAYER_WHITE )
					{
						temp->special = 1;
						temp->active = 0;
						refresh_button(set_diff_w, temp);
						temp->left->special = 0;
						temp->left->active =1;
						refresh_button(set_diff_w, temp->left);
					}
					break;
				
			}			
			if ( (temp->name == '1' + MINIMAX_DEPTH - 1 ) || ((temp->name == '5') && (MINIMAX_DEPTH == -1) ) )
			{
				button *tmp = set_diff_w->kid;
				while(tmp != NULL)
				{
					tmp->special = 0;
					tmp->active =1;
					refresh_button(set_diff_w, tmp);
					tmp = tmp->right;
				}
				temp->special = 1;
				temp->active = 0;
				refresh_button(set_diff_w, temp);
			}			
			
			temp = temp->next;
		}
		but->highlight = 0;
		refresh_button(load_game_w, but);
		load_game_w->kid->highlight = 1;
		refresh_button(load_game_w, load_game_w->kid);
		window->child = settings_w; //go to settings!
	}
}
void activate_button_save_game_w(button *but)
{
	switch (but->name)
	{
		case ('m'): 
			back_to_default(); //main menu button
			break;
		case ('A'): // load this slot
			save_xml("load_save/chess1.xml");
			break;
		case ('B'): // load this slot
			save_xml("load_save/chess2.xml");
			break;
		case ('C'): // load this slot
			save_xml("load_save/chess3.xml");
			break;		
		case ('D'): // load this slot
			save_xml("load_save/chess4.xml");
			break;		
		case ('E'): // load this slot
			save_xml("load_save/chess5.xml");
			break;
		case ('F'): // load this slot
			save_xml("load_save/chess6.xml");
			break;
		case ('G'): // load this slot
			save_xml("load_save/chess7.xml");
			break;
		case ('H'): // load this slot
			save_xml("load_save/chess8.xml");
			break;		
		case ('I'): // load this slot
			save_xml("load_save/chess9.xml");
			break;		
		case ('J'): // load this slot
			save_xml("load_save/chess10.xml");
			break;
		case ('K'): // load this slot
			save_xml("load_save/chess11.xml");
			break;		
		case ('L'): // load this slot
			save_xml("load_save/chess12.xml");
			break;
	}
	if ( but->name != 'm' )
	{
		but->highlight = 0;
		refresh_button(save_game_w, but);
		save_game_w->kid->highlight = 1;
		refresh_button(save_game_w, save_game_w->kid);
		window->child = play_game_w; //go back to game!
	}
}
void activate_button_promotion_w(button *but)
{
	switch (but->name)
	{
		case ('m'): 
			back_to_default(); //main menu button
			break;
		case ('Q'): // choose queen
			gui_promote_to = WHITE_Q;
			break;
		case ('R'): // choose rook
			gui_promote_to = WHITE_R;
			break;
		case ('N'): // choose knight
			gui_promote_to = WHITE_N;
			break;		
		case ('B'): // choose bishop
			gui_promote_to = WHITE_B;
			break;		
	}
	if ( but->name != 'm' )
	{
		but->highlight = 0;
		refresh_button(promotion_w, but);
		promotion_w->kid->highlight = 1;
		refresh_button(promotion_w, promotion_w->kid);
		window->child = play_game_w; //go back to game!
		move *real_move = create_move(gui_board_from[1],gui_board_from[0],gui_board_to[1],gui_board_to[0]);
		real_move->promote = WHITE_TURN ? gui_promote_to : gui_promote_to - ('a' - 'A');
		do_move_gui(real_move);
		free_move(real_move);
	}
}
void activate_button_choose_depth_w(button *but)
{
	int real_depth = MINIMAX_DEPTH;
	switch (but->name)
	{
		case ('m'): 
		back_to_default(); 
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
			MINIMAX_DEPTH = BEST_DEPTH_VALUE;
			break;
	}
	if ( but->name != 'm' )
	{
		but->highlight = 0;
		refresh_button(choose_depth_w, but);
		choose_depth_w->kid->highlight = 1;
		refresh_button(choose_depth_w, choose_depth_w->kid);
		window->child = play_game_w; //go back to game!
		apply_surface(0, 0, window->child->sr, window->sr);
		if (SDL_Flip(window->sr) != 0) 
		{
			printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());	
		}
		move *best_move = get_rand_move_minmax();
		gui_board_game[best_move->from->column][best_move->from->row]->special = 1;
		refresh_button(play_game_w, gui_board_game[best_move->from->column][best_move->from->row]);
		gui_board_game[best_move->to->column][best_move->to->row]->special = 1;
		refresh_button(play_game_w, gui_board_game[best_move->to->column][best_move->to->row]);
		free_move(best_move);
		MINIMAX_DEPTH = real_depth;
	}
}
void activate_button_declare_winner_w(button *but)
{
	
	switch(but->name)
	{
		case ('m'): 
			window->child->kid->highlight = 1;
			refresh_button(window->child, window->child->kid);
			back_to_default();
			break;
		case ('B'):
			but->highlight = 0;
			refresh_button(window->child, but);
			window->child->kid->highlight = 1;
			refresh_button(window->child, window->child->kid);
			window->child = play_game_w;
			break;

	}
	
}
void activate_button_play_game_w(button *but)
{
	if(is_best_move) 
	{
		toggle_gui_board_active(1, 1, gui_board_game);
		is_best_move = 0;
	}
	location l;
	l.column = but->column;
	l.row = but->row;
	move *opt_move = NULL; 
	move *tmp_move = NULL;
	if(but->board_button)
	{
		if ( gui_board_from[0]==-1 ) //this is the first location in the move
		{ 
			
			if ( (WHITE_TURN && !IS_WHITE(board[l.column][l.row])) || 
				 (!WHITE_TURN && !IS_BLACK(board[l.column][l.row])) )
			{
				but->special = 0;
				return; 
			}
			else
			{
				but->special = 1;
				gui_board_from[0] = but->column;
				gui_board_from[1] = but->row;
				refresh_button(play_game_w, but);
				opt_move = get_piece_moves(board,&l);
				tmp_move = opt_move;
				while(tmp_move != NULL ) 
				{
					gui_board_game[tmp_move->to->column][tmp_move->to->row]->special = 1;
					refresh_button(play_game_w, gui_board_game[tmp_move->to->column][tmp_move->to->row]);
					tmp_move = tmp_move->next;
				}
				free_move(opt_move);
			}
		}
		else //this is the second location in the move
		{
			move *real_move = NULL;
			location from;
			from.column = gui_board_from[0];
			from.row = gui_board_from[1];
			if (from.row == but->row && from.column == but->column)
			{
				but->special = 0;
				gui_board_from[0] = -1;
				gui_board_from[1] = -1;
				refresh_button(play_game_w, but);
				opt_move = get_piece_moves(board,&l);
				tmp_move = opt_move;
				while(tmp_move != NULL ) 
				{
					gui_board_game[tmp_move->to->column][tmp_move->to->row]->special = 0;
					refresh_button(play_game_w, gui_board_game[tmp_move->to->column][tmp_move->to->row]);
					tmp_move = tmp_move->next;
				}
				free_move(opt_move);
				return;
			}
			real_move = create_move(from.row, from.column, but->row, but->column);
			opt_move = get_piece_moves(board,&from);
			if ( is_legal_move(real_move) )
			{
				tmp_move = opt_move;
				
				while(tmp_move != NULL ) 
				{
					gui_board_game[tmp_move->to->column][tmp_move->to->row]->special = 0;
					refresh_button(play_game_w, gui_board_game[tmp_move->to->column][tmp_move->to->row]);
					tmp_move = tmp_move->next;
				}
				if ( ((board[from.column][from.row] == WHITE_P && real_move->to->row == BOARD_SIZE-1) //promotion is needed
					|| (board[from.column][from.row] == BLACK_P && real_move->to->row == 0)) )
				{
 					but->highlight = 0;
					refresh_button(play_game_w, but);
					play_game_w->kid->highlight = 1;
					refresh_button(play_game_w, play_game_w->kid);
					gui_board_to[0]	= but->column;
					gui_board_to[1]	= but->row;
					window->child = promotion_w;
					free_move(real_move);
					free_move(opt_move);
					return;
					
 				}
				do_move_gui(real_move);
			}
			else //selected an illegal location or another piece of the same color (need to restart special etc.)
			{
				location old_from;
				old_from.column = gui_board_from[0];
				old_from.row = gui_board_from[1];
				gui_board_game[old_from.column][old_from.row]->special = 0;
				refresh_button(play_game_w, gui_board_game[old_from.column][old_from.row]);
				tmp_move = opt_move;
				while(tmp_move != NULL ) 
				{
					gui_board_game[tmp_move->to->column][tmp_move->to->row]->special = 0;
					refresh_button(play_game_w, gui_board_game[tmp_move->to->column][tmp_move->to->row]);
					tmp_move = tmp_move->next;
				}
				gui_board_from[0] = -1;
				gui_board_from[1] = -1;
				activate_button_play_game_w(but); //return to first location of the move
			}
			free_move(opt_move);
			free_move(real_move);
		}
	}
	else
	{
		switch(but->name)
		{
			case ('m'): // back to main menu
				back_to_default();
				break;
			case ('s'):
				but->highlight = 0;
				refresh_button(play_game_w, but);
				play_game_w->kid->highlight = 1;
				refresh_button(play_game_w, play_game_w->kid);
				window->child = save_game_w; 
				break;
			case ('B'):
				toggle_gui_board_active(1, 1, gui_board_game);
				is_best_move = 1;
				if (!TWO_PLAYERS_MODE)
				{
					move *best_move = get_rand_move_minmax();
					gui_board_game[best_move->from->column][best_move->from->row]->special = 1;
					refresh_button(play_game_w, gui_board_game[best_move->from->column][best_move->from->row]);
					gui_board_game[best_move->to->column][best_move->to->row]->special = 1;
					refresh_button(play_game_w, gui_board_game[best_move->to->column][best_move->to->row]);
					free_move(best_move);
				}
				else 
				{
					but->highlight = 0;
					refresh_button(play_game_w, but);
					play_game_w->kid->highlight = 1;
					refresh_button(play_game_w, play_game_w->kid);
					window->child = choose_depth_w;
				}
				break;
			case ('W'):
				toggle_gui_board_active(1, 1, gui_board_game);
				is_best_move = 1;
				if (!TWO_PLAYERS_MODE)
				{
					move *best_move = get_rand_move_minmax();
					gui_board_game[best_move->from->column][best_move->from->row]->special = 1;
					refresh_button(play_game_w, gui_board_game[best_move->from->column][best_move->from->row]);
					gui_board_game[best_move->to->column][best_move->to->row]->special = 1;
					refresh_button(play_game_w, gui_board_game[best_move->to->column][best_move->to->row]);
					free_move(best_move);
				}
				else 
				{
					but->highlight = 0;
					refresh_button(play_game_w, but);
					play_game_w->kid->highlight = 1;
					refresh_button(play_game_w, play_game_w->kid);
					window->child = choose_depth_w;
				}
				break;
		}
	}
}
//---------------------------------------------------------------------------------//
	/* ----Main function----*/
	
int play_gui(void) {
	
	if ( init() ) return 1;
	

	SDL_Event event;
	init_gui_board(gui_board_set);
	init_gui_board(gui_board_game);
	welcome_to_chess_w =  init_welcome();
	main_w = init_main();
	settings_w = init_settings();
	set_diff_w = init_set_diff();
	change_board_w = init_change_board();
	load_game_w = init_load_game();
	save_game_w = init_save_game();
	promotion_w = init_promotion();
	play_game_w = init_play_game();
	choose_depth_w = init_choose_depth();
	declare_winner_w = init_declare_winner();

	/*welcome to chess!!! */
	window->child = welcome_to_chess_w;
	apply_surface(window->child->rct.x, window->child->rct.y, window->child->sr, window->sr);
	/* Wait a little before redrawing*/
	if (SDL_Flip(window->sr) != 0) {
			printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
	}
	SDL_Delay(2000);
	
	window->child = main_w; 
	int quit = 0;
	
	apply_surface(window->child->rct.x, window->child->rct.y, window->child->sr, window->sr);
	
	button *tmp = NULL;
	button *curr = window->child->kid;
	curr->highlight = 1;
	refresh_button(window->child, curr);
	apply_surface(0, 0, window->child->sr, window->sr);
	int w_id = window->child->id;
	while (!quit) {
		if ( window->child->id != w_id )
		{
			if ( window->child->id == PLAY_GAME_W  && (w_id != DECLARE_WINNER_W && w_id != SAVE_GAME_W) )
			{
				if ( (GAME_STATUS = game_over()) ) declare_winner_gui();
				else if (!TWO_PLAYERS_MODE && ((WHITE_TURN && !PLAYER_WHITE) || (!WHITE_TURN && PLAYER_WHITE))) // the computer turn
				{
					apply_surface(0, 0, window->child->sr, window->sr);
					if (SDL_Flip(window->sr) != 0) 
					{
						printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());	
					}
					play_computer_turn();
					toggle_gui_board_active(1, 0, gui_board_game);
					if ( (GAME_STATUS = game_over()) ) declare_winner_gui(); // check if the game is over after the move.
					else switch_turn_gui();// the game is not over, player turn.
				}
				
			}
			curr = window->child->kid;
			w_id = window->child->id;
		}
		apply_surface(0, 0, window->child->sr, window->sr);
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


	}
	free_gui();
	return 0;
}


