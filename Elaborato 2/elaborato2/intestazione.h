#include <math.h>
#define  DISCRIMINANTE(a,b,c) (pow((b),2)-4*(a)*(c))
#define  NUM_OF_ROOTS(a,b,c) ((DISCRIMINANTE(a,b,c) > 0) ? 2 : (DISCRIMINANTE(a,b,c) == 0) ? 1 : 0)
#define  ROOT1(a,b,c) (((a) > 0) ? (-(b)+sqrt(DISCRIMINANTE(a,b,c)))/(2*(a)) : (-(b)-sqrt(DISCRIMINANTE(a,b,c)))/(2*(a)))
#define  ROOT2(a,b,c) (((a) > 0) ? (-(b)-sqrt(DISCRIMINANTE(a,b,c)))/(2*(a)) : (-(b)+sqrt(DISCRIMINANTE(a,b,c)))/(2*(a)))
#define  EXTREME_POINT(a,b,c) ((-(b))/(2*(a)))
#define  MAXIMUM_POINT(a,b,c) ((2*(a) > 0) ? 0 : 1)