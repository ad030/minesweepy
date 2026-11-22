#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "board.h"

void initialize_board_struct(Board *board, int n, int m, int x) {
        assert(board != NULL);
        assert(n >= 0 && n < MAX_BOARD_HEIGHT);
        assert(m >= 0 && m < MAX_BOARD_WIDTH);

        // mine count cannot be more than number of squares on board
        assert(x >= 0 && x <= n * m);

        board->n = n;
        board->m = m;
        board->mine_count = x;
        board->flag_count = 0;
        board->game_ended = false;

        initialize_mine_board(board);
        initialize_opened_board(board);
        initialize_numbered_board(board);
        generate_mines(board);

        return;
}

void initialize_numbered_board(Board *board) {
        assert(board != NULL);
        assert(board->n >= 0 && board->n < MAX_BOARD_HEIGHT);
        assert(board->m >= 0 && board->m < MAX_BOARD_WIDTH);

        int i, j;

        // iterate over every square in board
        for (i = 0; i < board->n; i++) {
                // result[i] = (int *)malloc(board->m * sizeof(int));

                for (j = 0; j < board->m; j++) {
                        // check if current square is a mine
                        if (board->mines[i][j] == 1) {
                                board->numbered[i][j] = -1;
                                continue;
                        }

                        // check surrounding 8 squares for mines
                        board->numbered[i][j] =
                            count_adjacent_mines(board, i, j);
                }
        }

        return;
}

void initialize_opened_board(Board *board) {
        assert(board != NULL);
        assert(board->n >= 0 && board->n < MAX_BOARD_HEIGHT);
        assert(board->m >= 0 && board->m < MAX_BOARD_WIDTH);

        int i, j;

        for (i = 0; i < board->n; i++) {
                for (j = 0; j < board->m; j++) {
                        board->opened[i][j] = 0;
                };
        }

        return;
}

void initialize_mine_board(Board *board) {
        assert(board != NULL);
        assert(board->n >= 0 && board->n < MAX_BOARD_HEIGHT);
        assert(board->m >= 0 && board->m < MAX_BOARD_WIDTH);
        assert(board->mine_count >= 0 &&
               board->mine_count <= board->n * board->m);

        int mine_count = board->mine_count;
        int n = board->n;
        int m = board->m;

        int i, j;

        int result1d[MAX_BOARD_HEIGHT * MAX_BOARD_WIDTH];

        for (i = 0; i < MAX_BOARD_HEIGHT * MAX_BOARD_WIDTH; i++) {
                result1d[i] = 0;
        }

        // mines to be shuffled throughout board
        for (i = 0; i < mine_count; i++) {
                result1d[i] = 1;
        }

        // Fisher-Yates shuffle
        int random_num;
        for (i = n * m - 1; i > 0; i--) {
                random_num = rand() % i;

                int temp = result1d[i];
                result1d[i] = result1d[random_num];
                result1d[random_num] = temp;
        }

        // copy 1d shuffle to 2d mines board
        for (i = 0; i < n; i++) {
                for (j = 0; j < m; j++) {
                        board->mines[i][j] = result1d[i * m + j];
                }
        }

        return;
}

/**
 * Compares opened squares to minefield.
 * If a mine was opened, game is lost and return -1.
 * If all non-mine squares were opened, game is won and return 1.
 * If game is ongoing, return 0.
 */
int check_victory_condition(Board *board) {
        assert(board != NULL);

        int i, j;

        int unopened_counter = 0;

        // square is opened == opened[i][j] == 1
        // mine square == mines[i][j] == 1
        for (i = 0; i < board->n; i++) {
                for (j = 0; j < board->m; j++) {
                        int opened_state = board->opened[i][j];
                        if (opened_state == 1) {
                                // mine and opened; game is lost
                                if (board->mines[i][j] == 1) {
                                        return -1;
                                }
                        } else if (opened_state == 0) {
                                if (board->mines[i][j] == 0) {
                                        unopened_counter++;
                                }
                        }
                }
        }

        // if any unopened safe squares, continue
        if (unopened_counter != 0) {
                return 0;
        }

        // all non-mine squares opened; game is won
        return 1;
}

/**
 * After opening an empty square, open all empty squares adjacent to
 * it.
 */
void open_empty_squares(Board *board) {
        assert(board != NULL);

        int i, j;
        int opened_counter = 1;

        while (opened_counter != 0) {
                opened_counter = 0;
                for (i = 0; i < board->n; i++) {
                        for (j = 0; j < board->m; j++) {
                                if (board->opened[i][j] &&
                                    board->numbered[i][j] == 0) {
                                        opened_counter +=
                                            open_adjacent_squares(board, i, j);
                                }
                        }
                }
        }
        return;
}

/**
 * Open the 8 squares surrounding (x, y).
 */
int open_adjacent_squares(Board *board, int x, int y) {
        assert(board != NULL);
        assert(x >= 0 && x < board->n);
        assert(y >= 0 && y < board->m);

        int a, b;
        int counter = 0;

        for (a = -1; a <= 1; a++) {
                for (b = -1; b <= 1; b++) {
                        if (x + a < 0 || x + a >= board->n) {
                                continue;
                        } else if (y + b < 0 || y + b >= board->m) {
                                continue;
                        }

                        if (board->opened[x + a][y + b] == 0) {
                                counter++;
                                board->opened[x + a][y + b] = 1;
                        }
                }
        }

        return counter;
}

/**
 * Generate mine field and number board for the given Board struct.
 */
void generate_mines(Board *board) {
        assert(board != NULL);

        initialize_mine_board(board);
        initialize_numbered_board(board);

        assert(board->mines != NULL);
        assert(board->numbered != NULL);

        return;
}

/**
 * Open the selected square (x, y) if possible.
 * Sets flag in board->opened to 1.
 * If opened, return 1 else return 0.
 */
int open_square(Board *board, int x, int y) {
        assert(board != NULL);
        assert(x >= 0 && x < board->n);
        assert(y >= 0 && y < board->m);

        int open_state = board->opened[x][y];

        // can only open squares if in base state
        // not flagged, not already opened
        if (open_state == 0) {
                board->opened[x][y] = 1;
                return 1;
        }

        return 0;
}

int flag_square(Board *board, int x, int y) {
        assert(board != NULL);
        assert(x >= 0 && x < board->n);
        assert(y >= 0 && y < board->m);
        assert(board->flag_count >= 0);

        int success = 0;

        // toggle between flag and base state
        // cannot flag opened squares
        if (board->opened[x][y] == 0) {
                board->opened[x][y] = -1;
                board->flag_count++;
                success = 1;
        } else if (board->opened[x][y] == -1) {
                board->opened[x][y] = 0;
                board->flag_count--;
                success = 1;
        }

        assert(board->flag_count >= 0);

        return success;
}

int count_adjacent_flags(Board *board, int x, int y) {
        assert(board != NULL);
        assert(board->n >= 0 && board->n < MAX_BOARD_HEIGHT);
        assert(board->m >= 0 && board->m < MAX_BOARD_WIDTH);
        assert(x >= 0 && x < board->n);
        assert(y >= 0 && x < board->m);

        int i, j;
        int counter = 0;

        for (i = -1; i <= 1; i++) {
                for (j = -1; j <= 1; j++) {
                        // check for out of bounds
                        if (x + i < 0 || x + i >= board->n) {
                                continue;
                        } else if (y + j < 0 || y + j >= board->m) {
                                continue;
                        }

                        // increment on flag
                        if (board->opened[x + i][y + j] == -1) {
                                counter++;
                        }
                }
        }

        return counter;
}

int count_adjacent_mines(Board *board, int x, int y) {
        assert(board != NULL);
        assert(board->n >= 0 && board->n < MAX_BOARD_HEIGHT);
        assert(board->m >= 0 && board->m < MAX_BOARD_WIDTH);
        assert(x >= 0 && x < board->n);
        assert(y >= 0 && x < board->m);

        int i, j;
        int counter = 0;

        for (i = -1; i <= 1; i++) {
                for (j = -1; j <= 1; j++) {
                        // check for out of bounds
                        if (x + i < 0 || x + i >= board->n) {
                                continue;
                        } else if (y + j < 0 || y + j >= board->m) {
                                continue;
                        }

                        // increment on mine
                        if (board->mines[x + i][y + j] == 1) {
                                counter++;
                        }
                }
        }

        return counter;
}
