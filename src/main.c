#ifndef BOARD_H
#define BOARD_H
#include "board.h"
#endif

#include "display.h"
#include <stdio.h>

#ifndef NCURSES_H
#define NCURSES_H
#include <ncurses.h>
#endif

#ifndef ASSERT_H
#define ASSERT_H
#include <assert.h>
#endif

#ifndef STATE_H
#define STATE_H
#include "state.h"
#endif

int main(void)
{
        int game_state = MENU;

        initscr();
        cbreak();
        keypad(stdscr, TRUE);
        noecho();

        // set up ncurses colors
        start_color();
        init_color_pairs();

        Board board = *initialize_board(0, 0, 0);

        do
        {
                clear();
                refresh();

                switch (game_state)
                {
                case MENU:
                        game_state = handle_menu_state(&board);
                        break;
                case GAME:
                        game_state = handle_game_state(&board);
                        break;
                case WIN:
                        game_state = handle_win_state(&board);
                        break;
                case LOSE:
                        game_state = handle_lose_state(&board);
                        break;
                case END:
                        handle_end_state(&board);
                        break;
                default:
                        game_state = END;
                        break;
                }

        } while (game_state != END);

        endwin();

        printf("Done\n");
        return 0;
}
