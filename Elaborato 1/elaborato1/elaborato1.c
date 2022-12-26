/*
19 Settembre 2022
Programmazione B
Ingegneria e Scienze Informatiche - Cesena
A.A. 2022-2023
Elaborato 1
Data di sottomissione: entro le ore 20 del 25 Settembre 2022
Formato di sottomissione: un file compresso con nome elaborato1.zip
contenente un unico file sorgente con nome elaborato1.c
Specifiche:
• Scrivere un programma C che dichiari una variabile per ogni possibile
combinazione di specificatori e modificatori di tipo visti a lezione
(eccetto il tipo void).
• Inizializzare ogni variabile con un valore appartenente al range di valori
possibili per il tipo con cui è stata dichiarata.
• Per ogni variabile dichiarata, usare la printf() per stampare: il tipo
della variabile, il nome della variabile, il suo valore attuale, il valore
minimo e massimo ammissibile per il tipo della variabile e l’occupazione
di memoria (in byte) di tale tipo.
• Stampare una sola variabile per riga.
Vincoli:
• E’ necessario utilizzare correttamente gli specificatori di formato della
printf() per ogni tipo di dato considerato.
• E’ necessario utilizzare il costrutto sizeof per poter calcolare l’occupazione
di memoria dei tipi di dato.
• E’ necessario utilizzare i valori limite per tipo di dato specificati nei file
header limits.h e float.h.
Esempio di stampa per il tipo di dato char:
TYPE: char NAME: c VALUE: x MIN: -128 MAX: 127 BYTE: 1
*/

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <limits.h>

int main()
{
    // Definizione e inizializzazione di variabili.
    char a = 'a';
    signed char b = 'b';
    unsigned char c = 'c';
    signed short int d = -1;
    unsigned short int e = 1;
    int f = 22;
    unsigned int g = 0;
    signed long int h = -2;
    unsigned long int j = 2;
    float k = 3.1256f;
    double l = 7.1658298;
    long double m = 3.14926493L;

    // Stampa.
    printf(" TYPE: %-21s NAME: %-7s VALUE: %-11c MIN: %-15d MAX: %-15d BYTES: %lu\n", "char", "a", a, CHAR_MIN, CHAR_MAX, sizeof(char));
    printf(" TYPE: %-21s NAME: %-7s VALUE: %-11c MIN: %-15d MAX: %-15d BYTES: %lu\n", "signed char", "b", b, SCHAR_MIN, SCHAR_MAX, sizeof(signed char));
    printf(" TYPE: %-21s NAME: %-7s VALUE: %-11c MIN: %-15s MAX: %-15d BYTES: %lu\n", "unsigned char", "c", c, "0", UCHAR_MAX, sizeof(unsigned char));
    printf(" TYPE: %-21s NAME: %-7s VALUE: %-11i MIN: %-15d MAX: %-15d BYTES: %lu\n", "signed short int", "d", d, SHRT_MIN, SHRT_MAX, sizeof(signed short int));
    printf(" TYPE: %-21s NAME: %-7s VALUE: %-11u MIN: %-15s MAX: %-15d BYTES: %lu\n", "unsigned short int", "e", e, "0", USHRT_MAX, sizeof(unsigned short int));
    printf(" TYPE: %-21s NAME: %-7s VALUE: %-11i MIN: %-15d MAX: %-15d BYTES: %lu\n", "int", "f", f, INT_MIN, INT_MAX, sizeof(int));
    printf(" TYPE: %-21s NAME: %-7s VALUE: %-11u MIN: %-15s MAX: %-15u BYTES: %lu\n", "unsigned int", "g", g, "0", UINT_MAX, sizeof(unsigned int));
    printf(" TYPE: %-21s NAME: %-7s VALUE: %-11i MIN: %-15d MAX: %-15d BYTES: %lu\n", "signed long int", "h", h, LONG_MIN, LONG_MAX, sizeof(signed long int));
    printf(" TYPE: %-21s NAME: %-7s VALUE: %-11u MIN: %-15s MAX: %-15u BYTES: %lu\n", "unsigned long int", "j", j, "0", ULONG_MAX, sizeof(unsigned long int));
    printf(" TYPE: %-21s NAME: %-7s VALUE: %-11.4f MIN: %-15e MAX: %-15e BYTES: %lu\n", "float", "k", k, FLT_MIN, FLT_MAX, sizeof(float));
    printf(" TYPE: %-21s NAME: %-7s VALUE: %-11.7lf MIN: %-15e MAX: %-15e BYTES: %lu\n", "double", "l", l, DBL_MIN, DBL_MAX, sizeof(double));
    printf(" TYPE: %-21s NAME: %-7s VALUE: %-11.9lf MIN: %-15le MAX: %-15le BYTES: %lu\n", "long double", "m", m, LDBL_MIN, LDBL_MAX, sizeof(long double));

    return 0;
}
