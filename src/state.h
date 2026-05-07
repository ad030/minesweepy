#ifndef BOARD_H
#define BOARD_H
#include "board.h"
#endif

#define NUM_MENU_OPTIONS 4

enum State { MENU, GAME, WIN, LOSE, END };

int handle_menu_state(Board *);
int handle_game_state(Board *);
int handle_win_state(Board *);
int handle_lose_state(Board *);
void handle_end_state(Board *);
