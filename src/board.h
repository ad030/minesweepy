typedef struct Board {
  int n;          // length of board
  int m;          // width of board
  int num_mines;  // number of mines
  int num_flags;  // number of flags
  int **mines;    // tracks where mines are
  int **opened;   // tracks opened, unopened, flagged squares
  int **numbered; // for displaying results
} Board;

// initializing the game board
Board *initialize_board(int, int, int);
int **initialize_mine_board(int, int, int);
int **initialize_opened_board(int, int);
int **initialize_numbered_board(int **, int, int);
void free_board(Board *);

int check_victory_condition(Board *);
void open_empty_squares(Board *);
int open_adjacent_squares(int **, int, int, int, int);

int open_square(Board *, int, int);
int flag_square(Board *, int, int);
