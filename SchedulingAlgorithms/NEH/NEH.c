#include "NEH.h"
#include <limits.h>
int * NEH(int **M, int m, int n)
{
        int * sum = (int*)calloc(n, sizeof(int));
        //        int sum[n];
        int * seq = (int*)calloc(n, sizeof(int));
        int * res = (int*)calloc(n+1, sizeof(int));
        int i = 0,j = 0;
        //Calculer "total processing times"
        for(; i<n; i++)
        {
            for(j=0; j<m; j++ )
            {
                sum[i] += (int)(*(M+j*n+i));//M[j][i];
            }
        }

        //print(sum, 4);
        //Trier la séquence
        int tmp = sum[0];
        seq[0]=0;
        for( i=0; i<n; i++)
        {
            for(j=0; j<n; j++)
            {
                if(sum[j]>tmp)
                {
                    tmp = sum[j];
                    seq[i] = j;
                }
            }
            sum[seq[i]]=0;
            tmp = 0;
        }
        //print(seq, 4);
        //Maintenant on a la séquence triée
        int *resDup = (int*)calloc(n, sizeof(int));
        int pos;//La position de l'insersion au final
        int cmax;
        for( i=0; i<n; i++)
        {
            cmax=INT_MAX;
            //j est la position de l'insersion
            for(j=0; j<=i; j++)
            {
                memcpy(resDup, res, n*sizeof(int));
                print(resDup, 4);
                printf("j=%d\n\n",j);
                //Insérer job i à la position j de res
                insert(resDup, i, j, seq[i]);
                //print(resDup, 4);
                tmp=calculateCmax(M, m, n, resDup, i+1);//i+1 est le nombre de job
                if(tmp<cmax)
                {
                    cmax=tmp;
                    pos=j;
                }
            }
            insert(res,i ,pos, seq[i]);
        }
        free(sum);
        free(seq);
        free(resDup);
        res[n]=cmax;
        return res;
}

//Insérer job j à la position p de res
void insert(int* joblist, int length, int p, int j)
{
    int i;

    printf("%d--%d--%d\n", length, p, j);
    for(i=length-1; i>=p; i--)
    {
        joblist[i+1]=joblist[i];
    }
    joblist[p]=j;
    print(joblist, 4);
printf("--------------\n");
}

//Calculer le cmax de joblist
int calculateCmax(int **M, int m, int n, int* joblist, int jobcount)
{
    int *subM = (int*)malloc(m*jobcount*sizeof(int));
    int i,j;
    for(i=0; i<m; i++)
    {
        for(j=0; j<jobcount; j++)
        {
            if(i==0 && j==0)
                *(subM+i*jobcount+j)= (int)(*(M+i*n+joblist[j]));
            else if(i==0)
            {
                *(subM+i*jobcount+j)= *(subM+i*jobcount+j-1) + (int)(*(M+i*n+joblist[j]));
            }
            else if(j==0)
            {
                *(subM+i*jobcount+j)= *(subM+(i-1)*jobcount+j)+ (int)(*(M+i*n+joblist[j]));
            }
            else
            {
                if(*(subM+i*jobcount+j-1)>*(subM+(i-1)*jobcount+j))
                {
                    *(subM+i*jobcount+j)= *(subM+i*jobcount+j-1)+ (int)(*(M+i*n+joblist[j]));
                }
                else
                {
                    *(subM+i*jobcount+j)= *(subM+(i-1)*jobcount+j)+ (int)(*(M+i*n+joblist[j]));
                }
            }
        }
    }
    return *(subM+m*jobcount-1);

}
