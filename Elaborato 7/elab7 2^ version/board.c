#define BOARD_STUDENT
#ifdef BOARD_STUDENT
#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COND(x,y) ((x == y) || (x == ++y) || (x == --y))

static int total_count = 0;

void random_board(int board[][GAME_COLS], unsigned int rows, unsigned int cols, unsigned int i, unsigned int j, unsigned int num_mines) {

    srand(time(NULL));
    int r, c, nm;

    for (r = rows - 1; r >= 0; r--) {

        for (c = cols - 1; c >= 0; c--) {

            board[r][c] = UNKN_FREE;
        }
    }

    for (nm = num_mines; nm > 0; nm--) {
        do {
            r = (rand() % rows);
            c = (rand() % cols);
        } while (COND(r, i) || COND(c, j) || (board[r][c] == UNKN_MINE));

        board[r][c] = UNKN_MINE;
    }
}

int flag_board(int board[][GAME_COLS], unsigned int rows, unsigned int cols, unsigned int i, unsigned int j) {

    if (board[i][j] > 8 && board[i][j] < 13) {
        if (board[i][j] == FLAG_FREE) {
            board[i][j] = UNKN_FREE;
            return -1;
        }
        else if (board[i][j] == FLAG_MINE) {
            board[i][j] = UNKN_MINE;
            return -1;
        }
        else if (board[i][j] == UNKN_FREE) {
            board[i][j] = FLAG_FREE;
            return 1;
        }
        else if (board[i][j] == UNKN_MINE) {
            board[i][j] = FLAG_MINE;
            return 1;
        }

    }

    return 0;
}

int display_board(int board[][GAME_COLS], unsigned int rows, unsigned int cols, unsigned int i, unsigned int j) {

    if (board[i][j] < 9 || board[i][j] == 11 || board[i][j] == 12) {
        return 0;
    }
    else if (board[i][j] == UNKN_MINE) {
        board[i][j] = MINE;
        return -1;
    }
    else if (board[i][j] == UNKN_FREE) {
        int r, c, minecount = 0, local_count = 0;
        ;

        for (r = ((int)i - 1); r <= ((int)i + 1); r++) {
            for (c = ((int)j - 1); c <= ((int)j + 1); c++) {

                if (r >= 0 && c >= 0 && r < rows && c < cols) {

                    if (board[r][c] == UNKN_MINE || board[r][c] > 11)
                        minecount++;
                }
            }
        }
        board[i][j] = minecount;

        if (minecount == 0) {
            for (r = ((int)i - 1); r <= ((int)i + 1); r++) {
                for (c = ((int)j - 1); c <= ((int)j + 1); c++) {

                    if (r >= 0 && c >= 0 && r < rows && c < cols) {

                        if (board[r][c] > 8) {

                            total_count += display_board(board, rows, cols, r, c);
                        }
                    }
                }
            }

        }
        local_count = total_count + 1;
        total_count = 0;
        return local_count;
    }

}

int expand_board(int board[][GAME_COLS], unsigned int rows, unsigned int cols, unsigned int i, unsigned int j) {

    if (board[i][j] > 8) {
        return 0;
    }

    int r, c, flagcount = 0;

    for (r = ((int)i - 1); r <= ((int)i + 1); r++) {
        for (c = ((int)j - 1); c <= ((int)j + 1); c++) {

            if (r >= 0 && c >= 0 && r < rows && c < cols) {

                if (board[r][c] == FLAG_FREE || board[r][c] == FLAG_MINE)
                    flagcount++;
            }
        }
    }

    if (board[i][j] != flagcount) {
        return 0;
    }
    else {
        int count = 0;


        for (r = ((int)i - 1); r <= ((int)i + 1); r++) {
            for (c = ((int)j - 1); c <= ((int)j + 1); c++) {

                if (r >= 0 && c >= 0 && r < rows && c < cols) {

                    if (board[r][c] == UNKN_MINE) {
                        return -1;
                    }
                    else {
                        count += display_board(board, rows, cols, r, c);

                    }

                }
            }
        }

        return count;

    }
}
#endif
