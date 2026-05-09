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

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

void init_color_pairs()
{
        init_pair(CURSOR_COLOR, COLOR_BLACK, COLOR_WHITE);
        init_pair(EMPTY_COLOR, COLOR_WHITE, COLOR_BLACK);

        // for displaying board after game is over
        init_pair(MINE_COLOR, COLOR_BLACK, COLOR_WHITE);
        init_pair(ERROR_COLOR, COLOR_WHITE, COLOR_RED);
        init_pair(FALSE_FLAG_COLOR, COLOR_WHITE, COLOR_BLUE);

        // colour the numbers on board
        init_pair(ONE_COLOR, COLOR_BLUE, COLOR_BLACK);
        init_pair(TWO_COLOR, COLOR_GREEN, COLOR_BLACK);
        init_pair(THREE_COLOR, COLOR_RED, COLOR_BLACK);
        init_pair(FOUR_COLOR, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(FIVE_COLOR, COLOR_YELLOW, COLOR_BLACK);
        // using cyan for 6,7,8
        // ncurses doesn't have enough colours and you never see these anyway
        init_pair(SIX_COLOR, COLOR_CYAN, COLOR_BLACK);
        init_pair(SEVEN_COLOR, COLOR_CYAN, COLOR_BLACK);
        init_pair(EIGHT_COLOR, COLOR_CYAN, COLOR_BLACK);
}

void display_board_with_cursor(WINDOW *win, Board *board, int sel_x, int sel_y)
{
        assert(win != NULL);
        assert(board != NULL);
        assert(sel_x >= 0 && sel_x < board->n);
        assert(sel_y >= 0 && sel_y < board->m);

        int i, j;

        display_remaining_mine_count(win, board);

        for (i = 0; i < board->n; i++)
        {
                for (j = 0; j < board->m; j++)
                {
                        int is_opened = board->opened[i][j];
                        if (i == sel_x && j == sel_y)
                        {
                                attron(COLOR_PAIR(CURSOR_COLOR));
                                if (is_opened == 1)
                                {
                                        display_square(win, board, i, j);
                                }
                                else
                                {
                                        display_square_hidden(win, board, i, j);
                                }
                                attroff(COLOR_PAIR(CURSOR_COLOR));
                        }
                        else
                        {
                                if (is_opened == 1)
                                {
                                        display_square_color(win, board, i, j);
                                }
                                else
                                {
                                        display_square_hidden(win, board, i, j);
                                }
                        }
                }
                printw("\n");
        }

        return;
}

void display_full_board(WINDOW *win, Board *board)
{
        int i, j;
        for (i = 0; i < board->n; i++)
        {
                for (j = 0; j < board->m; j++)
                {
                        display_square_color(win, board, i, j);
                }
                printw("\n");
        }

        return;
}

void display_board_with_errors(WINDOW *win, Board *board)
{
        assert(win != NULL);
        assert(board != NULL);

        int i, j, is_mine, opened_state;

        for (i = 0; i < board->n; i++)
        {
                for (j = 0; j < board->m; j++)
                {
                        is_mine = board->mines[i][j];
                        opened_state = board->opened[i][j];

                        if (is_mine == 1)
                        {
                                if (opened_state == 1)
                                {
                                        attron(COLOR_PAIR(ERROR_COLOR));
                                        display_square(win, board, i, j);
                                        attroff(COLOR_PAIR(ERROR_COLOR));
                                }
                                else
                                {
                                        attron(COLOR_PAIR(MINE_COLOR));
                                        display_square(win, board, i, j);
                                        attroff(COLOR_PAIR(MINE_COLOR));
                                }
                        }
                        else if (is_mine == 0 && opened_state == -1)
                        {
                                attron(COLOR_PAIR(FALSE_FLAG_COLOR));
                                display_square(win, board, i, j);
                                attroff(COLOR_PAIR(FALSE_FLAG_COLOR));
                        }
                        else
                        {
                                display_square_color(win, board, i, j);
                        }
                }
                printw("\n");
        }

        return;
}

void display_menu_with_cursor(WINDOW *win, int x)
{
        assert(win != NULL);

        char *options[] = {"Easy", "Intermediate", "Expert", "Quit"};

        for (int i = 0; i < NUM_MENU_OPTIONS; i++)
        {
                if (i == x)
                {
                        attron(COLOR_PAIR(CURSOR_COLOR));
                        printw("%s\n", options[i]);
                        attroff(COLOR_PAIR(CURSOR_COLOR));
                }
                else
                {
                        printw("%s\n", options[i]);
                }
        }

        return;
}

/**
 * Display true value of square without any colours.
 * Mines and empty squares are replaced with their corresponding character.
 */
void display_square(WINDOW *win, Board *board, int x, int y)
{
        assert(win != NULL);
        assert(board != NULL);

        int square_num = board->numbered[x][y];

        if (square_num == -1)
        {
                wprintw(win, " %s ", MINE_SQUARE);
        }
        else if (square_num == 0)
        {
                wprintw(win, " %s ", EMPTY_SQUARE);
        }
        else
        {
                wprintw(win, " %d ", square_num);
        }
}

/**
 * Display square without colors
 * Hidden or flagged squares are displayed as such.
 */
void display_square_hidden(WINDOW *win, Board *board, int x, int y)
{
        assert(win != NULL);
        assert(board != NULL);

        int is_opened = board->opened[x][y];

        if (is_opened == -1)
        {
                wprintw(win, " %s ", FLAGGED_SQUARE);
        }
        else if (is_opened == 0)
        {
                wprintw(win, " %s ", HIDDEN_SQUARE);
        }
        else
        {
                display_square(win, board, x, y);
        }
}

/**
 * Display square with all colours for numbers.
 */
void display_square_color(WINDOW *win, Board *board, int x, int y)
{
        assert(win != NULL);
        assert(board != NULL);

        int square_num = board->numbered[x][y];

        switch (square_num)
        {
        case 0:
                attron(COLOR_PAIR(EMPTY_SQUARE));
                display_square(win, board, x, y);
                attroff(COLOR_PAIR(EMPTY_SQUARE));
                break;
        case 1:
                attron(COLOR_PAIR(ONE_COLOR));
                display_square(win, board, x, y);
                attroff(COLOR_PAIR(ONE_COLOR));
                break;
        case 2:
                attron(COLOR_PAIR(TWO_COLOR));
                display_square(win, board, x, y);
                attroff(COLOR_PAIR(TWO_COLOR));
                break;
        case 3:
                attron(COLOR_PAIR(THREE_COLOR));
                display_square(win, board, x, y);
                attroff(COLOR_PAIR(THREE_COLOR));
                break;
        case 4:
                attron(COLOR_PAIR(FOUR_COLOR));
                display_square(win, board, x, y);
                attroff(COLOR_PAIR(FOUR_COLOR));
                break;
        case 5:
                attron(COLOR_PAIR(FIVE_COLOR));
                display_square(win, board, x, y);
                attroff(COLOR_PAIR(FIVE_COLOR));
                break;
        case 6:
                attron(COLOR_PAIR(SIX_COLOR));
                display_square(win, board, x, y);
                attroff(COLOR_PAIR(SIX_COLOR));
                break;
        case 7:
                attron(COLOR_PAIR(SEVEN_COLOR));
                display_square(win, board, x, y);
                attroff(COLOR_PAIR(SEVEN_COLOR));
                break;
        case 8:
                attron(COLOR_PAIR(EIGHT_COLOR));
                display_square(win, board, x, y);
                attroff(COLOR_PAIR(EIGHT_COLOR));
                break;
        case -1:
                attron(COLOR_PAIR(MINE_COLOR));
                display_square(win, board, x, y);
                attroff(COLOR_PAIR(MINE_COLOR));
                break;
        default:
                break;
        }
}

void display_remaining_mine_count(WINDOW *win, Board *board)
{
        assert(win != NULL);
        assert(board != NULL);

        wprintw(win, "%d mines remaining\n",
                board->mine_count - board->flag_count);
        return;
}

/**
 * Create an ncurses Window based on the passed WinStruct.
 */
WINDOW *create_window(WinStruct *win)
{
        assert(win != NULL);
        WINDOW *new_win =
            newwin(win->width, win->height, win->startX, win->startY);

        if (new_win == NULL)
        {
                return NULL;
        }

        return new_win;
}

/**
 * Initialize a WinStruct with default parameters.
 */
WinStruct *init_win_struct()
{
        WinStruct *win = (WinStruct *)malloc(sizeof(WinStruct));

        if (win == NULL)
        {
                return NULL;
        }

        win->height = 5;
        win->width = 10;
        win->startY = (LINES - win->height) / 2;
        win->startX = (COLS - win->width) / 2;

        WinBorder *new_border = (WinBorder *)malloc(sizeof(WinBorder));

        if (new_border == NULL)
        {
                return NULL;
        }

        win->border = new_border;

        new_border->ls = '|';
        new_border->rs = '|';
        new_border->ts = '-';
        new_border->bs = '-';
        new_border->tl = '+';
        new_border->tr = '+';
        new_border->bl = '+';
        new_border->br = '+';

        return win;
}
