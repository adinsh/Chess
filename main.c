#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Chess.h"

//						 *************** Board init & print methods ****************

void print_line(){
	int i;
	printf("  |");
	for (i = 1; i < BOARD_SIZE*4; i++){
		printf("-");
	}
	printf("|\n");
}

void print_board(char board[BOARD_SIZE][BOARD_SIZE]) {
	int column,row;
	print_line();
	for (row = BOARD_SIZE-1; row >= 0 ; row--)
	{
		printf((row < 9 ? " %d" : "%d"), row+1);
		for (column = 0; column < BOARD_SIZE; column++){
			printf("| %c ", board[column][row]);
		}
		printf("|\n");
		print_line();
	}
	printf("   ");
	for (column = 0; column < BOARD_SIZE; column++){
		printf(" %c  ", (char)('a' + column));
	}
	printf("\n");
}


void init_board(char board[BOARD_SIZE][BOARD_SIZE])
{
	int column,row;
	char tmp;
	int transform; // delta from 'white' to 'black'
	for (row = 0; row < BOARD_SIZE; row++)
	{
		for (column = 0; column < BOARD_SIZE; column++)
		{
            tmp = EMPTY;
            transform = 'a'-'A';
			if ( row == 0 || row == 7)
			{
				if ( row == 0 ) transform = 0; // don't transform to black
				switch( column )
				{
					case 0:
						tmp = WHITE_R - transform;
						break;
					case 1:
						tmp = WHITE_N - transform;
						break;
					case 2:
						tmp = WHITE_B - transform;
						break;
					case 3:
						tmp = WHITE_Q - transform;
						break;
					case 4:
						tmp = WHITE_K - transform;
						break;
					case 5:
						tmp = WHITE_B - transform;
						break;
					case 6:
						tmp = WHITE_N - transform;
						break;
					case 7:
						tmp = WHITE_R - transform;
						break;
				}
			}
			if ( row == 1 || row == 6 ) // pawns rows
			{
				if ( row == 1 ) transform = 0;
				tmp = WHITE_P - transform;
			}

			board[column][row] = tmp;
		}
	}
}

void test1()
{
	char board[BOARD_SIZE][BOARD_SIZE];
	init_board( board );
	print_board( board );

}


/** the main function. */
int main( int argc, char** argv )
{
	int gui_mode = 0; // '0' - application runs in 'Console mode', '1' - 'Gui mode'

	if ( argc == 2 ) // more than 1 argument
	{
		if ( strcmp(argv[1], "gui") == 0 ) gui_mode = 1;
	}
	if ( !gui_mode ) test1();
	return 0;
}
