typedef struct Board {
  int n;          // length of board
  int m;          // width of board
  int mine_count; // number of mines
  int flag_count; // number of flags
  int **mines;    // tracks where mines are
  int **opened;   // tracks opened, unopened, flagged squares
  int **numbered; // for displaying results
} Board;

// initializing the game board
Board *get_board_struct(int, int, int);
void generate_mines(Board *);
int **initialize_mine_board(Board *);
int **initialize_opened_board(Board *);
int **initialize_numbered_board(Board *);
void free_board(Board *);

int check_victory_condition(Board *);
void open_empty_squares(Board *);
int open_adjacent_squares(Board *, int, int);
int count_adjacent_flags(Board *, int, int);
int count_adjacent_mines(Board *, int, int);

int open_square(Board *, int, int);
int flag_square(Board *, int, int);
