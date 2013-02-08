#include <stdlib.h>
//Insérer job i à la position j de res
void insert(int* joblist, int length, int p, int j);
//Calculer le cmax de joblist
int calculateCmax(int ** M, int m, int n, int* joblist, int jobcount);
int * NEH(int **M, int m, int n);
