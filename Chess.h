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


#define EMPTY ' '

#define BOARD_SIZE 8

#define IS_WHITE(x) (((x) >= ('a')) && ((x) <= ('z')))
#define IS_BLACK(x) (((x) >= ('A')) && ((x) <= ('Z')))
#define IS_KING(x) (((x) == (WHITE_K))||((x) == (BLACK_K)))

#define BUFF_SIZE 52

#define DEBUG 1
#define DO_DEBUG(x,y) (DEBUG == 1) ? (x) : (y)
#define PRINTD(x) (DEBUG == 1) ? printf("%s\n", x) : NULL
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

#define ENTER_MOVE(x) (((x) == 1) ? "white player - enter your move:\n" : "black player - enter your move:\n")
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
struct widget_st;
typedef struct widget_st widget;
struct button_st;
typedef struct button_st button;

// console functions
void print_board( char board[BOARD_SIZE][BOARD_SIZE] );
void init_board( char board[BOARD_SIZE][BOARD_SIZE] );
void print_line( void );
void read_input( char block[BUFF_SIZE] );
void quit( void );
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
void start_game(void);
int parse_input_game( char input[BUFF_SIZE] );
void play_computer_turn(void);
int game_over(void);
void declare_winner(void);

// XML functions
int load_xml(char *file_pth);
int parse_line(char *line, int cnt);
void load_row(char *row_str, int row_num);
int  save_xml(const char *file_name);
char *str_row(char row_buff[9], int row);


//GUI functions
int play_gui(void);
void quit_allocation_error(void);
SDL_Surface *load_image( char *filename );
widget *build_window(void);
widget *build_panel(int x, int y, int size_w, int size_h, char *filename, int id);
button *build_button(int x, int y, int size_w, int size_h, char *filename, char *filename2, char *filename3, char name, int is_board); 
void apply_surface( int x, int y, SDL_Surface *source, SDL_Surface *destination );
int init(void);
void refresh_button(widget *panel, button *butt);
widget *init_main(void);
int do_job(button *but);




#endif