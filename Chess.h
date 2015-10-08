#ifndef CHESS_
#define CHESS_

#include<stdio.h>
#include <SDL.h>
#include <SDL_video.h>

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

#define WIN_SCORE 200
#define LOOSE_SCORE -200
#define TIE_SCORE -199
#define INF_SCORE 201 //naive infinity :)
#define EMPTY ' '

#define BOARD_SIZE 8

#define IS_WHITE(x) (((x) >= ('a')) && ((x) <= ('z')))
#define IS_BLACK(x) (((x) >= ('A')) && ((x) <= ('Z')))
#define IS_SAME_COLOR(x,y) (((IS_WHITE(x)) && (IS_WHITE(y))) || ((IS_BLACK(x)) && (IS_BLACK(y)))) // x not EMPTY or y not EMPTY
#define IS_KING(x) (((x) == (WHITE_K))||((x) == (BLACK_K)))


#define BUFF_SIZE 52
#define BEST_DEPTH_VALUE -1
#define MAX_DEPTH_VALUE 10
#define P_FIRST_MOVE 1
#define DO_P_FIRST_MOVE(x) if(P_FIRST_MOVE){x}


#define DEBUG 0
#define DEBUG2 0
#define DEBUG3 0
#define DO_DEBUG(x) if(DEBUG){x}
#define DO_DEBUG2(x) if(DEBUG2){x}
#define DO_DEBUG3(x) if(DEBUG3){x}

#define PRINTD(x) if(DEBUG == 1) {printf("%s", x); fflush(stdout);}

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
#define TOO_MUCH 3500000
#define ENTER_MOVE(x) (((x) == 1) ? "White player - enter your move:\n" : "Black player - enter your move:\n")
#define NOT_YOUR_PIECE "The specified position does not contain your piece\n"
#define ILLEGAL_COMMAND "Illegal command, please try again\n"
#define ILLEGAL_MOVE "Illegal move\n"

#define DECLARE_TIE "The game ends in a tie\n"
#define DECLARE_WINNER(x) (((x) == 1) ? "Mate! White player wins the game\n":"Mate! Black player wins the game\n" )
#define CHECK "Check!\n"

#define WRONG_ROOK_POSITION "Wrong position for a rook\n" 
#define ILLEGAL_CALTLING_MOVE "Illegal castling move\n"  

#define TIE "The game ends in a tie\n"
 
#define perror_message(func_name) (perror("Error: standard function %s has failed", func_name));
#define print_message(message) (printf("%s", message));

	//----infrastructure----//

struct location_st   //define a location 
{
  int column;	//0-7 (a-h)
  int row;	//0-7 (1-8)
};
typedef struct location_st location;
struct move_st   //define a move
{
  location *from;
  location *to; //will be a linked list of steps within this move	
  struct move_st *next;	//linked list of moves
  char promote;
};
typedef struct move_st move;
struct widget_st;
typedef struct widget_st widget;
struct button_st;
typedef struct button_st button;




	//----console functions----//
	
void print_board( char board[BOARD_SIZE][BOARD_SIZE] );
void init_board( char board[BOARD_SIZE][BOARD_SIZE] );
void print_line( void );
void read_input( char block[BUFF_SIZE] );
void quit( void );
location *create_location(int row, int column);
move *create_move(int from_row, int from_column, int to_row, int to_column);
void free_location(location *l);
void free_move(move *m);
int check_settings( void );
void set_minmax_depth( int );
void set_user_color( char *s );
void clear(void);
void remove_location(location l);
void set_location( location l, int white, char piece );
location str_to_location( char* locus );
void parse_input_settings( char input[BUFF_SIZE] );
char to_piece( char *word );
int is_legal_placement( location l, char piece );
int is_legal_location( location l );
int is_legal_move(move* m);
void start_game(void);
int parse_input_game( char input[BUFF_SIZE] );
void play_computer_turn(void);
int game_over(void);
void declare_winner(void);
move *get_moves(char a_board[BOARD_SIZE][BOARD_SIZE], int white_turn);
move *get_piece_moves(char a_board[BOARD_SIZE][BOARD_SIZE], location *from);
move *get_p_moves(char a_board[BOARD_SIZE][BOARD_SIZE], location *from);
move *get_b_moves(char a_board[BOARD_SIZE][BOARD_SIZE], location *from);
move *get_n_moves(char a_board[BOARD_SIZE][BOARD_SIZE], location *from);
move *get_r_moves(char a_board[BOARD_SIZE][BOARD_SIZE], location *from);
move *get_q_moves(char a_board[BOARD_SIZE][BOARD_SIZE], location *from);
move *get_k_moves(char a_board[BOARD_SIZE][BOARD_SIZE], location *from);
move *link_moves(move *m1, move *m2);
void do_move(char a_board[BOARD_SIZE][BOARD_SIZE], move *user_move);
int is_check(char a_board[BOARD_SIZE][BOARD_SIZE], int color);
char piece_name_to_char(move *m, char *name);
void print_all_moves(move *m);
void print_move(move *m);
int score_board(char a_board[BOARD_SIZE][BOARD_SIZE], int white_player);
int minmax(char a_board[BOARD_SIZE][BOARD_SIZE], int minim, int next_color, int depth, int alpha, int beta);
move *get_move_minmax(void);
move *get_rand_move_minmax(void);
int get_best_minmax_depth(void);










	//----XML functions----//
	
int load_xml(char *file_pth);
int parse_line(char *line, int cnt);
void load_row(char *row_str, int row_num);
int  save_xml(const char *file_name);
char *str_row(char row_buff[9], int row);


	//----GUI functions----//
	
int play_gui(void);
void quit_allocation_error(void);
SDL_Surface *load_image( char *filename, int transper );
widget *build_window(void);
widget *build_panel(int x, int y, int size_w, int size_h, char *filename, int id);
button *build_button(int x, int y, int size_w, int size_h, SDL_Surface *sr, SDL_Surface *sr2, SDL_Surface *sr3,
					 SDL_Surface *sr4, SDL_Surface *sr5, char name, int is_board);
void apply_surface( int x, int y, SDL_Surface *source, SDL_Surface *destination );
int init(void);
void refresh_button(widget *panel, button *butt);
widget *init_main(void);
int activate_buttons_function(button *but);
int activate_button_main_w(button *but);
void activate_button_settings_w(button *but);
void activate_button_set_diff_w(button *but);
void activate_button_change_board_w(button *but);
void activate_button_load_game_w(button *but);
void activate_button_save_game_w(button *but);
void activate_button_promotion_w(button *but);
void activate_button_play_game_w(button *but);
void activate_button_choose_depth_w(button *but);
void activate_button_declare_winner_w(button *but);
SDL_Surface *get_piece_sr(char piece);
void load_board_to_screen(widget *panel, button *gui_board[BOARD_SIZE][BOARD_SIZE]);
widget *init_change_board(void);
widget *init_load_game(void);
widget *init_save_game(void);
widget *init_promotion(void);
widget *init_choose_depth(void);
widget *init_declare_winner(void);
void toggle_gui_board_active(int on_off, int clean_selected, button *gui_board[BOARD_SIZE][BOARD_SIZE]);
void switch_turn_gui(void);
void declare_winner_gui(void);
void back_to_default(void);
void free_button(button *but);
void free_widget(widget *panel);
void free_gui(void);





#endif

