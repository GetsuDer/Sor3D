#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "omp.h"
#define  Max(a,b) ((a)>(b)?(a):(b))

#define  N   ((1 << 6) +2)
double   maxeps = 0.1e-7;
int itmax = 100;
int i,j,k;
double eps;

double A [N][N][N];

void relax();
void init();
void verify(); 

int main(int an, char **as)
{
    (void) an;
    (void) as;
    int it;
    double time_begin = omp_get_wtime();
	init();

	for(it=1; it<=itmax; it++)
	{
		eps = 0.;
		relax();
		printf( "it=%4i   eps=%f\n", it,eps);
		if (eps < maxeps) break;
	}

	verify();
    double time_end = omp_get_wtime();
    fprintf(stderr, "time = %lf\n", time_end - time_begin);
	return 0;
}


void init()
{ 

	for(k=0; k<=N-1; k++)
	for(j=0; j<=N-1; j++)
	for(i=0; i<=N-1; i++)
	{
		if(i==0 || i==N-1 || j==0 || j==N-1 || k==0 || k==N-1)
		A[i][j][k]= 0.;
		else A[i][j][k]= ( 4. + i + j + k) ;
	}
} 


void relax()
{

	for(k=1; k<=N-2; k++)
	for(j=1; j<=N-2; j++)
	for(i=1; i<=N-2; i++)
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
	for(k=0; k<=N-1; k++)
	for(j=0; j<=N-1; j++)
	for(i=0; i<=N-1; i++)
	{
		s=s+A[i][j][k]*(i+1)*(j+1)*(k+1)/(N*N*N);
	}
	printf("  S = %f\n",s);

}
