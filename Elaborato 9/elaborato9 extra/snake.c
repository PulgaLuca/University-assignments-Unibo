#include "snake.h"
#include <stdio.h>
#include <stdlib.h>

static struct body* body_alloc(struct position pos) {
	struct body* tmp = (struct body*)malloc(sizeof(struct body));

	if (tmp != NULL) {
		tmp->pos = pos;
		tmp->next = NULL;
		tmp->prev = NULL;
	}
	return tmp;
}

/* Creates a snake's head in a world of size rows*cols */
struct snake* snake_create(unsigned int rows, unsigned int cols) {
	if (rows > 0 && cols > 0) {

		struct snake* s = (struct snake*)malloc(sizeof(struct snake));

		if (s != NULL) {
			s->rows = rows;
			s->cols = cols;
			s->length = 1;
			
			struct position p;
			p.i = 0;
			p.j = 0;

			do {
				s->body = body_alloc(p);
			} while (s->body == NULL);


			return s;
		}
		
	}
}

static void destroy_body(struct body* b) {
	if (b != NULL) {
		if (b->next != NULL) {
			destroy_body(b->next);
		}
		free(b);
	}
}

/* Destroys the snake data structure. */
void snake_kill(struct snake* s) {
	if (s != NULL) {
		
		if (s->body != NULL) {
			destroy_body(s->body);
		}
		
		free(s);
	}
}

/* Returns the (coordinates of the) snake's head. */
struct position snake_head(struct snake* s) {
	if (s != NULL)
		return s->body->pos;
}
/*
 * Returns the (position of the) i-th part of the snake body.
 * Position 0 is equivalento the snake's head. If the snake
 * is shorter than i, the position returned is not defined.
 */
struct position snake_body(struct snake* s, unsigned int i) {

	if (s != NULL) {
		struct body* tmp = s->body;

		if (i == 0) {
			return snake_head(s);
		}

		if (i > s->length)
			return;

		while (i-- > 0 && tmp != NULL) {
			tmp = tmp->next;
		}

		return tmp->pos;
	}
}

/*  Returns 1 if the snake crosses himself, 0 otherwise. */
int snake_knotted(struct snake* s) {
	if (s != NULL) {

		if (s->length == 1)
			return 0;

		int l;

		struct body* tmp = s->body->next;

		for (l = 1; l < s->length; l++) {
			if (tmp->pos.i == s->body->pos.i && tmp->pos.j == s->body->pos.j) {

				return 1;
			}
			else if (tmp->next != NULL)
				tmp = tmp->next;
		}

		return 0;
	}
}

static void snake_move_body(struct snake* s, struct position tmp) {
	if (s != NULL && s->length > 1) {
		int i;

		struct body* acc = s->body->next;
		struct position tmp1 = tmp, tmp2;
		for (i = 1; i < s->length; i++) {

			tmp2 = acc->pos;
			acc->pos = tmp1;
			tmp1 = tmp2;

			if(acc->next != NULL)
				acc = acc->next;
		}

	}
}

/*
 * Moves the snake one step forward in the dir direction
 * into a toroidal world of size rows*cols.
 */
void snake_move(struct snake* s, enum direction dir) {
	if (s != NULL && dir >= 0 && dir <= 3) {

		struct position postmp = s->body->pos;

		switch (dir)
		{
		case UP:
			s->body->pos.i = (s->body->pos.i - 1 + s->rows) % s->rows;
			snake_move_body(s, postmp);
			break;
		case DOWN:
			s->body->pos.i = (s->body->pos.i + 1 + s->rows) % s->rows;
			snake_move_body(s, postmp);
			break;
		case LEFT:
			s->body->pos.j = (s->body->pos.j - 1 + s->cols) % s->cols;
			snake_move_body(s, postmp);
			break;
		case RIGHT:
			s->body->pos.j = (s->body->pos.j + 1 + s->cols) % s->cols;
			snake_move_body(s, postmp);
			break;
		default:
			break;
		}
	}
}

static void body_reverse(struct body* b) {
	struct body* tmp;

	if (b->next != NULL) {
		body_reverse(b->next);
	}

	tmp = b->next;
	b->next = b->prev;
	b->prev = tmp;
}
/*
 * Reverses the snake.
 * The tail of the snake is now the new head.
 */
void snake_reverse(struct snake* s) {
	if (s != NULL && s->length > 1) {
		
		struct body* b_tmp = s->body;
		struct body* tmp = s->body;

		int i;
		for (i = 0; i < s->length; i++) {

			if (i == s->length - 1)
				s->body = tmp;
			else
				tmp = tmp->next;

		}

		body_reverse(b_tmp);

		s->body->prev = NULL;
		tmp = s->body;
		for (i = 0; i < s->length; i++) {

			if (i == s->length - 1)
				tmp->next = 0;
			else
				tmp = tmp->next;

		}
	}
}
/*
 * Increases the snake length.
 * This is equivalent to:
 * - add a new head in the dir direction wrt the old head.
 */
void snake_increase(struct snake* s, enum direction dir) {
	if (s != NULL && dir >= 0 && dir <= 3) {

		struct body* new = body_alloc(s->body->pos);

		if (new != NULL) {


			switch (dir)
			{
			case UP:
				new->pos.i = (s->body->pos.i - 1 + s->rows) % s->rows;
				break;
			case DOWN:
				new->pos.i = (s->body->pos.i + 1 + s->rows) % s->rows;
				break;
			case LEFT:
				new->pos.j = (s->body->pos.j - 1 + s->cols) % s->cols;
				break;
			case RIGHT:
				new->pos.j = (s->body->pos.j + 1 + s->cols) % s->cols;
				break;
			default:
				break;
			}


			new->next = s->body;
			s->body->prev = new;
			s->body = new;
			s->length++;
		}
	}
}

/* Removes from the tail of the snake decrease_len parts.*/
void snake_decrease(struct snake* s, unsigned int decrease_len) {
	if (s != NULL && decrease_len <= s->length) {

		if (decrease_len == s->length)
			snake_kill(s);

		int i, k = (int)s->length - decrease_len - 1;
		struct body* tmp = s->body;

		for (i = 0; i < s->length; i++) {
			if (i > k) {
				tmp->prev->next = NULL;
				destroy_body(tmp);
				s->length -= decrease_len;
				return;
			}
			else {

				tmp = tmp->next;
			}
		}

	}
}

/* Saves the snake into the filename. */
void snake_save(struct snake* s, char* filename) {
	if (s != NULL && filename != NULL) {
		FILE* f = fopen(filename, "w");

		fprintf(f, "%u %u %u\n", s->rows, s->cols, s->length);

		int i;
		struct body* tmp = s->body;

		for (i = 0; i < s->length; i++) {
			fprintf(f, "%u %u\n", tmp->pos.i, tmp->pos.j);
			if (tmp->next != NULL) {
				tmp = tmp->next;
			}
		}

		fclose(f);
	}
}

/* Loads the snake from filename */
struct snake* snake_read(char* filename) {
	if (filename != NULL) {
		FILE* f = fopen(filename, "r");

		struct snake* s = (struct snake*)malloc(sizeof(struct snake));

		if (s != NULL) {

			fscanf(f, "%u", &s->rows);
			fscanf(f, "%u", &s->cols);
			fscanf(f, "%u", &s->length);

			int i;

			for (i = 0; i < s->length; i++) {

				struct position pos;
				pos.i = 0;
				pos.j = 0;
				struct body* tmp = body_alloc(pos), *tmp1 = body_alloc(pos);

				if (tmp != NULL) {

					fscanf(f, "%u %u", &tmp->pos.i, &tmp->pos.j);

					if (i == 0) {
						s->body = tmp;
						tmp1 = tmp;
					}
					else {

						while (tmp1->next != NULL) {
							tmp1 = tmp1->next;
						}

						tmp1->next = tmp;
						tmp->prev = tmp1;
						
					}


				}
			}
		}

		fclose(f);
		return s;
	}
}

