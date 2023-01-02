#include <stdlib.h>
#include "bigint.h"

#define SGN(N) ((N) == NULL ? 0 : ((N)->x < 0 ? -1 : 1)) // Analizzo il segno dell'operando

// Return the last element of a number
static bigint* last(bigint* N) {
	if (N != NULL) {
		return N->prev;
	}
}

// Circular Bigint allocation 
static bigint* bigint_alloc(digit x) {
	bigint* tmp = (bigint*)malloc(sizeof(bigint));
	if (tmp != NULL) {
		tmp->x = x;
		tmp->next = tmp;
		tmp->prev = tmp;
	}
	return tmp;
}

// Inserimento di un elemento in testa
static int bigint_insert(bigint* N, digit x) {
	if (N == NULL) return 1;
	else {
		bigint* tmp = bigint_alloc(x), * nxt = N->next, * prv = N;
		if (tmp != NULL) {
			tmp->prev = prv;
			tmp->next = nxt;
			prv->next = tmp;
			nxt->prev = tmp;
		}
		return tmp == NULL;
	}
}

// Inserimento in testa di un elemento
static int head_insert(bigint** N, digit x) {
	if (N == NULL) return 1;
	else if (*N == NULL) return (*N = bigint_alloc(x)) == NULL;
	else if (bigint_insert((*N)->prev, x) == 1) return 1;
	else return (*N = (*N)->prev) == NULL;
}

// Inserimento in coda di un elemento
static int tail_insert(bigint** N, digit x) {
	if (N == NULL) return 1;
	else if (*N == NULL) return head_insert(N, x);
	else return bigint_insert((*N)->prev, x);
}

// Aggiunge zeri a seconda del numero di moltiplicazioni effettuate fino a quel momento.
static void add_zeros(bigint* N, unsigned int n) {
	int i;
	if (N != NULL && N->x != 0) {
		for (i = 0; i < n; i++)
			tail_insert(&N, 0);
	}
}

// Somma fra i risultati ottenuti dalle moltiplicazioni.
static bigint* sum_pos(bigint* N1, bigint* N2) {
	bigint* N = NULL;
	int car = 0; // riporto
	int val = 0; // resto
	if (SGN(N1) > 0 && SGN(N2) > 0) {
		// tengo conto della testa dei numeri.
		bigint* tmp2 = N2;
		bigint* tmp1 = N1;
		N1 = N1->prev;
		N2 = N2->prev;

		while (N1 != NULL || N2 != NULL || car != 0) {
			val = (N1 ? N1->x : 0) + (N2 ? N2->x : 0) + car;
			car = val / 10;
			val = val % 10;
			head_insert(&N, val);
			N1 = N1 && N1 != tmp1 ? N1->prev : NULL; // se prev viene assegnato, vuol dire che ancora non siamo arrivati alla coda
			N2 = N2 && N2 != tmp2 ? N2->prev : NULL; // se prev viene assegnato, vuol dire che ancora non siamo arrivati alla coda
		}
	}
	return N;
}

// eliminazione elemento bigint
static int bigint_delete(bigint* N) {
	if (N == NULL) return 1;
	else {
		bigint* prv = N->prev, * nxt = N->next;
		nxt->prev = prv;
		prv->next = nxt;
		free(N);
		return 0;
	}
}

// elimina l'elemento in testa
static int head_delete(bigint** N) {
	if (N == NULL || *N == NULL) return 1;
	else if (*N == (*N)->next) {
		free(*N);
		*N = NULL;
		return 0;
	}
	else {
		*N = (*N)->next;
		return bigint_delete((*N)->prev);
	}
}

static void delete(bigint** N) {
	if (N != NULL) {
		while (*N != NULL) {
			head_delete(N);
		}
	}
}

// Inverte il segno di N
static void negate(bigint* N) {
	// se N è non NULL
	if (N != NULL) {
		// invertire il segno di N
		N->x = N->x * (-1);
	}
}

// MOltiplicazione de N1 per la cifra x del numero N2
static bigint* mul_digit_pos(bigint* N, digit x) {
	bigint* X = NULL;
	int val = 0, car = 0;
	bigint* tmp = N;
	N = N->prev;
	while (N != NULL || car != 0) {
		val = (N ? N->x : 0) * x + car;
		car = val / 10; // riporto
		val = val % 10; // valore
		head_insert(&X, val);
		N = (N && N != tmp) ? N->prev : NULL; // se prev viene assegnato, vuol dire che ancora non siamo arrivati alla coda
	}
	return X;
}

// Multiplication between a N2's digit and the whole N1 bigint
static bigint* mul_digit(bigint* N, digit x) {
	if (N == NULL || x < -9 || x > 9) return NULL;
	else if (x == 0) return bigint_alloc(0);
	else {
		bigint* ans;
		int sgn1 = SGN(N);
		int sgn2 = x < 0 ? -1 : 1;
		if (sgn1 < 0) negate(N);
		if (sgn2 < 0) x = -x;
		ans = mul_digit_pos(N, x);
		if (sgn1 < 0)     negate(N);
		if (sgn1 != sgn2) negate(ans);
		return ans;
	}
}

// Multiplication between N1 and N2
bigint* mul(bigint* N1, bigint* N2) {
	int sgn1 = SGN(N1);
	int sgn2 = SGN(N2);
	bigint* ans = NULL; // result

	if (sgn1 == 0 || sgn2 == 0) return NULL;
	else {
		bigint* tmp = last(N2); // takes the first (the last) digit of the 2° bigint
		bigint* tmpfirst = last(N2);
		int n = 0; // Zeros counter
		// attenzione al segno di N1 e N2.
		// Li tengo positivi per la moltiplicazione e poi in caso sia -*+, allora cambierò il segno solo alla fine della moltiplicazione.
		if (sgn1 == -1) negate(N1);
		if (sgn2 == -1) negate(N2);

		ans = bigint_alloc(0);

		// case limit: in caso ci fosse solo una cifra, non entrerebbe mai con while (il primo numero è anche l'ultimo), con do while lo fa almeno una volta, e quindi anche per una sola cifra
		do {
			bigint* a, * b; //b = accumulator;
			a = mul_digit(N1, tmp->x); // multiplication between from the last digit of the 2° number to the first and the whole N1 number
			add_zeros(a, n++); // Aggiungo numero di zeri a seconda della moltiplicazione che sto effettuando (1^ = 0, 2^ = 1, 3^ = 2, ...)
			b = sum_pos(ans, a); // Sommo tutti i risultati delle moltiplicazioni
			delete(&ans);
			delete(&a);
			ans = b; // Risultato ottenuto ad ogni iterazione (mul)
			tmp = tmp->prev; // passo alla prossima cifra, del 2° numero, verso sinistra

		} while (tmp != tmpfirst);

		// attenzione al segno di N1 e N2.
		if (sgn1 != sgn2) negate(ans);
	}
	return ans;
}
