#include "board.h"
#include <stdlib.h>

#ifndef ASSERT_H
#define ASSERT_H
#include <assert.h>
#endif

int **initialize_numbered_board(Board *board)
{
        assert(board != NULL);
        assert(board->mines != NULL);

        // playerBoard tracks which squares are opened
        int **result = (int **)malloc(board->n * sizeof(int *));

        int i, j;

        // iterate over every square in board
        for (i = 0; i < board->n; i++)
        {
                result[i] = (int *)malloc(board->m * sizeof(int));

                for (j = 0; j < board->m; j++)
                {
                        // check if current square is a mine
                        if (board->mines[i][j] == 1)
                        {
                                result[i][j] = -1;
                                continue;
                        }

                        // check surrounding 8 squares for mines
                        result[i][j] = count_adjacent_mines(board, i, j);
                }
        }

        return result;
}

int **initialize_opened_board(Board *board)
{
        assert(board != NULL);
        assert(board->n >= 0 && board->m >= 0);

        int **result = (int **)malloc(board->n * sizeof(int *));

        for (int i = 0; i < board->n; i++)
        {
                result[i] = (int *)calloc(board->m, sizeof(int));
        }

        return result;
}

int **initialize_mine_board(Board *board)
{
        assert(board != NULL);
        assert(board->n >= 0 && board->m >= 0);
        assert(board->mine_count >= 0);

        int mine_count = board->mine_count;
        int n = board->n;
        int m = board->m;

        assert(mine_count >= 0 && mine_count <= n * m);
        assert(n >= 0 && m >= 0);

        int *result1d = (int *)calloc(n * m, sizeof(int));

        if (!result1d)
                return NULL;

        // set first x elements in table to 1
        int i;
        for (i = 0; i < mine_count; i++)
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
        assert(board != NULL);
        assert(board->opened != NULL);
        assert(board->mines != NULL);

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
        assert(board != NULL);
        assert(board->opened != NULL);
        assert(board->numbered != NULL);

        int i, j;
        int opened_counter = 1;

        while (opened_counter != 0)
        {
                opened_counter = 0;
                for (i = 0; i < board->n; i++)
                {
                        for (j = 0; j < board->m; j++)
                        {
                                if (board->opened[i][j] &&
                                    board->numbered[i][j] == 0)
                                {
                                        opened_counter +=
                                            open_adjacent_squares(board, i, j);
                                }
                        }
                }
        }
        return;
}

/**
 * Open the surrounding 8 squares around square at (x, y).
 */
int open_adjacent_squares(Board *board, int x, int y)
{
        assert(board != NULL);
        assert(x >= 0 && x < board->n);
        assert(y >= 0 && y < board->m);

        int a, b;
        int counter = 0;

        for (a = -1; a <= 1; a++)
        {
                for (b = -1; b <= 1; b++)
                {
                        if (x + a < 0 || x + a >= board->n)
                        {
                                continue;
                        }
                        else if (y + b < 0 || y + b >= board->m)
                        {
                                continue;
                        }

                        if (board->opened[x + a][b + y] == 0)
                        {
                                counter++;
                                board->opened[x + a][b + y] = 1;
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
 * mines and numbered are NOT initialized yet, set to NULL.
 */
Board *get_board_struct(int n, int m, int x)
{
        assert(n >= 0 && m >= 0); // board size must not be 0
        assert(x >= 0);           // mine count must be non-negative
        assert(x <= n * m); // mine count must not exceed total squares on board

        Board *board = (Board *)malloc(sizeof(Board));

        board->n = n;
        board->m = m;
        board->mine_count = x;
        board->flag_count = 0;
        board->opened = initialize_opened_board(board);
        board->mines = NULL;
        board->numbered = NULL;

        generate_mines(board);

        return board;
}

/**
 * Generate mine field and number board for the given Board struct.
 */
void generate_mines(Board *board)
{
        assert(board != NULL);

        if (board->mines != NULL)
        {
                free(board->mines);
                board->mines = NULL;
        }

        if (board->numbered != NULL)
        {
                free(board->numbered);
                board->numbered = NULL;
        }

        assert(board->mines == NULL);
        assert(board->numbered == NULL);

        board->mines = initialize_mine_board(board);
        board->numbered = initialize_numbered_board(board);

        assert(board->mines != NULL);
        assert(board->numbered != NULL);

        return;
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
        assert(board->flag_count >= 0);

        int success = 0;

        // toggle between flag and base state
        // cannot flag opened squares
        if (board->opened[x][y] == 0)
        {
                board->opened[x][y] = -1;
                board->flag_count++;
                success = 1;
        }
        else if (board->opened[x][y] == -1)
        {
                board->opened[x][y] = 0;
                board->flag_count--;
                success = 1;
        }

        assert(board->flag_count >= 0);

        return success;
}

void free_board(Board *board)
{
        assert(board != NULL);

        free(board->numbered);
        free(board->mines);
        free(board->opened);
        free(board);

        return;
}

int count_adjacent_flags(Board *board, int x, int y)
{
        assert(board != NULL);
        assert(x >= 0 && x < board->n);
        assert(y >= 0 && x < board->m);

        int i, j;
        int counter = 0;

        for (i = -1; i <= 1; i++)
        {
                for (j = -1; j <= 1; j++)
                {
                        // check for out of bounds
                        if (x + i < 0 || x + i >= board->n)
                        {
                                continue;
                        }
                        else if (y + j < 0 || y + j >= board->m)
                        {
                                continue;
                        }

                        // increment on flag
                        if (board->opened[x + i][y + j] == -1)
                        {
                                counter++;
                        }
                }
        }

        return counter;
}

int count_adjacent_mines(Board *board, int x, int y)
{
        assert(board != NULL);
        assert(x >= 0 && x < board->n);
        assert(y >= 0 && x < board->m);

        int i, j;
        int counter = 0;
        for (i = -1; i <= 1; i++)
        {
                for (j = -1; j <= 1; j++)
                {
                        // check for out of bounds
                        if (x + i < 0 || x + i >= board->n)
                        {
                                continue;
                        }
                        else if (y + j < 0 || y + j >= board->m)
                        {
                                continue;
                        }

                        // increment on mine
                        if (board->mines[x + i][y + j] == 1)
                        {
                                counter++;
                        }
                }
        }

        return counter;
}
