#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "Chess.h"

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


//						*************** Infrastructure ***************

struct location_st   //define a location 
{
  int column;	//0-7 (a-h)
  int row;	//0-7 (1-8)
  //struct location_st *next; //linked list implementation???
};

struct move_st   //define a move
{
  int what_eat; //???what have you eaten on the way
  location *step; //will be a linked list of steps within this move	
  struct move_st *next;	//linked list of moves
};


//						*************** Board init & print methods ****************

void print_line(){
	int i;
	printf("  |");
	for ( i = 1; i < BOARD_SIZE*4; i++ ){
		printf("-");
	}
	printf("|\n");
}

void print_board( char board[BOARD_SIZE][BOARD_SIZE] )
{
	int column,row;
	print_line();
	for (row = BOARD_SIZE-1; row >= 0 ; row--)
	{
		printf((row < 9 ? " %d" : "%d"), row+1);
		for ( column = 0; column < BOARD_SIZE; column++ )
		{
			printf("| %c ", board[column][row]);
		}
		printf("|\n");
		print_line();
	}
	printf("   ");
	for ( column = 0; column < BOARD_SIZE; column++ )
	{
		printf(" %c  ", (char)('a' + column));
	}
	printf("\n");
}


void init_board( char board[BOARD_SIZE][BOARD_SIZE] )
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

//						*************** General methods ****************

void quit( void )
{
  exit(0);
}

/** checks if l is valid location in the board
  * returns '1' if l is a legal location
  * else, returns '0'. */
int is_legal_location( location l )
{ 
  if ( l.row > BOARD_SIZE - 1  ||l.row < 0 ||\
       l.column > BOARD_SIZE - 1  || l.column < 0) return 0; //location is outside the board
  return 1;	//this location is legal (black and on the board)
}

//						*************** Settings methods ****************

// checks if 'piece' can be set at location 'l'. returns '1' if True, else '0'.
int is_legal_placement( location l, char piece )
{
	int count = 0;
	int is_bishop = ((piece == WHITE_B) || (piece == BLACK_B)) ? 1 : 0;
	int max_allowed; 

	//check for allowed amount this piece
	switch( tolower(piece) )
	{
		case WHITE_P:
			max_allowed = 8;
			break;
		case WHITE_B:
			max_allowed = 2;
			break;
		case WHITE_N:
			max_allowed = 2;
			break;
		case WHITE_R:
			max_allowed = 2;
			break;
		case WHITE_Q:
			max_allowed = 1;
			break;
		case WHITE_K:
			max_allowed = 1;
			break;
	}
	// count and check that the amount of this piece is legal after placement	
	for ( int i = 0; i < BOARD_SIZE; i++ )
	{
		for ( int j = 0; j < BOARD_SIZE; j++ )
		{
			if ( l.row == i && l.column == j ) continue;
			if ( board[j][i] == piece )
			{
				count++;
				if ( count >= max_allowed ) return 0; 
				if ( is_bishop && ((l.row + l.column) % 2 == (i + j) % 2 ) ) return 0;
			}
		}
	}
	return 1;
}

// translates a 'word' to it's relevant char 'piece'
char to_piece( char *word )
{
	if ( strcmp(word, "king") == 0 ) return WHITE_K;
	if ( strcmp(word, "queen") == 0 ) return WHITE_Q;
	if ( strcmp(word, "rook") == 0 ) return WHITE_R;
	if ( strcmp(word, "bishop") == 0 ) return WHITE_B;
	if ( strcmp(word, "knight") == 0 ) return WHITE_N;
	return WHITE_P;
}

/** returns '1' if the board set-up is legal.
  * '0' otherwise. */
int check_settings( void )
{
	int kings_cnt = 0;
	for ( int i = 0; i < BOARD_SIZE; i++ )
	{
		for ( int j = 0; j < BOARD_SIZE; j++ )
		{
			if (tolower(board[i][j]) == WHITE_K) kings_cnt++;
		}
	}
	if ( kings_cnt != 2 ) return 0;
	return 1;
}
			

//						*************** input methods ****************

void read_input( char block[BUFF_SIZE] )
{
	if ( GUI_MODE ) printf("hello, my name is Cheesprog and I'm in GUI_MODE!\n");// ???
    fgets (block, BUFF_SIZE, stdin);
    if ( (strlen(block)>0) && (block[strlen(block) - 1] == '\n') )
        block[strlen (block) - 1] = '\0';
}

/** receives 'input'.
  * if input contains valid, setting phase, instructions;
  * executes them. 
  * otherwise in nessecary, prints ILLEGAL_COMMAND. */
void parse_input_settings( char input[BUFF_SIZE] )
{
	char words[BUFF_SIZE]; // will be a copy of the input.
	char *word;
	strcpy(words, input); 
	word = strtok(words, " ");
	
	// check if 'word' matches a legal (settings) command: 
	if ( strcmp(word, "difficulty") == 0 && !TWO_PLAYERS_MODE )
	{
		word = strtok(NULL, " "); //next word 
		if ( strcmp(word, "best") == 0 ) MINIMAX_DEPTH = -1;
		else if ( strcmp(word, "depth") == 0 )
		{
			word = strtok(NULL, " "); //next word 
			int x = atoi(word); // returns '0' if not valid
			set_minmax_depth(x);		
		}
	}
	else if ( strcmp(word, "user_color") == 0 && !TWO_PLAYERS_MODE )
	{
		word = strtok(NULL, " ");
		set_user_color(word);
	}
	else if ( strcmp(word, "clear") == 0 )
	{ 
		clear();
	}
	else if ( strcmp(word, "rm") == 0 )
	{
		word = strtok(NULL, " ");
		location l = str_to_location(word);
		remove_location(l);	
	}
	else if ( strcmp(word, "set") == 0)
	{
		int is_white = 1;		
		word = strtok(NULL, " ");
		location l = str_to_location(word); 
		word = strtok(NULL, " ");
		if ( strcmp(word, "black") == 0 ) is_white = 0;
		word = strtok(NULL, " ");
		set_location( l, is_white, to_piece(word) );	
	}
	else if ( strcmp(word, "game_mode") == 0 )
	{
		word = strtok(NULL, " ");
		if ( atoi(word) != 1 && atoi(word) != 2 )
		{
			print_message(WRONG_GAME_MODE);
			return;
		}
		TWO_PLAYERS_MODE =  (atoi(word) == 2) ? 0 : 1;
		if ( TWO_PLAYERS_MODE )
		{
			print_message(TWO_PLAYERS_GAME_MODE);
		}
		else print_message(PLAYER_VS_AI_GAME_MODE);
	}
	else if ( strcmp(word, "next_player") == 0)
	{
		word = strtok(NULL, " ");
		if ( strcmp(word, "black") == 0 ) WHITE_TURN = 0;
		if ( strcmp(word, "white") == 0 ) WHITE_TURN = 1;
	}
	else if ( strcmp(word, "print") == 0) print_board(board);
	else if ( strcmp(word, "load") == 0)
	{
		if ( load_xml(strtok(NULL, " ")) ) print_message(WRONG_FILE_NAME)
		else print_board(board);
	}
	else if ( strcmp(word, "start") == 0 )
	{
		if ( check_settings() ) // if the game is a "game over" game we need to decide wtftd ??? 
		{
			start_game();
		}
		else print_message(WROND_BOARD_INITIALIZATION);
	}
	// 'word' doesn't match any legal (settings) command 
	else print_message(ILLEGAL_COMMAND);
}

/** converts locus into a location. */
location str_to_location( char* locus )
{
	location l;
	int column = locus[1] - 'a'; // convert the row to integer
	int row = 0;
	for ( int i = 3 ; locus[i] != '>' ; i++ )
	{
		row = 10 * row + (locus[i] - '0'); // calculate column value (could be '10')
	}
	l.column = column; 
	l.row = row-1; // normalize to range starting from 0
	return l;
}


//						*************** setters methods ***************

// set the minimax_depth
void set_minmax_depth( int x ) 
{ 
  if ( x > 0 && x < 5 ) MINIMAX_DEPTH = x; 
  else print_message(WRONG_MINIMAX_DEPTH); // x is illegal
}

// sets user color, default is white
void set_user_color( char *s )
{  
  if ( strcmp(s, "black")== 0 ) PLAYER_WHITE = 0;
}

// clears the board
void clear(void)
{ 
  for ( int i = 0; i < BOARD_SIZE; i++ )
  {
    for ( int j = 0; j < BOARD_SIZE; j++ )
	{
		board[i][j] = EMPTY;
	}
  }
}

void start_game(void)
{
	SETTINGS = 0;
	GAME = 1;
}

// sets EMPTY to location 'l'
void remove_location(location l)
{ 
	if ( is_legal_location(l) ) board[l.column][l.row] = EMPTY;
	else print_message(WRONG_POSITION);
}

/** sets a 'piece' on the board at location 'l' if 'l' is legal.
  * 'white' is '1' if the 'piece' should be white (otherwise, black) 
  * if 'l' is an illegal location or the setting createsd an illegal board,
  * prints an appropriate message. */
void set_location( location l, int white, char piece )
{
	if ( !(white) ) piece = piece - ('a' - 'A');
	if ( is_legal_location(l) )
	{
	if ( is_legal_placement(l, piece) ) board[l.column][l.row] = piece;
	else print_message(NO_PIECE);
	}
	else print_message(WRONG_POSITION);
}


// GAME //

/** returns 'l' if current player is still playing (the turn hasn't passed).
  * else returns'0' (turn will pass to the next player).  
  * ????. */
int parse_input_settings( char input[BUFF_SIZE] )
{
	char words[BUFF_SIZE]; // will be a copy of the input.
	char *word;
	strcpy(words, input); 
	word = strtok(words, " ");
	
	// check if 'word' matches a legal (settings) command: 
	if ( strcmp(word, "save") == 0 && !TWO_PLAYERS_MODE )
	{
		if ( save_xml(strtok(NULL, " ")) ) print_message(WRONG_FILE_NAME);
		return 1; // repeat
	}
}

  
