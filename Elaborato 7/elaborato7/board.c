#include <stdio.h>
#include "board.h"
#include "game.h"

#define IS_MINE(board,i,j) (board[i][j] == UNKN_MINE || board[i][j] == FLAG_MINE || board[i][j] == MINE) // mine control
#define IS_FLAG(board,i,j) (board[i][j] == FLAG_MINE || board[i][j] == FLAG_FREE) // flag control
#define IS_UNKNOW(board,i,j) (board[i][j] == UNKN_MINE || board[i][j] == UNKN_FREE) //unknown control
#define SET_LIMIT int i0 = (i == 0) ? 0 : (i - 1); int j0 = (j == 0) ? 0 : (j - 1); int i1 = (i == (rows - 1)) ? i : (i + 1); int j1 = (j == (cols - 1)) ? j : (j + 1); // range di celle intorno [i,j]
#define ABS(x,y) ((x) > (y) ? (x)-(y) : (y)-(x))
#define COND(x,y) ((x == y) || (x == ++y) || (x == --y))

// Dichiarazione funzioni utility
static int count(int board[][GAME_COLS], unsigned int rows, unsigned int cols, int i, int j); // Conta numero mine nell'intorno [i, j]
static int count_flagged(int board[][GAME_COLS], unsigned int rows, unsigned int cols, int i, int j); // Conta numero flag nell'intorno [i, j]
static int totalCount = 0;

// RETURN NUMERO MINE NEL RANGE INTORNO [i, j]
static int count(int board[][GAME_COLS], unsigned int rows, unsigned int cols, int i, int j) {
	int a, b, c = 0;
	SET_LIMIT
	for (a = i0; a < i1; a++) {
		for (b = j0; b < j1; b++) {
			if (a != i || b != j) {
				if (IS_MINE(board, a, b)) {
					c++;
				}
			}
		}
	}
	return c;
}

// RETURN NUMERO FLAG NEL RANGE INTORNO [i, j]
static int count_flagged(int board[][GAME_COLS], unsigned int rows, unsigned int cols, int i, int j) {
	int a, b, c = 0;
	SET_LIMIT
	for (a = i0; a < i1; a++) {
		for (b = j0; b < j1; b++) {
			if (a != i || b != j) {
				if (IS_FLAG(board, a, b)) {
					c++;
				}
			}
		}
	}
	return c;
}

/*
 * Fills the rows*cols board with num_mines random mines
 * leaving free the neighborhood of position i,j
 */
void random_board(int board[][GAME_COLS], unsigned int rows, unsigned int cols, unsigned int i, unsigned int j, unsigned int num_mines) {
	unsigned int a, b, c = 0;
	srand(time(NULL)); // richiede <stdio.h>
	for (a = 0; a < rows; a++)
		for (b = 0; b < cols; b++)
			board[a][b] = UNKN_FREE; // setto tutto le celle a unknown_free
	while (c != num_mines) {
		a = rand() % rows;
		b = rand() % cols;
		if (board[a][b] == UNKN_FREE && (ABS(a, i) > 1 || (ABS(b, j) > 1))) {
			board[a][b] = UNKN_MINE;
			c++;
		}
	}
}

/*
 * Flags/unflags the i,j position in the board. Returns
 * - -1 if the position was flagged. Removes the flag
 * -  0 if the position is already displyed
 * -  1 if the position is not flagged and not already
 *    displayed. Puts a flag in position i,j.
 */
int flag_board(int board[][GAME_COLS], unsigned int rows, unsigned int cols, unsigned int i, unsigned int j) {
	int c = 0;
	switch (board[i][j]) {
	case UNKN_FREE:
		board[i][j] = FLAG_FREE; c = 1; break;
	case UNKN_MINE:
		board[i][j] = FLAG_MINE; c = 1; break;
	case FLAG_MINE:
		board[i][j] = UNKN_MINE; c = -1; break;
	case FLAG_FREE:
		board[i][j] = UNKN_FREE; c = -1; break;
	default: c = 0; break;
	}
	return c;
}

/*
 * Displays position i,j in the board. Returns the number of
 * displayed cells or -1 if i,j contains a mine.
 */
int display_board(int board[][GAME_COLS], unsigned int rows, unsigned int cols, unsigned int i, unsigned int j) {

	//caso in cui board[i][j] sia scoperta o "flaggata"
	if (board[i][j] < 9 || board[i][j] == 11 || board[i][j] == 12) {
		return 0;
	}
	//caso in cui board[i][j] sia una mina
	else if (board[i][j] == UNKN_MINE) {
		board[i][j] = MINE;
		return -1;
	}
	else if (board[i][j] == UNKN_FREE) {
		int r, c, minecount = 0, localCount = 0;
		// Scorro tutte le caselle 3x3 dell'intorno [i, j]
		for (r = ((int)i - 1); r <= ((int)i + 1); r++) {
			for (c = ((int)j - 1); c <= ((int)j + 1); c++) {
				// Escludo le eventuali caselle fuori dalla matrice.
				if (r >= 0 && r < rows && c >= 0 && c < cols) {
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
						// Tolgo i casi in cui board[r][c] sia gia' stata scoperta
						if (board[r][c] > 8) totalCount += display_board(board, rows, cols, r, c);
					}
				}
			}

		}
		localCount = totalCount + 1;
		totalCount = 0;
		return localCount;
	}
}

/*
 * Expands all the free cells sourrounding  position i,j in
 * the board. Returns the number of expanded cells or -1 if
 * one contains a mine.
 */
int expand_board(int board[][GAME_COLS], unsigned int rows, unsigned int cols, unsigned int i, unsigned int j) {

	int r, c, flagCount = 0;

	if (board[i][j] > 8) return 0;

	for (r = ((int)i - 1); r <= ((int)i + 1); r++) {
		for (c = ((int)j - 1); c <= ((int)j + 1); c++) {
			if (r >= 0 && c >= 0 && r < rows && c < cols) {
				if (board[r][c] == FLAG_FREE || board[r][c] == FLAG_MINE)
					flagCount++;
			}
		}
	}

	if (board[i][j] != flagCount) return 0;
	else {
		int count = 0;

		for (r = ((int)i - 1); r <= ((int)i + 1); r++) {
			for (c = ((int)j - 1); c <= ((int)j + 1); c++) {
				if (r >= 0 && c >= 0 && r < rows && c < cols) {
					if (board[r][c] == UNKN_MINE) return -1;
					else count += display_board(board, rows, cols, r, c);
				}
			}
		}
		return count;
	}
}