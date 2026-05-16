#include "board.h"

#include <ncurses.h>

#ifndef STATE_H
#define STATE_H
#define NUM_MENU_OPTIONS 4

enum State { MENU, GAME, WIN, LOSE, END };

int handle_menu_state(WINDOW *, Board *);
int handle_game_state(WINDOW *, Board *);
int handle_win_state(WINDOW *, Board *);
int handle_lose_state(WINDOW *, Board *);
void handle_end_state(Board *);

#endif
