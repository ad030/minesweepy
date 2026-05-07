#ifndef BOARD_H
#define BOARD_H
#include "board.h"
#endif

#ifndef STATE_H
#define STATE_H
#include "state.h"
#endif

// menu
void display_menu_with_cursor(int);

// game board
void display_board_with_cursor(Board *, int, int);
void display_board_with_errors(Board *);
void display_full_board(Board *);
