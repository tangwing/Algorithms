#include <stdio.h>
#include <stdlib.h>
#include "NEH.h"

void print(int *res, int n)
{
    int i;
    for(i=0;i<n;i++)
	{
	    printf("%d, ",res[i]);
	}
	printf("\n");
}
int main()
{
    int M[3][4];
    M[0][0] = 9;
    M[0][1] = 3;
    M[0][2] = 1;
    M[0][3] = 1;
    M[1][0] = 2;
    M[1][1] = 6;
    M[1][2] = 1;
    M[1][3] = 6;
    M[2][0] = 9;
    M[2][1] = 2;
    M[2][2] = 1;
    M[2][3] = 2;

    int *res=NEH(M, 3, 4);
	printf("\n\nFinale, on obtient Cm= %d , le sequence est :  ", res[4]);
	int i;
	print(res, 4);
    printf("Hello world!\n");
    return 0;
}
