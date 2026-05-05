#include "board.h"
#include "display.h"
#include <stdio.h>

#ifndef NCURSES_H
#define NCURSES_H
#include <ncurses.h>
#endif

int main(void)
{
        int n, m, x, sel_x, sel_y;

        n = m = 9;
        x = 10;
        sel_x = sel_y = 0;

        int **mine_board;
        int **opened_board;
        int **numbered_board;

        int ch;

        initscr();
        raw();
        keypad(stdscr, TRUE);
        noecho();
        start_color();

        mine_board = initialize_mine_board(n, m, x);
        opened_board = initialize_opened_board(n, m);
        numbered_board = get_numbered_board(mine_board, n, m);

        ch = getch();
        display_board(numbered_board, opened_board, n, m, sel_x, sel_y);
        do
        {
                if (ch == KEY_UP)
                {
                        if (sel_x > 0)
                        {
                                sel_x--;
                        }
                }
                else if (ch == KEY_DOWN)
                {
                        if (sel_x < n - 1)
                        {
                                sel_x++;
                        }
                }
                else if (ch == KEY_LEFT)
                {
                        if (sel_y > 0)
                        {
                                sel_y--;
                        }
                }
                else if (ch == KEY_RIGHT)
                {
                        if (sel_y < n - 1)
                        {
                                sel_y++;
                        }
                }
                else if (ch == '\n')
                {
                        opened_board[sel_x][sel_y] = 1;
                        if (numbered_board[sel_x][sel_y] == 0)
                        {
                                open_empty_squares(numbered_board, opened_board,
                                                   n, m);
                        }

                        int is_won = check_victory_condition(
                            mine_board, opened_board, n, m);

                        if (is_won == 1)
                        {
                                printw("You won!\n");
                                display_full_board(numbered_board, n, m);
                                getch();
                                break;
                        }
                        else if (is_won == -1)
                        {
                                printw("You lost...\n");
                                display_full_board(numbered_board, n, m);
                                getch();
                                break;
                        }
                }

                display_board(numbered_board, opened_board, n, m, sel_x, sel_y);
                ch = getch();
                clear();
                refresh();
        } while (ch != KEY_BACKSPACE);

        endwin();

        printf("hello world!");
        return 0;
}
