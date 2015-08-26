#ifndef CHESS_
#define CHESS_

#include<stdio.h>

#define WHITE_P 'm'
#define WHITE_B 'b'
#define WHITE_N 'n'
#define WHITE_R 'r'
#define WHITE_Q 'q'
#define WHITE_K 'k'

#define BLACK_P 'M'
#define BLACK_B 'B'
#define BLACK_N 'N'
#define BLACK_R 'R'
#define BLACK_Q 'Q'
#define BLACK_K 'K'


#define EMPTY ' '

#define BOARD_SIZE 8

#define IS_WHITE(x) (((x) >= ('a')) && ((x) <= ('z')))
#define IS_BLACK(x) (((x) >= ('A')) && ((x) <= ('Z')))
#define IS_KING(x) (((x) == (WHITE_K))||((x) == (BLACK_K)))


// board initialization
#define ENTER_SETTINGS "Enter game settings:\n" 
#define WRONG_GAME_MODE "Wrong game mode\n"
#define TWO_PLAYERS_GAME_MODE "Running game in 2 players mode\n"
#define PLAYER_VS_AI_GAME_MODE "Running game in player vs. AI mode\n"
#define WRONG_MINIMAX_DEPTH "Wrong value for minimax depth. The value should be between 1 to 4\n"
#define WRONG_FILE_NAME "Wrong file name\n"
#define WRONG_POSITION "Invalid position on the board\n"
#define NO_PIECE "Setting this piece creates an invalid board\n"  
#define WROND_BOARD_INITIALIZATION "Wrong board initialization\n"

#define ILLEGAL_COMMAND "Illegal command, please try again\n"
#define ILLEGAL_MOVE "Illegal move\n"
 
#define WRONG_ROOK_POSITION "Wrong position for a rook\n" 
#define ILLEGAL_CALTLING_MOVE "Illegal castling move\n"  

#define TIE "The game ends in a tie\n"
 
#define perror_message(func_name) (perror("Error: standard function %s has failed", func_name));
#define print_message(message) (printf("%s", message));


struct location_st;
typedef struct location_st location;
struct move_st;
typedef struct move_st move;
 
// funtions not all nessecarry
void print_board(char board[BOARD_SIZE][BOARD_SIZE]);
void init_board(char board[BOARD_SIZE][BOARD_SIZE]);
void set_minmax_depth(int x);
void set_user_color(char *s);
void quit(void);
void clear(void);
void remove_location(location l);
int is_legal_location(location l);
void set_location(location l, int white, int man);
void quit_allcation_error(void);
void start_game(void);
char* read_input(void);
void parse_input_settings(char* input);
int check_settings(void);
location str_to_location(char* locus);
void declare_winner(void);
int minmax(char a_board[BOARD_SIZE][BOARD_SIZE], int maxi, int depth);
move* get_disc_moves(char a_board[BOARD_SIZE][BOARD_SIZE], location *l);
move* get_moves(char a_board[BOARD_SIZE][BOARD_SIZE], int is_white_turn); 
void do_move(char a_board[BOARD_SIZE][BOARD_SIZE],move* m);
move *get_eating_moves(int row ,int column, char a_board[BOARD_SIZE][BOARD_SIZE]);
move *get_move_minmax(void);
move* link_moves(move *moves, move *disc_moves);
int score_board(char a_board[BOARD_SIZE][BOARD_SIZE],int white_player);
int same_color(char a, char b);
void print_move(move *m);
void print_all_moves(move *m);
int is_legal_move(move* m);
int parse_input_game(char* input); 
//^^ funtions not all nessecarry

#endif CHESS_