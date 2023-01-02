#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Numero di cifre rappresentabili in base b.
static int num_of_digits(unsigned int x, unsigned int base) {
	return ceil(log(x + 1) / log(base));
}

static int nDigit(int n, int b, int pos) {
	unsigned long int i = 0, resto = 0, nDigits = num_of_digits(n, b) - pos;
	for (i = 0; n != 0; i++) {
		resto = n % b;
		n /= b;
		if (i == nDigits) return resto;
	}
}

void base_conversion_it(unsigned int n, unsigned int b) {
	unsigned long int i, digit, nDigits = num_of_digits(n, b);
	if (b >= 2 && b <= 16) {
		if (n == 0) printf("%i", 0);
		else {
			for (i = 1; i <= nDigits; i++) {
				digit = nDigit(n, b, i);
				printf("%c", digit >= 10 ? digit + 55 : digit + 48);
			}
		}
	}
}

void base_conversion_rc(unsigned int x, unsigned int base) {
	unsigned int r;
	if (base >= 2 && base <= 16) {
		if (x != 0) {
			r = x % base;
			base_conversion_rc(x / base, base);
			if (r >= 10) printf("%c", r + 55);
			else printf("%d", r);
		}
	}
}

int main() {
	printf("\nCONVERTING: 123456 WITH 2: RECURSIVE\n");
	base_conversion_rc(123456, 2);
	printf("\nCONVERTING: 123456 WITH 2: ITERATIVE\n");
	base_conversion_it(123456, 2);

	printf("\nCONVERTING: 123456 WITH 3: RECURSIVE\n");
	base_conversion_rc(123456, 3);
	printf("\nCONVERTING: 123456 WITH 3: ITERATIVE\n");
	base_conversion_it(123456, 3);

	printf("\nCONVERTING: 123456 WITH 8: RECURSIVE\n");
	base_conversion_rc(123456, 8);
	printf("\nCONVERTING: 123456 WITH 8: ITERATIVE\n");
	base_conversion_it(123456, 8);

	printf("\nCONVERTING: 123456 WITH 12: RECURSIVE\n");
	base_conversion_rc(123456, 12);
	printf("\nCONVERTING: 123456 WITH 12: ITERATIVE\n");
	base_conversion_it(123456, 12);

	printf("\nCONVERTING: 123456 WITH 16: RECURSIVE\n");
	base_conversion_rc(123456, 16);
	printf("\nCONVERTING: 123456 WITH 16: ITERATIVE\n");
	base_conversion_it(123456, 16);

	printf("\nCONVERTING: 123456 WITH 17: RECURSIVE\n");
	base_conversion_rc(123456, 17);
	printf("\nCONVERTING: 123456 WITH 17: ITERATIVE\n");
	base_conversion_it(123456, 17);

	printf("\nCONVERTING: 123456 WITH 1: RECURSIVE\n");
	base_conversion_rc(123456, 1);
	printf("\nCONVERTING: 123456 WITH 1: ITERATIVE\n");
	base_conversion_it(123456, 1);

	printf("\nCONVERTING: 0 WITH 16: RECURSIVE\n");
	base_conversion_rc(1000000000000, 16);
	printf("\nCONVERTING: 0 WITH 16: ITERATIVE\n");
	base_conversion_it(1000000000000, 16);
}