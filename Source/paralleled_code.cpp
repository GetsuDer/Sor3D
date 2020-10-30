#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "omp.h"

#define  Max(a,b) ((a)>(b)?(a):(b))

#define  N   (2*2*2*2*2*2+2)
double   maxeps = 0.1e-7;
int itmax = 100;
double eps;
int i,j,k;
double A [N][N][N];

void relax();
void init();
void verify(); 

int main(int an, char **as)
{
    (void) an;
    (void) as;
    int it;

	init();

	for(it=1; it<=itmax; it++)
	{
		eps = 0.;
		relax();
		printf( "it=%4i   eps=%f\n", it,eps);
		if (eps < maxeps) break;
	}

	verify();

	return 0;
}


void init_part(double matrix[N][N], int size, int i_it)
{
    for (int j_it = 0; j_it < size; j_it++) {
        for (int k_it = 0; k_it < size; k_it++) {
            if (i_it == 0 || i_it == N - 1 || j_it == 0 || j_it == N - 1 || k_it == 0 || k_it == N - 1) {
                matrix[j_it][k_it] = 0.;
            } else {
                matrix[j_it][k_it] = (4. + i_it + j_it + k_it);
            }
        }
    }
}

void init()
{
#pragma omp parallel
    {
#pragma omp for 
    for (int i_it = 0; i_it < N; i_it++) {
        init_part(A[i_it], N, i_it);
    }
    }
}

void relax()
{

	for(i=1; i<=N-2; i++)
	for(j=1; j<=N-2; j++)
	for(k=1; k<=N-2; k++)
	{ 
		float e;
		e=A[i][j][k];
		A[i][j][k]=(A[i-1][j][k]+A[i+1][j][k]+A[i][j-1][k]+A[i][j+1][k]+A[i][j][k-1]+A[i][j][k+1])/6.;
		eps=Max(eps, fabs(e-A[i][j][k]));
	}    
}


void verify()
{ 
	float s;

	s=0.;
	for(i=0; i<=N-1; i++)
	for(j=0; j<=N-1; j++)
	for(k=0; k<=N-1; k++)
	{
		s=s+A[i][j][k]*(i+1)*(j+1)*(k+1)/(N*N*N);
	}
	printf("  S = %f\n",s);

}
