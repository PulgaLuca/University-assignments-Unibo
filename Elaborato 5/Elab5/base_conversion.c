//#include "base_conversion.h"
#include <math.h>
#include <stdio.h>

// Numero di cifre rappresentabili in base b.
static int num_of_digits(unsigned int x, unsigned int base) {
	return ceil(log(x + 1) / log(base));
}

// ritorna singola cifra indicizzata della rappresentazione in base b.
static int nDigit(int n, int b, int pos) {
	unsigned int i = 0, resto = 0, nDigits = num_of_digits(n, b) - pos;
	for (i = 0; n != 0; i++) {
		resto = n % b;
		n /= b;
		if (i == nDigits) return resto;
	}
}

// Conversione iterativa.
void base_conversion_it(unsigned int n, unsigned int b) {
	unsigned int i, digit, nDigits = num_of_digits(n, b);
	if (b >= 2 && b <= 16) {
		if (n == 0) printf("%i", 0);
		else {
			for (i = 1; i <= nDigits; i++) {
				digit = nDigit(n, b, i); // ritorna singola cifra indicizzata della rappresentazione in base b.
				printf("%c", digit >= 10 ? digit + 55 : digit + 48); // Stampa numero o carattere in base al valore ritornato prima.
			}
		}
	}
}

// Conversione ricorsiva.
void base_conversion_rc(unsigned int x, unsigned int base) {
	unsigned int r;
	if (base >= 2 && base <= 16) {
		if (x != 0) {
			r = x % base;
			base_conversion_rc(x / base, base);
			if (r >= 10) printf("%c", r + 55); // Stampa numero o carattere in base al valore ritornato prima.
			else printf("%d", r);
		}
	}
}
