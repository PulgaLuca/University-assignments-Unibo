#include <stdlib.h>
#include "bigint.h"

#define SGN(N) ((N) == NULL ? 0 : ((N)->x < 0 ? -1 : 1)) // Analizzo il segno dell'operando

// Return the last element of a number
static bigint* last(bigint* N) {
	if (N != NULL) {
		// fintanto che il campo next di N è diverso da NULL
		while (N->next != NULL) {
			// spostiamo N al successivo
			N = N->next;
		}
		return N;
	}
	return N;
}

// Digit bigint allocation
static bigint* bigint_alloc(digit x) {
	bigint* tmp = (bigint*)malloc(sizeof(bigint));

	if (tmp != NULL) {
		tmp->x = x;
		tmp->next = NULL;
		tmp->prev = NULL;
	}
	return tmp;
}

static int head_insert(bigint** N, digit x) {
	if (N == NULL) {
		return 1;
	}
	else if (*N == NULL) {
		return (*N = bigint_alloc(x)) == NULL;
	}
	else {
		bigint* tmp = bigint_alloc(x);

		if (tmp != NULL) {
			tmp->next = *N;
			(*N)->prev = tmp;
			*N = tmp;
		}
		return tmp == NULL;
	}
}

// Inserimento in coda
static int tail_insert(bigint** N, digit x) {
	if (N == NULL) {
		return 1;
	}
	else if (*N == NULL) {
		return head_insert(N, x);
	}
	else {
		bigint* tmp = last(*N);
		tmp->next = bigint_alloc(x);
		if (tmp->next != NULL) {
			tmp->next->prev = tmp;
		}
		return tmp->next != NULL;
	}
}

// Aggiunge zeri a seconda del numero di moltiplicazioni effettuate fino a quel momento.
static void add_zeros(bigint* N, unsigned int n) {
	if (N != NULL && N->x != 0) {
		int i;
		for (i = 0; i < n; i++)
			tail_insert(&N, 0);
	}
}

// Inserimento di un elemento in testa
static int bigint_insert(bigint* N, digit x) {
	if (N == NULL) {
		return 1;
	}
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

// Somma fra i risultati ottenuti dalle moltiplicazioni.
static bigint* sum_pos(bigint* N1, bigint* N2) {
	bigint* N = NULL;
	if (SGN(N1) > 0 && SGN(N2) > 0) {
		int val = 0, car = 0;
		N1 = last(N1);
		N2 = last(N2);
		while (N1 != NULL || N2 != NULL || car != 0) {
			val = (N1 ? N1->x : 0) + (N2 ? N2->x : 0) + car;
			car = val / 10;
			val = val % 10;
			head_insert(&N, val);
			N1 = N1 ? N1->prev : NULL;
			N2 = N2 ? N2->prev : NULL;
		}
	}
	return N;
}

static int bigint_delete(bigint* N) {
	if (N == NULL) {
		return 1;
	}
	else {
		if (N->next != NULL)
			N->next->prev = N->prev;
		if (N->prev != NULL)
			N->prev->next = N->next;
		free(N);
		return 0;
	}
}

static int head_delete(bigint** N) {
	if (N == NULL || *N == NULL) {
		return 1;
	}
	else {
		bigint* tmp = *N;
		*N = (*N)->next;
		return bigint_delete(tmp);
	}
}

static void delete(bigint** N) {
	if (N != NULL) {
		/*while (*N != NULL) {
			head_delete(N);
		}*/
		head_delete(N);
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

static bigint* mul_digit_pos(bigint* N, digit x) {
	bigint* X = NULL;
	int val = 0, car = 0;
	N = last(N);
	while (N != NULL || car != 0) {
		val = (N ? N->x : 0) * x + car;
		car = val / 10;
		val = val % 10;
		head_insert(&X, val); // Inserisco in testa il risultato della moltiplicazione fra le due digit
		N = N ? N->prev : NULL; // Scorro N e vado alla k-esima cifra indietro di N1
	}
	return X;
}

// Multiplication between a N2's digit and the whole N1 bigint
static bigint* mul_digit(bigint* N, digit x) {
	bigint* X = NULL;
	//se N è NULL o x è esterno all’intervallo (-9,9)
	if (N == NULL || x < -9 || x > 9) {
		//restituisci NULL
		return NULL;
	}
	//altrimenti se x è uguale a 0 
	else if (x == 0) {
		// chiama bigint_alloc(0);
		bigint_alloc(0);
	} //altrimenti 
	else {
		//definisci i segni di N e x
		// 
		// tieni conto dei segni di N ed x
		// 
		// assegna ad X il valore della funzione mul_digit_pos(N,x);
		X = mul_digit_pos(N, x);
		//definisci i segni di N ed X

		return X;
	}
}

// Multiplication between N1 and N2
bigint* mul(bigint* N1, bigint* N2) {
	int sgn1 = SGN(N1);
	int sgn2 = SGN(N2);
	bigint* N = NULL;

	if (sgn1 == 0 || sgn2 == 0) { // Se i segni sono NULL, allora ritorno NULL
		return NULL;
	}
	else {
		bigint* tmp = last(N2); // takes the first (the last) digit of the 2° bigint
		int n = 0; // Zeros counter
		// attenzione al segno di N1 e N2.
		if (sgn1 == -1) {
			negate(N1);
		}
		if (sgn2 == -1) {
			negate(N2);
		}
		N = bigint_alloc(0);

		while (tmp != NULL) {
			bigint* a, * b; //b = accumulator;
			a = mul_digit(N1, tmp->x); // multiplication between from the last digit of the 2° number to the first and the whole N1 number
			add_zeros(a, n++); // Aggiungo numero di zeri a seconda della moltiplicazione che sto effettuando (1^ = 0, 2^ = 1, 3^ = 2, ...)
			b = sum_pos(N, a); // Sommo tutti i risultati delle moltiplicazioni
			delete(&N);
			delete(&a);
			N = b; // Risultato ottenuto ad ogni iterazione (mul)
			tmp = tmp->prev; // passo alla prossima cifra, del 2° numero, verso sinistra
		}

		// attenzione al segno di N1 e N2.

		if (sgn1 != sgn2) {
			negate(N);
		}
	}
	return N;
}