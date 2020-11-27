#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#define  Max(a,b) ((a)>(b)?(a):(b))

#define  N   ((1 << 2) +2)
double   maxeps = 0.1e-7;
int itmax = 100;
double eps;

int main(int argc, char **argv)
{

    MPI_Init(&argc, &argv);
    int rank;
    int size;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    
    int part_size = N / size;
    int ind_first = part_size * rank, ind_last = ind_first + part_size;
    if (rank == size - 1) {
        ind_last = N;
    } 

    fprintf(stderr, "Process %d works with layers [%d-%d)\n", rank, ind_first, ind_last);

    double *matrix = (double *)calloc(N * N * (ind_last - ind_first), sizeof(double));
    int ind1, ind2, ind3;
    for (ind1 = 0; ind1 < ind_last - ind_first; ind1++) {
        for (ind2 = 0; ind2 <= N - 1; ind2++) {
            for (ind3 = 0; ind3 <= N - 1; ind3++) {
                if (ind_first + ind1 == 0 || ind2 == 0 || ind3 == 0 || ind3 == N - 1 
                        || ind2 == N - 1 || ind_first + ind1 == N - 1) {
                    *(matrix + ind1 * N * N + ind2 * N + ind3) = 0;
                } else {
                    *(matrix + ind1 * N * N + ind2 * N + ind3) = 4. + (ind_first + ind1) + ind2 + ind3; 
                }
            }
        }
    }
    MPI_Finalize();
	return 0;
}
