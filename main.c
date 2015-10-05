#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Chess.h"


// Genral settings
int MINIMAX_DEPTH = 1;
int GAME = 0;
int SETTINGS = 1;
int GAME_STATUS = 0; // '0' - ongoing game, '1' - tie, '2'-current player wins '3'-current pleyer loose.
int GUI_MODE = 0; // '0' - application runs in 'Console mode', '1' - 'Gui mode'
int TWO_PLAYERS_MODE = 1; // '1' - two players mode, '2'(0) - player vs. AI mode
int WHITE_TURN = 1; // 0 - black, 1- white
int PLAYER_WHITE = 1; //???
char board[BOARD_SIZE][BOARD_SIZE]; // the Board 

// Game setiings
int WHITE_CR_ENABLE = 1; // ??? what to do in case the game is loaded?
int BLACK_CR_ENABLE = 1;
int WHITE_CL_ENABLE = 1;
int BLACK_CL_ENABLE = 1;

int CHECK_ON_WHITE = 0;
int CHECK_ON_BLACK = 0;




void test1()
{
	char board[BOARD_SIZE][BOARD_SIZE];
	init_board( board );
	print_board( board );

}


/** the main function. */
int main( int argc, char** argv )
{
	int repeat = 0;
	char input[BUFF_SIZE] = "\n"; ///??? right initialization? 
	init_board(board);
	if ( argc == 2 ) // more than 1 argument
	{
		if ( strcmp(argv[1], "gui") == 0 ) GUI_MODE = 1;
	}
	if ( !GUI_MODE ) print_board(board); //prints only if in console mode

	while( !GUI_MODE ) //play console mode
	{
		if ( (SETTINGS) || (TWO_PLAYERS_MODE) || (WHITE_TURN&&PLAYER_WHITE) || ((!WHITE_TURN)&&(!PLAYER_WHITE)) )  // ??? change conditions LLLLATERR
		{
			if ( GAME && (GAME_STATUS = game_over()) ) GAME = 0; // ??? 
			if( !GAME && !SETTINGS ) // end the game
			{  
				declare_winner();
				quit();
			}
			if ( GAME )	print_message(ENTER_MOVE(WHITE_TURN)); //???
			if ( SETTINGS ) print_message(ENTER_SETTINGS);
			read_input(input);
			if( strcmp(input,"") == 0 ) continue; // verify input isn't empty.
			if ( strcmp(input, "quit") == 0 ) quit();
		}
		if ( SETTINGS )
		{
			parse_input_settings(input);
			DO_DEBUG
			(
				printf("user color is: %d\n", PLAYER_WHITE);
				printf("minmax depth is: %d\n", MINIMAX_DEPTH);
				printf("next player is: %d\n", WHITE_TURN);
				printf("game mode is: %d\n", TWO_PLAYERS_MODE);
			)
		}
		else if ( GAME )
		{
			
			if ( (TWO_PLAYERS_MODE) || (WHITE_TURN&&PLAYER_WHITE) || ((!WHITE_TURN)&&(!PLAYER_WHITE)) ) //user's turn 
			{
				if( (repeat = parse_input_game(input)) == 1 ) //'1' if user's input was wrong in some way or need another input 
					WHITE_TURN = (WHITE_TURN + 1)%2; 
			}
			else play_computer_turn(); //computer's turn 

			WHITE_TURN = (WHITE_TURN + 1)%2;
			if( !repeat ) print_board(board);
			
			
		}
		
	}
	play_gui();
	return 0;
}
