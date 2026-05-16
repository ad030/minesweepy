#include "board.h"
#include "display.h"
#include "state.h"

#include <assert.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
        int game_state = MENU;

        initscr();
        cbreak();
        keypad(stdscr, TRUE);
        curs_set(0);
        noecho();

        // set up ncurses colors
        start_color();
        init_color_pairs();

        // max terminal size
        int yMax, xMax;
        getmaxyx(stdscr, yMax, xMax);
        printf("%d, %d\n", yMax, xMax);

        Board board = *get_board_struct(0, 0, 0);
        WINDOW *win = stdscr;

        // WinStruct *win_struct = init_win_struct();
        // win_struct->height = yMax;
        // win_struct->width = xMax;
        // win_struct->startY = (LINES - win_struct->height) / 2;
        // win_struct->startX = (COLS - win_struct->width) / 2;
        // win = create_window(win_struct);
        // box(win, 0, 0);

        int seed = time(NULL);
        srand(seed);

        do
        {
                switch (game_state)
                {
                case MENU:
                        game_state = handle_menu_state(win, &board);
                        break;
                case GAME:
                        game_state = handle_game_state(win, &board);
                        break;
                case WIN:
                        game_state = handle_win_state(win, &board);
                        break;
                case LOSE:
                        game_state = handle_lose_state(win, &board);
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

        return 0;
}
