#include "display.h"

#ifndef BOARD_H
#define BOARD_H
#include "board.h"
#endif

#ifndef NCURSES_H
#define NCURSES_H
#include <ncurses.h>
#endif

#ifndef ASSERT_H
#define ASSERT_H
#include <assert.h>
#endif

#define EMPTY_SQUARE "-"
#define MINE_SQUARE "X"
#define HIDDEN_SQUARE "*"
#define FLAGGED_SQUARE ">"

#define CURSOR 1
#define MINE 2
#define ERROR 3
#define FALSE_FLAG 4

void display_board_with_cursor(Board *board, int sel_x, int sel_y)
{
        int i, j;

        init_pair(CURSOR, COLOR_BLACK, COLOR_WHITE);

        for (i = 0; i < board->n; i++)
        {
                for (j = 0; j < board->m; j++)
                {
                        int opened_state = board->opened[i][j];
                        int current = board->numbered[i][j];

                        if (i == sel_x && j == sel_y)
                        {
                                attron(COLOR_PAIR(CURSOR));
                        }

                        if (opened_state == 1)
                        {
                                if (current == -1)
                                {
                                        printw(MINE_SQUARE);
                                }
                                else if (current == 0)
                                {
                                        printw(EMPTY_SQUARE);
                                }
                                else
                                {
                                        printw("%d", current);
                                }
                        }
                        else if (opened_state == -1)
                        {
                                printw(FLAGGED_SQUARE);
                        }
                        else
                        {
                                printw(HIDDEN_SQUARE);
                        }
                        attroff(COLOR_PAIR(CURSOR));
                }
                printw("\n");
        }

        return;
}

void display_full_board(Board *board)
{
        int i, j, current;
        for (i = 0; i < board->n; i++)
        {
                for (j = 0; j < board->m; j++)
                {
                        current = board->numbered[i][j];
                        if (current == -1)
                        {
                                printw(MINE_SQUARE);
                        }
                        else if (current == 0)
                        {
                                printw(EMPTY_SQUARE);
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

void display_board_with_errors(Board *board)
{
        int i, j, is_mine, opened_state, num_mines_adj;

        init_pair(MINE, COLOR_BLACK, COLOR_WHITE);
        init_pair(ERROR, COLOR_WHITE, COLOR_RED);
        init_pair(FALSE_FLAG, COLOR_WHITE, COLOR_BLUE);

        for (i = 0; i < board->n; i++)
        {
                for (j = 0; j < board->m; j++)
                {
                        is_mine = board->mines[i][j];
                        opened_state = board->opened[i][j];
                        num_mines_adj = board->numbered[i][j];

                        if (is_mine == 1)
                        {
                                if (opened_state == 1)
                                {
                                        attron(COLOR_PAIR(ERROR));
                                        printw(MINE_SQUARE);
                                        attroff(COLOR_PAIR(ERROR));
                                }
                                else
                                {
                                        attron(COLOR_PAIR(MINE));
                                        printw(MINE_SQUARE);
                                        attroff(COLOR_PAIR(MINE));
                                }
                        }
                        else if (is_mine == 0)
                        {
                                if (opened_state == -1)
                                {
                                        attron(COLOR_PAIR(FALSE_FLAG));
                                        printw("%d", num_mines_adj);
                                        attroff(COLOR_PAIR(FALSE_FLAG));
                                }
                                else if (num_mines_adj == 0)
                                {
                                        printw(EMPTY_SQUARE);
                                }
                                else
                                {
                                        printw("%d", num_mines_adj);
                                }
                        }
                        else
                        {
                                printw("%d", board->numbered[i][j]);
                        }
                }
                printw("\n");
        }

        return;
}

void display_menu_with_cursor(int x)
{
        init_pair(CURSOR, COLOR_BLACK, COLOR_WHITE);

        char *options[] = {"Easy", "Intermediate", "Expert", "Quit"};

        for (int i = 0; i < NUM_MENU_OPTIONS; i++)
        {
                if (i == x)
                {
                        attron(COLOR_PAIR(CURSOR));
                        printw("%s\n", options[i]);
                        attroff(COLOR_PAIR(CURSOR));
                }
                else
                {
                        printw("%s\n", options[i]);
                }
        }

        return;
}
