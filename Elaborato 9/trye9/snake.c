#include "snake.h"
#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////
static void destroy_body(struct body* body) {
	if (body->next != NULL)
		destroy_body(body->next);
	free(body);
}

static void body_movement(struct snake* snake, struct position position) {
	unsigned int j;
	if (snake != NULL && snake->length >= 2) {
		struct body* tot = snake->body->next;
		struct position pos2;
		struct position pos1 = position;
		for (j = 1; j < snake->length; j++) {
			pos2 = tot->pos;
			tot->pos = pos1;
			pos1 = pos2;

			if (!(tot->next == NULL)) tot = tot->next;
		}
	}
}

static struct body* node_search(struct snake* snake, unsigned int i) {
	if (snake != NULL) {
		while (i-- > 0 && snake != NULL)
			snake->body = snake->body->next;
		return snake->body;
	}
}

static struct body* new_body_allocation(struct position position) {
	struct body* body = (struct body*)malloc(sizeof(struct body));
	if (body != NULL) {
		body->next = NULL;
		body->pos = position;
		body->prev = NULL;
	}
	return body;
}

static void reverse_body_pos(struct body* body) {
	struct body* bodychange;

	if (body->next != NULL) reverse_body_pos(body->next);

	bodychange = body->next;
	body->next = body->prev;
	body->prev = bodychange;
}

////////////////////////////////////////////////////////////////////////

/*
crea la struttura che rappresenta un serpente di lunghezza 1 (la testa del un serpente),
posizionato in una qualsiasi cella in una matrice di dimensione rows×cols.
*/
struct snake* snake_create(unsigned int rows, unsigned int cols) {
	struct position pos;
	pos.i = 0; pos.j = 0; // to change? in {0,0}
	struct snake* snake = (struct snake*)malloc(sizeof(struct snake));

	if (snake != NULL) {
		snake->length = 1;
		snake->cols = cols;
		snake->rows = rows;
		do {
			snake->body = new_body_allocation(pos);
		} while (snake->body == NULL);
		return snake;
	}
}

/*
distrugge la struttura serpente (i.e. libera la memoria allocata dinamicamente).
*/
void snake_kill(struct snake* s) {
	if (s != NULL) {
		if (s->body != NULL) destroy_body(s->body);
		free(s);
	}
}
/*
restituisce le coordinate della testa del serpente (ossia la posizione attuale)
*/
struct position snake_head(struct snake* s) {
	if (s != NULL)
		return s->body->pos;
}

/*
Restituisce la (posizione della) parte i-esima del corpo del serpente. La posizione 0 equivale alla
testa del serpente. Se il serpente è più corto di i, la posizione restituita non è definita. In altre parole ritorna la
posizione attuale di una parte del corpo del serpente, dove si assume che la testa sia in posizione 0.
*/
struct position snake_body(struct snake* s, unsigned int i) {
	if (s != NULL && s->body != NULL) {
		struct body* bodytmp = s->body;
		if (i > s->length) return;
		if (i == 0) return snake_head(s);
		while (i-- > 0 && bodytmp != NULL) {
			bodytmp = bodytmp->next;
		}
		return bodytmp->pos;
	}
}

/*
Restituisce 1 se il serpente attraversa se stesso, 0 altrimenti. In altre parole verifica che il
serpente non si sia annodato. Per verificare se il serpente è annodato è sufficiente verificare che le coordinate della
testa non siano ripetute nella lista.
*/
int snake_knotted(struct snake* s) {
	struct body* bodytmp = s->body->next;
	unsigned int length = 1;

	if (s != NULL) {
		if (s->length == 1) return 0;
		while (length < s->length) {
			if (bodytmp->pos.i == s->body->pos.i && bodytmp->pos.j == s->body->pos.j)
				return 1;
			else if (bodytmp->next != NULL)
				bodytmp = bodytmp->next;
			length++;
		}
		return 0;
	}
}
/*
sposta il serpente in una direzione (up, down, right, left). Tale funzione produce un nuovo set di
posizioni per l’oggetto serpente equivalente al set di coordinate ottenuto con la seguente procedura:
1. aggiungiamo una nuova testa nella direzione dir rispetto alla vecchia testa,
2. rimuoviamo la posizione in coda
*/
void snake_move(struct snake* s, enum direction dir) {
	struct position actualpos = s->body->pos;

	if (dir == LEFT) {
		s->body->pos.j = (s->body->pos.j - 1 + s->cols) % s->cols;
		body_movement(s, actualpos);
	}
	else if (dir == UP) {
		s->body->pos.i = (s->body->pos.i - 1 + s->rows) % s->rows;
		body_movement(s, actualpos);
	}
	else if (dir == RIGHT) {
		s->body->pos.j = (s->body->pos.j + 1 + s->cols) % s->cols;
		body_movement(s, actualpos);
	}
	else if (dir == DOWN) {
		s->body->pos.i = (s->body->pos.i + 1 + s->rows) % s->rows;
		body_movement(s, actualpos);
	}
}
/*
inverte il serpente (i.e. la coda diventa la testa e viceversa).
*/
void snake_reverse(struct snake* s) {
	struct body* body = s->body;
	s->body = node_search(s, s->length - 1);
	reverse_body_pos(body);
}

/*
aggiunge una nuova testa al serpente in una direzione specificata (up, down, right, left) rispetto alla testa
attuale.
*/
void snake_increase(struct snake* s, enum direction dir) {
	struct body* bodyacc = new_body_allocation(s->body->pos);
	if (s != NULL && bodyacc != NULL) {

		if (dir == LEFT) bodyacc->pos.j = (s->body->pos.j - 1 + s->cols) % s->cols;
		else if (dir == UP) bodyacc->pos.i = (s->body->pos.i - 1 + s->rows) % s->rows;
		else if (dir == RIGHT) bodyacc->pos.j = (s->body->pos.j + 1 + s->cols) % s->cols;
		else if (dir == DOWN) bodyacc->pos.i = (s->body->pos.i + 1 + s->rows) % s->rows;

		s->length++;
		bodyacc->next = s->body;
		s->body->prev = bodyacc;
		s->body = bodyacc;
	}
}

/*
rimuove un determinato numero di parti del serpente, a partire dalla coda.
*/
void snake_decrease(struct snake* s, unsigned int decrease_len) {
	if(s != NULL && decrease_len == s->length) snake_kill(s);
	if (s != NULL && decrease_len <= s->length) {

		struct body* bodytmp = s->body;
		int totdecr = (int)s->length - 1 - decrease_len;
		unsigned int j = 0;

		while (j < s->length) {
			if (j <= totdecr) {
				bodytmp = bodytmp->next;
			}
			else {
				bodytmp->prev->next = NULL;
				destroy_body(bodytmp);
				s->length -= decrease_len;
				return;
			}
			j++;
		}
	}
}

/*
salva le coordinate del serpente in un file. Il formato di salvataggio è libero.
*/
void snake_save(struct snake* s, char* filename) {
	unsigned int j = 0;
	struct body* bodytmp = s->body;
	FILE* file = fopen(filename, "w");

	//fprintf(file, "%u %u %u\n", s->rows, s->cols, s->length);
	fprintf(file, "%d\n", s->length);
	fprintf(file, "%d\n", s->rows);
	fprintf(file, "%d\n", s->cols);

	while (j < s->length) {
		fprintf(file, "%d %d\n", bodytmp->pos.i, bodytmp->pos.j);
		if (bodytmp->next != NULL) bodytmp = bodytmp->next;
		j++;
	}
	fclose(file);
}

/*
carica le coordinate del serpente da un file.
*/
struct snake* snake_read(char* filename) {
	struct snake* snake = (struct snake*)malloc(sizeof(struct snake));
	struct position pos;
	unsigned int j = 0;
	FILE* file = fopen(filename, "r");

	if (snake != NULL) {

		fscanf(file, "%d", &snake->length);
		fscanf(file, "%d", &snake->rows);
		fscanf(file, "%d", &snake->cols);

		while (j < snake->length) {
			pos.i = 0;
			pos.j = 0;
			struct body* bodytmp = new_body_allocation(pos);
			struct body* bodytmp1 = new_body_allocation(pos);

			if (bodytmp != NULL) {

				fscanf(file, "%d %d", &bodytmp->pos.i, &bodytmp->pos.j);

				if (j == 0) {
					snake->body = bodytmp;
					bodytmp1 = bodytmp;
				}
				else {
					while (bodytmp1->next != NULL) bodytmp1 = bodytmp1->next;
					bodytmp1->next = bodytmp;
					bodytmp->prev = bodytmp1;
				}
			}
			j++;
		}
	}
	fclose(file);
	return snake;
}

