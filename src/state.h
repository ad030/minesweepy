#ifndef STATE_H
#define STATE_H

#include "board.h"
#include <ncurses.h>

#define NUM_MENU_OPTIONS 4

enum GameState { MENU, GAME, WIN, LOSE, END };

typedef struct game_state {
  enum GameState current_state;
  Board *board;
} GlobalState;

GlobalState *init_global_state_struct();

int handle_menu_state(GlobalState *glob);
int handle_game_state(GlobalState *glob);
int handle_win_state(GlobalState *glob);
int handle_lose_state(GlobalState *glob);
int handle_end_state(GlobalState *glob);

#endif
