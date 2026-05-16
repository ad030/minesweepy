#include "state.h"
#include "display.h"

#include <assert.h>
#include <ncurses.h>

/**
 * Return either win or lose state
 */
int handle_game_state(WINDOW *win, Board *board)
{
        assert(win != NULL);
        assert(board != NULL);

        int ch = 0;
        int sel_x, sel_y;
        sel_x = sel_y = 0;

        int is_first_square = 1;

        while (ch != KEY_BACKSPACE)
        {
                wclear(win);
                display_board_with_cursor(win, board, sel_x, sel_y);
                wrefresh(win);

                ch = getch();

                switch (ch)
                {
                case KEY_UP:
                case 'k':
                case 'w':
                        sel_x--;
                        if (sel_x < 0)
                        {
                                sel_x = 0;
                        }
                        assert(sel_x >= 0 && sel_x < board->n);
                        break;
                case KEY_DOWN:
                case 'j':
                case 's':
                        sel_x++;
                        if (sel_x >= board->n)
                        {
                                sel_x = board->n - 1;
                        }
                        assert(sel_x >= 0 && sel_x < board->n);
                        break;
                case KEY_LEFT:
                case 'h':
                case 'a':
                        sel_y--;
                        if (sel_y < 0)
                        {
                                sel_y = 0;
                        }
                        assert(sel_y >= 0 && sel_y < board->m);
                        break;
                case KEY_RIGHT:
                case 'l':
                case 'd':
                        sel_y++;
                        if (sel_y >= board->m)
                        {
                                sel_y = board->m - 1;
                        }
                        assert(sel_y >= 0 && sel_y < board->m);
                        break;
                case '\n':
                        // first click safety
                        // give player empty space on first click of board
                        // repeat until selected square has 0 mines surrounding
                        if (is_first_square == 1)
                        {
                                while (board->numbered[sel_x][sel_y] != 0)
                                {
                                        generate_mines(board);
                                }

                                is_first_square = 0;
                        }

                        assert(is_first_square == 0);
                        assert(board->mines != NULL);
                        assert(board->numbered != NULL);

                        // chording
                        if (board->opened[sel_x][sel_y] == 1 &&
                            board->numbered[sel_x][sel_y] ==
                                count_adjacent_flags(board, sel_x, sel_y))
                        {
                                open_adjacent_squares(board, sel_x, sel_y);
                        }
                        else
                        {
                                open_square(board, sel_x, sel_y);
                        }

                        open_empty_squares(board);

                        int is_won = check_victory_condition(board);

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
                        flag_square(board, sel_x, sel_y);
                        break;
                default:
                        break;
                }
        }

        return END;
}

int handle_menu_state(WINDOW *win, Board *board)
{
        assert(win != NULL);
        assert(board != NULL);

        int ch = 0;
        int sel = 0;

        while (ch != KEY_BACKSPACE)
        {
                wclear(win);
                display_menu_with_cursor(win, sel);
                wrefresh(win);

                ch = getch();

                switch (ch)
                {
                case KEY_UP:
                        sel--;
                        break;
                case KEY_DOWN:
                        sel++;
                        // if (sel >= NUM_MENU_OPTIONS)
                        // {
                        //         sel = NUM_MENU_OPTIONS - 1;
                        // }
                        break;
                case '\n':
                        switch (sel)
                        {
                        case 0:
                                *board = *get_board_struct(9, 9, 10);
                                return GAME;
                                break;
                        case 1:
                                *board = *get_board_struct(16, 16, 40);
                                return GAME;
                                break;
                        case 2:
                                *board = *get_board_struct(16, 30, 99);
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

void handle_end_state(Board *board)
{
        assert(board != NULL);

        free_board(board);
        return;
}

int handle_win_state(WINDOW *win, Board *board)
{
        assert(win != NULL);
        assert(board != NULL);

        wclear(win);
        wprintw(win, "You win!\n");
        display_board_with_errors(win, board);
        refresh();
        wrefresh(win);
        getch();
        return MENU;
}

int handle_lose_state(WINDOW *win, Board *board)
{
        assert(win != NULL);
        assert(board != NULL);

        wclear(win);
        printw("You lose...\n");
        display_board_with_errors(win, board);
        refresh();
        wrefresh(win);
        getch();
        return MENU;
}
