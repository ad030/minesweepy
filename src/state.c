#ifndef STATE_H
#define STATE_H
#include "state.h"
#endif

#ifndef DISPLAY_H
#define DISPLAY_H
#include "display.h"
#endif

#ifndef NCURSES_H
#define NCURSES_H
#include <ncurses.h>
#endif

/**
 * Return either win or lose state
 */
int handle_game_state(Board *board)
{
        clear();
        refresh();

        int ch = 0;
        int sel_x, sel_y;
        sel_x = sel_y = 0;

        while (ch != KEY_BACKSPACE)
        {
                clear();
                refresh();
                display_board_with_cursor(board, sel_x, sel_y);
                ch = getch();

                switch (ch)
                {
                case KEY_UP:
                        sel_x--;
                        if (sel_x < 0)
                        {
                                sel_x = 0;
                        }
                        break;
                case KEY_DOWN:
                        sel_x++;
                        if (sel_x >= board->n)
                        {
                                sel_x = board->n - 1;
                        }
                        break;
                case KEY_LEFT:
                        sel_y--;
                        if (sel_y < 0)
                        {
                                sel_y = 0;
                        }
                        break;
                case KEY_RIGHT:
                        sel_y++;
                        if (sel_y >= board->m)
                        {
                                sel_y = board->m - 1;
                        }
                        break;
                case '\n':
                        open_square(board, sel_x, sel_y);

                        if (board->numbered[sel_x][sel_y] == 0)
                        {
                                open_empty_squares(board);
                        }

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
                        flag_square(board, sel_x, sel_y);
                        break;
                default:
                        break;
                }
        }

        return END;
}

int handle_menu_state(Board *board)
{
        clear();
        refresh();

        int ch = 0;
        int sel_x = 0;

        while (ch != KEY_BACKSPACE)
        {
                clear();
                refresh();

                display_menu_with_cursor(sel_x);

                ch = getch();

                switch (ch)
                {
                case KEY_UP:
                        sel_x--;
                        if (sel_x < 0)
                        {
                                sel_x = 0;
                        }
                        break;
                case KEY_DOWN:
                        sel_x++;
                        if (sel_x >= NUM_MENU_OPTIONS)
                        {
                                sel_x = NUM_MENU_OPTIONS - 1;
                        }
                        break;
                case '\n':
                        switch (sel_x)
                        {
                        case 0:
                                *board = *initialize_board(9, 9, 10);
                                return GAME;
                                break;
                        case 1:
                                *board = *initialize_board(16, 16, 40);
                                return GAME;
                                break;
                        case 2:
                                *board = *initialize_board(16, 30, 99);
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
        }

        return END;
}

void handle_end_state(Board *board)
{
        free_board(board);
        return;
}

int handle_win_state(Board *board)
{
        printw("You win!\n");
        display_full_board(board);
        getch();
        return MENU;
}

int handle_lose_state(Board *board)
{
        printw("You lose...\n");
        display_board_with_errors(board);
        getch();
        return MENU;
}
