#ifndef STATE_H
#define STATE_H

#include "board.h"
#include <ncurses.h>

#define NUM_MENU_OPTIONS 4

enum GameState { MENU, GAME, WIN, LOSE, END };

typedef struct game_state {
        enum GameState current_state;
        Board board;
        WINDOW *win;
} GlobalState;

void init_global_state_struct(GlobalState *glob);

void state_machine(GlobalState *glob);

void handle_menu_state(GlobalState *glob);
void handle_game_state(GlobalState *glob);
void handle_win_state(GlobalState *glob);
void handle_lose_state(GlobalState *glob);
void handle_end_state(GlobalState *glob);

#endif
