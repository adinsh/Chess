#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Chess.h"


// Genral settings
int DEBUG = 0;
int MINIMAX_DEPTH = 1;
int GAME = 0;
int SETTINGS = 1;
int GUI_MODE = 0; // '0' - application runs in 'Console mode', '1' - 'Gui mode'
int TWO_PLAYERS_MODE = 1; // '1' - two players mode, '2' - player vs. AI mode
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
	char input[BUFF_SIZE] = "\n"; ///??? right initializion? 
	init_board(board);
	print_board(board);
	if ( argc == 2 ) // more than 1 argument
	{
		if ( strcmp(argv[1], "gui") == 0 ) GUI_MODE = 1;
	}

	while(1)
	{
		if ( 1  || (SETTINGS) ) // ??? change condtions LLLLATERR
		{
			//if (GAME) print_message(ENTER_YOUR_MOVE);
			print_message(ENTER_SETTINGS);
			read_input(input);
			if( strcmp(input,"\n") == 0 ) continue; // verify input isn't empty.
			if ( strcmp(input, "quit") == 0 ) quit();
		}
		if ( SETTINGS ) parse_input_settings(input);
		if ( DEBUG )
		{
			printf("user color is: %d\n", PLAYER_WHITE);
			printf("minmax depth is: %d\n", MINIMAX_DEPTH);
			printf("next player is: %d\n", WHITE_TURN);
			printf("game mode is: %d\n", TWO_PLAYERS_MODE);		
		}
	}
	return 0;
}
