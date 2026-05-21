#include "state.h"
#include "board.h"
#include "display.h"

#include <assert.h>
#include <ncurses.h>
#include <stdlib.h>

/**
 * Return either win or lose state
 */
int handle_game_state(GlobalState *glob)
{
        assert(glob != NULL);
        assert(glob->board != NULL);

        int ch = 0;
        int sel_x, sel_y;
        sel_x = sel_y = 0;

        int is_first_square = 1;
        WINDOW *win = stdscr;

        while (ch != KEY_BACKSPACE)
        {
                wclear(win);
                display_board_with_cursor(win, glob->board, sel_x, sel_y);
                wrefresh(win);

                ch = getch();

                switch (ch)
                {
                case KEY_UP:
                case 'k':
                case 'w':
                        sel_x--;
                        if (sel_x < 0)
                                sel_x = 0;

                        assert(sel_x >= 0 && sel_x < glob->board->n);
                        break;
                case KEY_DOWN:
                case 'j':
                case 's':
                        sel_x++;
                        if (sel_x >= glob->board->n)
                                sel_x = glob->board->n - 1;

                        assert(sel_x >= 0 && sel_x < glob->board->n);
                        break;
                case KEY_LEFT:
                case 'h':
                case 'a':
                        sel_y--;
                        if (sel_y < 0)
                        {
                                sel_y = 0;
                        }
                        assert(sel_y >= 0 && sel_y < glob->board->m);
                        break;
                case KEY_RIGHT:
                case 'l':
                case 'd':
                        sel_y++;
                        if (sel_y >= glob->board->m)
                                sel_y = glob->board->m - 1;

                        assert(sel_y >= 0 && sel_y < glob->board->m);
                        break;
                case '\n':
                        // first click safety
                        // give player empty space on first click of board
                        // repeat until selected square has 0 mines surrounding
                        if (is_first_square == 1)
                        {
                                while (glob->board->numbered[sel_x][sel_y] != 0)
                                        generate_mines(glob->board);

                                is_first_square = 0;
                        }

                        assert(is_first_square == 0);
                        assert(glob->board->mines != NULL);
                        assert(glob->board->numbered != NULL);

                        // chording
                        if (glob->board->opened[sel_x][sel_y] == 1 &&
                            glob->board->numbered[sel_x][sel_y] ==
                                count_adjacent_flags(glob->board, sel_x, sel_y))
                        {
                                open_adjacent_squares(glob->board, sel_x,
                                                      sel_y);
                        }
                        else
                        {
                                open_square(glob->board, sel_x, sel_y);
                        }

                        open_empty_squares(glob->board);

                        int is_won = check_victory_condition(glob->board);

                        if (is_won == 1)
                        {
                                return WIN;
                        }
                        else if (is_won == -1)
                        {
                                return LOSE;
                        }
                        break;
                case ' ':
                case 'f':
                        flag_square(glob->board, sel_x, sel_y);
                        break;
                default:
                        break;
                }
        }

        return END;
}

int handle_menu_state(GlobalState *glob)
{
        assert(glob != NULL);
        assert(glob->board != NULL);

        int ch = 0;
        int sel = 0;

        WINDOW *win = stdscr;

        while (ch != KEY_BACKSPACE)
        {
                wclear(win);
                display_menu_with_cursor(win, sel);
                wrefresh(win);

                ch = getch();

                switch (ch)
                {
                case KEY_UP:
                case 'w':
                case 'k':
                        sel--;
                        break;
                case KEY_DOWN:
                case 's':
                case 'j':
                        sel++;
                        // if (sel >= NUM_MENU_OPTIONS)
                        // {
                        //         sel = NUM_MENU_OPTIONS - 1;
                        // }
                        break;
                case '\n':

                        if (glob->board != NULL)
                                free_board(glob->board);

                        switch (sel)
                        {
                        case 0:
                                glob->board = get_board_struct(9, 9, 10);
                                return GAME;
                                break;
                        case 1:
                                glob->board = get_board_struct(16, 16, 40);
                                return GAME;
                                break;
                        case 2:
                                glob->board = get_board_struct(16, 30, 99);
                                return GAME;
                                break;
                        case 3:
                                return END;
                                break;
                        default:
                                break;
                        }

                        break;
                default:
                        break;
                }
                sel = ((sel % NUM_MENU_OPTIONS) + NUM_MENU_OPTIONS) %
                      NUM_MENU_OPTIONS;
        }

        return END;
}

int handle_end_state(GlobalState *glob)
{
        assert(glob != NULL);
        assert(glob->board != NULL);

        free_board(glob->board);
        free(glob);

        return 0;
}

int handle_win_state(GlobalState *glob)
{
        assert(glob != NULL);
        assert(glob->board != NULL);

        WINDOW *win = stdscr;

        wclear(win);
        wprintw(win, "You win!\n");
        display_board_with_errors(win, glob->board);
        refresh();
        wrefresh(win);
        getch();
        return MENU;
}

int handle_lose_state(GlobalState *glob)
{
        assert(glob != NULL);
        assert(glob->board != NULL);

        WINDOW *win = stdscr;

        wclear(win);
        wprintw(win, "You lose...\n");
        display_board_with_errors(win, glob->board);
        refresh();
        wrefresh(win);
        getch();
        return MENU;
}

GlobalState *init_global_state_struct()
{
        GlobalState *glob = (GlobalState *)malloc(sizeof(GlobalState));

        if (glob == NULL)
                return NULL;

        glob->current_state = MENU;
        glob->board = get_board_struct(0, 0, 0);

        return glob;
}
