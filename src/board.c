#include "board.h"
#include <stdlib.h>

#ifndef ASSERT_H
#define ASSERT_H
#include <assert.h>
#endif

int **initialize_numbered_board(int **board, int n, int m)
{
        // playerBoard tracks which squares are opened
        int **result = (int **)malloc(n * sizeof(int *));

        int i, j, x, y;

        // iterate over every square in board
        for (i = 0; i < n; i++)
        {
                result[i] = (int *)malloc(m * sizeof(int));

                for (j = 0; j < m; j++)
                {
                        // check if current square is a mine
                        if (board[i][j] == 1)
                        {
                                result[i][j] = -1;
                                continue;
                        }

                        // check surrounding 8 squares for mines
                        int counter = 0;

                        for (x = -1; x <= 1; x++)
                        {
                                for (y = -1; y <= 1; y++)
                                {
                                        // check for out of bounds
                                        if (x + i < 0 || x + i >= n)
                                        {
                                                continue;
                                        }
                                        else if (j + y < 0 || j + y >= m)
                                        {
                                                continue;
                                        }

                                        // increment on mine
                                        if (board[i + x][j + y] == 1)
                                        {
                                                counter++;
                                        }
                                }
                        }
                        result[i][j] = counter;
                }
        }

        return result;
}

int **initialize_opened_board(int n, int m)
{
        int **result = (int **)malloc(n * sizeof(int *));

        for (int i = 0; i < n; i++)
        {
                result[i] = (int *)calloc(m, sizeof(int));
        }

        return result;
}

int **initialize_mine_board(int n, int m, int x)
{
        if (x < 0 || x > n * m)
        {
                return NULL;
        }
        if (n < 0 || m < 0)
        {
                return NULL;
        }

        int *result1d = (int *)calloc(n * m, sizeof(int));

        if (!result1d)
                return NULL;

        // set first x elements in table to 1
        int i;
        for (i = 0; i < x; i++)
        {
                result1d[i] = 1;
        }

        // fisher yates shuffle
        int random_num;
        for (i = n * m - 1; i > 0; i--)
        {
                random_num = rand() % i;

                int temp = result1d[i];
                result1d[i] = result1d[random_num];
                result1d[random_num] = temp;
        }

        // copy to 2d array
        int **result = (int **)malloc(n * sizeof(int *));
        int j;
        for (i = 0; i < n; i++)
        {
                result[i] = (int *)malloc(sizeof(int) * m);
                for (j = 0; j < m; j++)
                {
                        result[i][j] = result1d[i * m + j];
                }
        }

        free(result1d);

        return result;
}

/**
 * Compares opened squares to minefield.
 * If a mine was opened, game is lost and return -1.
 * If all non-mine squares were opened, game is won and return 1.
 * If game is ongoing, return 0.
 */
int check_victory_condition(Board *board)
{
        int i, j;

        int **mines = board->mines;
        int unopened_counter = 0;

        // square is opened == opened[i][j] == 1
        // mine square == mines[i][j] == 1
        for (i = 0; i < board->n; i++)
        {
                for (j = 0; j < board->m; j++)
                {
                        int opened_state = board->opened[i][j];
                        if (opened_state == 1)
                        {
                                // mine and opened; game is lost
                                if (mines[i][j] == 1)
                                {
                                        return -1;
                                }
                        }
                        else if (opened_state == 0)
                        {
                                if (mines[i][j] == 0)
                                {
                                        unopened_counter++;
                                }
                        }
                }
        }

        // if any unopened safe squares, continue
        if (unopened_counter != 0)
        {
                return 0;
        }

        // all non-mine squares opened; game is won
        return 1;
}

/**
 * After opening an empty square, open all empty squares adjacent to
 * it.
 */
void open_empty_squares(Board *board)
{
        int i, j;
        int opened_counter = 1;

        int **opened = board->opened;
        int **numbered = board->numbered;

        while (opened_counter != 0)
        {
                opened_counter = 0;
                for (i = 0; i < board->n; i++)
                {
                        for (j = 0; j < board->m; j++)
                        {
                                if (opened[i][j] && numbered[i][j] == 0)
                                {
                                        opened_counter += open_adjacent_squares(
                                            opened, board->n, board->m, i, j);
                                }
                        }
                }
        }
}

/**
 * Open the surrounding 8 squares around square at (x, y).
 */
int open_adjacent_squares(int **opened, int n, int m, int x, int y)
{
        int a, b;
        int counter = 0;
        for (a = -1; a <= 1; a++)
        {
                for (b = -1; b <= 1; b++)
                {
                        if (x + a < 0 || x + a >= n)
                        {
                                continue;
                        }
                        else if (y + b < 0 || y + b >= m)
                        {
                                continue;
                        }

                        int current = opened[x + a][b + y];

                        if (current == 0)
                        {
                                counter++;
                                opened[x + a][b + y] = 1;
                        }
                }
        }
        return counter;
}

/**
 * Create and return the board struct for the game.
 * n is number of rows.
 * m is number of columns.
 * x is number of mines on the board.
 */
Board *initialize_board(int n, int m, int x)
{
        assert(n >= 0 && m >= 0); // board size must not be 0
        assert(x >= 0);           // mine count must be non-negative
        assert(x <= n * m); // mine count must not exceed total squares on board

        Board *newBoard = (Board *)malloc(sizeof(Board));

        newBoard->n = n;
        newBoard->m = m;
        newBoard->mines = initialize_mine_board(n, m, x);
        newBoard->opened = initialize_opened_board(n, m);
        newBoard->numbered = initialize_numbered_board(newBoard->mines, n, m);

        return newBoard;
}

/**
 * Open the selected square (x, y) if possible.
 * Sets flag in board->opened to 1.
 * If opened, return 1 else return 0.
 */
int open_square(Board *board, int x, int y)
{
        assert(board != NULL);
        assert(x >= 0 && x < board->n);
        assert(y >= 0 && y < board->m);

        int open_state = board->opened[x][y];

        // can only open squares if in base state
        // not flagged, not already opened
        if (open_state == 0)
        {
                board->opened[x][y] = 1;
                return 1;
        }

        return 0;
}

int flag_square(Board *board, int x, int y)
{
        assert(board != NULL);
        assert(x >= 0 && x < board->n);
        assert(y >= 0 && y < board->m);

        // toggle between flag and base state
        // cannot flag opened squares
        if (board->opened[x][y] == 0)
        {
                board->opened[x][y] = -1;
                return 1;
        }
        else if (board->opened[x][y] == -1)
        {
                board->opened[x][y] = 0;
                return 1;
        }

        return 0;
}

void free_board(Board *board)
{
        free(board->mines);
        free(board->opened);
        free(board->numbered);
        free(board);

        return;
}
