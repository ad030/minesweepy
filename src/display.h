#ifndef BOARD_H
#define BOARD_H
#include "board.h"
#endif

#ifndef STATE_H
#define STATE_H
#include "state.h"
#endif

// for printing full board
#define EMPTY_SQUARE "-"
#define HIDDEN_SQUARE "*"
#define FLAGGED_SQUARE ">"
#define MINE_SQUARE "X"

// colors for displaying board
#define EMPTY_COLOR 10
#define CURSOR_COLOR 11
#define MINE_COLOR 12
#define ERROR_COLOR 13
#define FALSE_FLAG_COLOR 14

// colors for numbers on board
#define ONE_COLOR 1
#define TWO_COLOR 2
#define THREE_COLOR 3
#define FOUR_COLOR 4
#define FIVE_COLOR 5
#define SIX_COLOR 6
#define SEVEN_COLOR 7
#define EIGHT_COLOR 8

// ncurses color pairs
void init_color_pairs();

// menu
void display_menu_with_cursor(int);

// game board
void display_board_with_cursor(Board *, int, int);
void display_board_with_errors(Board *);
void display_full_board(Board *);
void display_square(Board *, int, int);
void display_square_hidden(Board *, int, int);
void display_square_color(Board *, int, int);

// board info
void display_remaining_mine_count(Board *);
