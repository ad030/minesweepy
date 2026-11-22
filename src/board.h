#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

#define MAX_BOARD_HEIGHT 50
#define MAX_BOARD_WIDTH 100

typedef struct Board {
        int n;          // length of board
        int m;          // width of board
        int mine_count; // number of mines
        int flag_count; // number of flags

        bool game_ended;

        // tracks where mines are
        int mines[MAX_BOARD_HEIGHT][MAX_BOARD_WIDTH];

        // tracks opened, unopened, flagged squares
        // 0 = unopened, 1 = opened, -1 = flagged
        int opened[MAX_BOARD_HEIGHT][MAX_BOARD_WIDTH];

        // for displaying board
        int numbered[MAX_BOARD_HEIGHT][MAX_BOARD_WIDTH];
} Board;

// initializing the game board

/**
 * Create and return the board struct for the game.
 * n is number of rows.
 * m is number of columns.
 * x is number of mines on the board.
 */
void initialize_board_struct(Board *, int, int, int);

void initialize_mine_board(Board *);
void initialize_opened_board(Board *);
void initialize_numbered_board(Board *);
void generate_mines(Board *);

int check_victory_condition(Board *);
void open_empty_squares(Board *);
int open_adjacent_squares(Board *, int, int);
int count_adjacent_flags(Board *, int, int);
int count_adjacent_mines(Board *, int, int);

int open_square(Board *, int, int);
int flag_square(Board *, int, int);

#endif
