#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "bigint.h"

#define C2N(c) ((c)-'0')

static bigint *bigint_alloc(digit x) {
	bigint *tmp = (bigint *)malloc(sizeof(bigint));

	if(tmp != NULL) {
		tmp->x    = x;
		tmp->next = NULL;
		tmp->prev = NULL;
	}
	return tmp;
}

static int bigint_delete(bigint *N) {
	if(N == NULL) {
		return 1;
	} else {
		if(N->next != NULL)
			N->next->prev = N->prev;
		if(N->prev != NULL)
			N->prev->next = N->next;
		free(N);
		return 0;
	}
}

static int head_insert(bigint **N, digit x) {
	if(N == NULL) {
		return 1;
	} else if(*N == NULL) {
		return (*N = bigint_alloc(x)) == NULL;
	} else {
		bigint *tmp = bigint_alloc(x);

		if(tmp != NULL) {
			 tmp->next = *N;
			(*N)->prev = tmp;
			 *N        = tmp;
		}
		return tmp == NULL;
	}
}

static int head_delete(bigint **N) {
	if(N == NULL || *N == NULL) {
		return 1;
	} else {
		bigint *tmp = *N;

		*N = (*N)->next;
		return bigint_delete(tmp);
	}
}

static void remove_leading_zeros(bigint **N) {
	if(N != NULL) {
		while(*N != NULL && (*N)->x == 0 && (*N)->next != NULL)
			head_delete(N);
	}
}

static int well_formed_str(char *s) {
	if(s == NULL || *s == '\0') {
		return 0;
	} else {
		if(*s == '-') s++;
		if(*s == '\0')
			return 0;
		while(*s != '\0') {
			if(!isdigit(*s))
				return 0;
			s++;
		}
		return 1;
	}
}

static bigint *str2bigint_rec(char *s) {
	bigint *N = NULL;

	if(*s != '\0') {
		N = str2bigint_rec(s+1);
		head_insert(&N,C2N(*s));
	}
	return N;
}

static bigint *str2bigint(char *s) {
	bigint *N = NULL;

	if(well_formed_str(s)) {
		int sgn = 1;

		if(s[0]=='-') {
			sgn = -1;
			s++;
		}
		N = str2bigint_rec(s);
		remove_leading_zeros(&N);
		if(N != NULL && sgn == -1)
			N->x = -N->x;
	}

	return N;
}

static void print(bigint *N) {
	if(N == NULL) {
		printf("NaN");
	} else {
		while(N != NULL) {
			printf("%d",N->x);
			N = N->next;
		}
	}
}


