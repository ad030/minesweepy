#include "state.h"
#include "board.h"
#include "display.h"

#include <assert.h>
#include <ncurses.h>
#include <stdlib.h>

void init_global_state_struct(GlobalState *glob) {
        glob->current_state = MENU;
        glob->win = stdscr;
        initialize_board_struct(&(glob->board), 0, 0, 0);

        return;
}

void state_machine(GlobalState *glob) {
        while (glob->current_state != END) {
                switch (glob->current_state) {
                case MENU:
                        // printf("menu state\n");
                        handle_menu_state(glob);
                        break;
                case GAME:
                        // printf("game state\n");
                        handle_game_state(glob);
                        break;
                case WIN:
                        handle_win_state(glob);
                        break;
                case LOSE:
                        handle_lose_state(glob);
                        break;
                default:
                        break;
                }
        }

        handle_end_state(glob);

        return;
}

/**
 * Return either win or lose state
 */
void handle_game_state(GlobalState *glob) {
        assert(glob != NULL);

        int ch = 0;
        int sel_x, sel_y;
        sel_x = sel_y = 0;

        int is_first_square = 1;
        WINDOW *win = glob->win;
        Board *board = &(glob->board);

        while (glob->current_state == GAME) {
                wclear(win);
                display_board_with_cursor(win, board, sel_x, sel_y);
                wrefresh(win);

                ch = getch();

                switch (ch) {
                case KEY_UP:
                case 'k':
                case 'w':
                        sel_x--;
                        if (sel_x < 0)
                                sel_x = 0;

                        assert(sel_x >= 0 && sel_x < board->n);
                        break;
                case KEY_DOWN:
                case 'j':
                case 's':
                        sel_x++;
                        if (sel_x >= glob->board.n)
                                sel_x = glob->board.n - 1;

                        assert(sel_x >= 0 && sel_x < board->n);
                        break;
                case KEY_LEFT:
                case 'h':
                case 'a':
                        sel_y--;
                        if (sel_y < 0) {
                                sel_y = 0;
                        }
                        assert(sel_y >= 0 && sel_y < board->m);
                        break;
                case KEY_RIGHT:
                case 'l':
                case 'd':
                        sel_y++;
                        if (sel_y >= glob->board.m)
                                sel_y = glob->board.m - 1;

                        assert(sel_y >= 0 && sel_y < board->m);
                        break;
                case '\n':
                        // first click safety
                        // give player empty space on first click of board
                        // repeat until selected square has 0 mines surrounding
                        if (is_first_square == 1) {
                                while (board->numbered[sel_x][sel_y] != 0)
                                        generate_mines(board);

                                is_first_square = 0;
                        }

                        assert(is_first_square == 0);

                        // chording
                        if (board->opened[sel_x][sel_y] == 1 &&
                            board->numbered[sel_x][sel_y] ==
                                count_adjacent_flags(board, sel_x, sel_y)) {
                                open_adjacent_squares(board, sel_x, sel_y);
                        } else {
                                open_square(board, sel_x, sel_y);
                        }

                        open_empty_squares(board);

                        int is_won = check_victory_condition(board);

                        if (is_won == 1) {
                                glob->current_state = WIN;
                        } else if (is_won == -1) {
                                glob->current_state = LOSE;
                        }
                        break;
                case ' ':
                case 'f':
                        flag_square(board, sel_x, sel_y);
                        break;
                case KEY_BACKSPACE:
                        glob->current_state = END;
                        break;
                default:
                        break;
                }
        }

        return;
}

void handle_menu_state(GlobalState *glob) {
        assert(glob != NULL);

        int ch = 0;
        int sel = 0;

        WINDOW *win = glob->win;
        Board *board = &(glob->board);

        while (glob->current_state == MENU) {
                wclear(win);
                display_menu_with_cursor(win, sel);
                wrefresh(win);

                ch = getch();

                switch (ch) {
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
                        switch (sel) {
                        case 0:
                                initialize_board_struct(board, 9, 9, 10);
                                glob->current_state = GAME;
                                break;
                        case 1:
                                initialize_board_struct(board, 16, 16, 40);
                                glob->current_state = GAME;
                                break;
                        case 2:
                                initialize_board_struct(board, 16, 30, 99);
                                glob->current_state = GAME;
                                break;
                        case 3:
                                glob->current_state = END;
                                break;
                        default:
                                break;
                        }

                        break;
                case KEY_BACKSPACE:
                        glob->current_state = END;
                        break;
                default:
                        break;
                }
                sel = ((sel % NUM_MENU_OPTIONS) + NUM_MENU_OPTIONS) %
                      NUM_MENU_OPTIONS;
        }

        return;
}

void handle_end_state(GlobalState *glob) {
        endwin();
        return;
}

void handle_win_state(GlobalState *glob) {
        assert(glob != NULL);

        WINDOW *win = stdscr;
        Board *board = &(glob->board);

        wclear(win);
        wprintw(win, "You win!\n");
        display_board_with_errors(win, board);
        refresh();
        wrefresh(win);
        getch();

        glob->current_state = MENU;

        return;
}

void handle_lose_state(GlobalState *glob) {
        assert(glob != NULL);

        WINDOW *win = stdscr;
        Board *board = &(glob->board);

        wclear(win);
        wprintw(win, "You lose...\n");
        display_board_with_errors(win, board);
        refresh();
        wrefresh(win);
        getch();

        glob->current_state = MENU;

        return;
}
