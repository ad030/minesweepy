#include "display.h"

#ifndef NCURSES_H
#define NCURSES_H
#include <ncurses.h>
#endif

#define EMPTY "-"
#define MINE "X"
#define CURSOR_PAIR 1

void display_board(int **num_board, int **opened_board, int n, int m, int sel_x,
                   int sel_y)
{
        int i, j;

        init_pair(CURSOR_PAIR, COLOR_BLACK, COLOR_WHITE);

        for (i = 0; i < n; i++)
        {
                for (j = 0; j < m; j++)
                {
                        if (i == sel_x && j == sel_y)
                        {
                                attron(COLOR_PAIR(CURSOR_PAIR));
                        }
                        if (opened_board[i][j] == 1)
                        {
                                int current = num_board[i][j];
                                if (current == -1)
                                {
                                        printw(MINE);
                                }
                                else if (current == 0)
                                {
                                        printw(EMPTY);
                                }
                                else
                                {
                                        printw("%d", current);
                                }
                        }
                        else
                        {
                                printw("*");
                        }
                        attroff(COLOR_PAIR(CURSOR_PAIR));
                }
                printw("\n");
        }

        return;
}

void display_full_board(int **num_board, int n, int m)
{
        int i, j, current;
        for (i = 0; i < n; i++)
        {
                for (j = 0; j < m; j++)
                {
                        current = num_board[i][j];
                        if (current == -1)
                        {
                                printw("X");
                        }
                        else if (current == 0)
                        {
                                printw("-");
                        }
                        else
                        {
                                printw("%d", current);
                        }
                }
                printw("\n");
        }

        return;
}
