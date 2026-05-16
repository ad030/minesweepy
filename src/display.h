#ifndef DISPLAY_H
#define DISPLAY_H

#include "board.h"
#include "state.h"

#include <ncurses.h>

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

// struct for creating windows
typedef struct _win_border_struct {
  char ls, rs, ts, bs, tl, tr, bl, br;
} WinBorder;

typedef struct _win_struct {
  int startX, startY;
  int height, width;
  WinBorder *border;
} WinStruct;

// ncurses color pairs
void init_color_pairs();

// menu
void display_menu_with_cursor(WINDOW *, int);

// game board
void display_board_with_cursor(WINDOW *, Board *, int, int);
void display_board_with_errors(WINDOW *, Board *);
void display_full_board(WINDOW *, Board *);
void display_square(WINDOW *, Board *, int, int);
void display_square_hidden(WINDOW *, Board *, int, int);
void display_square_color(WINDOW *, Board *, int, int);

// board info
void display_remaining_mine_count(WINDOW *, Board *);

// ncurses windows
WINDOW *create_window(WinStruct *);
WinStruct *init_win_struct();

#endif
