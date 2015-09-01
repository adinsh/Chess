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

/** 'file' is a relative\full path to a xml file.
  * the funciton returns '1' if 'file' does not exists,
  * and '0' if no problem occured. the function loads the settings 
  * from 'file' and sets the relevant values. */  

  
int load_xml(char *file_pth)
{
	FILE *f_in;
	char line[100];
	int cnt = 0;
	f_in = fopen(file_pth, "r");
	
	if ( f_in == NULL ) return 1; // file doesn't exists
	
	while ( fgets(line, 100, f_in) != NULL )
	{
		if ( (strlen(line)>0) && (line[strlen(line) - 1] == '\n') )
			line[strlen (line) - 1] = '\0';
		if ( !parse_line(line, cnt) ) break;
		cnt++;
	}
	fclose(f_in);//???
	return 0;
}

/** 'file' is a relative\full path to a xml file. ???
  * the funciton returns '1' if 'file' does not exists,
  * and '0' if no problem occured. the function loads the settings 
  * from 'file' and sets the relevant values. */  

int parse_line(char *line, int cnt)
{
	char words[BUFF_SIZE]; // will be a copy of the input.
	char *word;
	strcpy(words, line); 
	word = strtok(words, " \t<>");
	
	if ( cnt < 2 ) return 1;
	
	if ( strcmp(word, "/game") == 0 ) return 0;
	else if ( strcmp(word, "next_turn") == 0 )
	{
		word = strtok(NULL, " \t<>"); // word is either white or black or '/next_turn'
		if ( strcmp(word, "/next_turn") == 0 ) return 1;
		for ( int i = 0; word[i]; i++ ) word[i] = tolower(word[i]); // lower case the word
		set_user_color(word);
		return 1;
	}
	else if ( strcmp(word, "game_mode") == 0 )
	{
		word = strtok(NULL, " \t<>"); // word is either white or black
		if ( strcmp(word, "/game_mode") == 0 ) return 1;
		TWO_PLAYERS_MODE =  (atoi(word) == 2) ? 0 : 1;
		return 1;
	}
	else if ( strcmp(word, "difficulty") == 0 )
	{
		word = strtok(NULL, " \t<>"); // word is either white or black or '/next_turn'
		if ( strcmp(word, "/difficulty") == 0 ) return 1;
		if ( strcmp(word, "best") == 0 )
		{
			MINIMAX_DEPTH = -1;
			return 1;
		}
		int x = atoi(word); // returns '0' if not valid
		set_minmax_depth(x);
		return 1;			
	}
	else if ( strcmp(word, "user_color") == 0 )
	{
		word = strtok(NULL, " \t<>"); // word is either white or black or '/next_turn'
		if ( strcmp(word, "/user_color") == 0 ) return 1;
		set_user_color(word);
		return 1;			
	}
	else if ( strcmp(word, "board") == 0 ) return 1; // word is either white or black or '/next_turn'	
	else if ( strcmp(word, "row_8") == 0 ) 
	{
		load_row(strtok(NULL, " \t<>"), 7); // send the row to 'load_row'
		return 1;			
	}	
	else if ( strcmp(word, "row_7") == 0 )
	{
		load_row(strtok(NULL, " \t<>"), 6);
		return 1;			
	}	
	else if ( strcmp(word, "row_6") == 0 )
	{
		load_row(strtok(NULL, " \t<>"), 5);
		return 1;			
	}	
	else if ( strcmp(word, "row_5") == 0 )
	{
		load_row(strtok(NULL, " \t<>"), 4);
		return 1;			
	}	
	else if ( strcmp(word, "row_4") == 0 )
	{
		load_row(strtok(NULL, " \t<>"), 3);
		return 1;			
	}	
	else if ( strcmp(word, "row_3") == 0 )
	{
		load_row(strtok(NULL, " \t<>"), 2);
		return 1;			
	}	
	else if ( strcmp(word, "row_2") == 0 )
	{
		load_row(strtok(NULL, " \t<>"), 1);
		return 1;			
	}	
	else if ( strcmp(word, "row_1") == 0 )
	{
		load_row(strtok(NULL, " \t<>"), 0);
		return 1;			
	}	
	else if ( strcmp(word, "/board") == 0 ) return 1;
	else return 2;
}

void load_row(char *row_str, int row_num)
{
	for ( int column = 0; column < BOARD_SIZE; column++ )
	{
		board[column][row_num] = row_str[column] == '_' ? EMPTY:row_str[column];
	}
}




		