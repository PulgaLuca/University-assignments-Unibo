/*NB: test x=0*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>


//#define EVAL //UNCOMMENT for testing but REMOVE from submission

int main() {
    unsigned int x;
    unsigned int res1 = 0, res2 = 0, res3 = 0;

    //INPUT: x must be the input value, do not overwrite it 
    printf("Enter a positive integer: "); scanf("%u", &x);
    //calculate here
    int resto = 0, count = 0, x1 = 0;
    x1 = x;

    // Controllo se l'input è uguale a 0.
    if (x1 == 0) {
        res1 = 0;
        res2 = 10;
        res3 = 1;
    }
    else {
        // Numero al contrario.
        while (x1 > 0) {
            resto = x1 % 10;
            res1 = res1 * 10 + resto;
            x1 = (x1 - resto) / 10;
            count++;
        }

        // Complemento a 10.
        res2 = pow(10, count) - (x);

        // Numero di 0.
        x1 = x;
        resto = 0;
        do {
            resto = x1 % 10;
            if (resto == 0) res3++;
            x1 = x1 / 10;
        } while (x1 > 0);
    }

    //do NOT add code after this point
    //OUTPUT
    printf("Actual result:\n");
    printf("%u %u %u\n", res1, res2, res3);

    //DON'T REMOVE from submittion
#ifdef EVAL
    eval(x, res1, res2, res3);
#endif
    return;
}