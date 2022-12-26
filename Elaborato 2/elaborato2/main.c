#include "intestazione.h"
#include <stdio.h>

int main() {

	float a = 1.0, b = 0.0, c = -4.0
		;

#ifdef NUM_OF_ROOTS(a, b, c)
	printf("NUM_OF_ROOTS(a, b, c): %d\n", NUM_OF_ROOTS(a, b, c));
#endif

#ifdef ROOT1(a, b, c)
	printf("ROOT1(a, b, c): %0.2f\n", ROOT1(a, b, c));
#endif

#ifdef ROOT2(a, b, c)
	printf("ROOT2(a, b, c): %0.2f\n", ROOT2(a, b, c));
#endif

#ifdef EXTREME_POINT(a,b,c)
	printf("EXTREME_POINT(a,b,c): %0.2f\n", EXTREME_POINT(a, b, c));
#endif

#ifdef MAXIMUM_POINT(a,b,c)
	printf("MAXIMUM_POINT(a,b,c): %i\n", MAXIMUM_POINT(a, b, c));
#endif
}