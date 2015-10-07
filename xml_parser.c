#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "Chess.h"

	//----Genral settings----//
extern int MINIMAX_DEPTH;
extern int GAME;
extern int SETTINGS;
extern int GUI_MODE; // '0' - application runs in 'Console mode', '1' - 'Gui mode'
extern int TWO_PLAYERS_MODE; // '1' - two players mode, '0' - player vs. AI mode
extern int WHITE_TURN; // 0 - black, 1- white
extern int PLAYER_WHITE;
extern char board[BOARD_SIZE][BOARD_SIZE];


	//----Game settings----//
extern int CHECK_ON_WHITE;
extern int CHECK_ON_BLACK;

/** 'file' is a relative\full path to a xml file.
  * the funciton returns '1' if 'file' does not exists,
  * and '0' if no problem occured. the function loads the settings 
  * from 'file' and sets the relevant values. */  
int load_xml(char *file_pth)
{
	char full_path[70] = "load_save/";
	char *s = strchr (file_pth, '/');
	if (s == NULL) strcat(full_path, file_pth);
	else strcpy(full_path, file_pth);
	
	FILE *f_in;
	char line[100];
	int cnt = 0;
	f_in = fopen(full_path, "r");
	
	if ( f_in == NULL ) return 1; // file doesn't exists
	
	while ( fgets(line, 100, f_in) != NULL )
	{
		if ( (strlen(line)>0) && (line[strlen(line) - 1] == '\n') )
			line[strlen (line) - 1] = '\0';
		if ( !parse_line(line, cnt) ) break;
		cnt++;
	}
	fclose(f_in);
	return 0;
}

/** 'file' is a relative\full path to a xml file.
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
		if ( strcmp(word, "black") == 0 ) WHITE_TURN = 0;
		if ( strcmp(word, "white") == 0 ) WHITE_TURN = 1;
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
			MINIMAX_DEPTH = BEST_DEPTH_VALUE;
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
		for ( int i = 0; word[i]; i++ ) word[i] = tolower(word[i]); // lower case the word
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

/** updates row 'row_num' in the board according to 'row_str'. */
void load_row(char *row_str, int row_num)
{
	for ( int column = 0; column < BOARD_SIZE; column++ )
	{
		board[column][row_num] = row_str[column] == '_' ? EMPTY:row_str[column];
	}
}

/** saves the current game to specified 'file_name'
  * returns 0 upon success, 1 otherwise. */
int  save_xml(const char *file_name)
{
	char full_path[70] = "load_save/";
	char *s = strchr (file_name, '/');
	if (s == NULL) strcat(full_path, file_name);
	else strcpy(full_path, file_name);
	FILE *f_out;
	if ( (f_out = fopen(full_path, "w")) == NULL ) return 1;

	char *next_turn = WHITE_TURN == 1 ? "White" : "Black";
	char *game_mode = TWO_PLAYERS_MODE == 1 ? "1" : "2";
	char min_depth[] = {MINIMAX_DEPTH + '0', '\0'}; //MINIMAX_DEPTH to string
	char *difficulty = MINIMAX_DEPTH == BEST_DEPTH_VALUE ? "best" : min_depth;
	char *user_color = PLAYER_WHITE == 1 ? "White" : "Black";
	char row_buff[9];

	if ( TWO_PLAYERS_MODE == 1 )
	{
		user_color = "";
		difficulty = "";
	}

	fputs("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n", f_out);
	fputs("<game>\n", f_out);
	fputs("\t<next_turn>", f_out);
	fputs(next_turn, f_out); 
	fputs("</next_turn>\n", f_out);
	fputs("\t<game_mode>", f_out);
	fputs(game_mode, f_out); 
	fputs("</game_mode>\n", f_out);
	fputs("\t<difficulty>", f_out);
	fputs(difficulty, f_out); 
	fputs("</difficulty>\n", f_out);
	fputs("\t<user_color>", f_out);
	fputs(user_color, f_out); 
	fputs("</user_color>\n", f_out);
	fputs("\t<board>\n", f_out);
	fputs("\t\t<row_8>", f_out); 
	fputs(str_row(row_buff, 7), f_out);
	fputs("</row_8>\n", f_out);
	fputs("\t\t<row_7>", f_out);
	fputs(str_row(row_buff, 6), f_out);
	fputs("</row_7>\n", f_out);
	fputs("\t\t<row_6>", f_out); 
	fputs(str_row(row_buff, 5), f_out);
	fputs("</row_6>\n", f_out);
	fputs("\t\t<row_5>", f_out); 
	fputs(str_row(row_buff, 4), f_out);
	fputs("</row_5>\n", f_out);
	fputs("\t\t<row_4>", f_out); 
	fputs(str_row(row_buff, 3), f_out);
	fputs("</row_4>\n", f_out);
	fputs("\t\t<row_3>", f_out); 
	fputs(str_row(row_buff, 2), f_out);
	fputs("</row_3>\n", f_out);
	fputs("\t\t<row_2>", f_out); 
	fputs(str_row(row_buff, 1), f_out);
	fputs("</row_2>\n", f_out);
	fputs("\t\t<row_1>", f_out); 
	fputs(str_row(row_buff, 0), f_out);
	fputs("</row_1>\n", f_out);
	fputs("\t</board>\n", f_out);
	
	fputs("</game>", f_out);
	fclose(f_out);
	return 0;
}

/** translates row 'row' of the board to a "string".
  * returns 'row_buff'. */
char *str_row(char row_buff[9], int row)
{
	for ( int column = 0; column < BOARD_SIZE; column++ )
	{
		row_buff[column] = board[column][row] == EMPTY ? '_' : board[column][row]; 
	}
	row_buff[8] = '\0';
	return row_buff;
}

