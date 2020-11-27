#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#define  Max(a,b) ((a)>(b)?(a):(b))

#define  N   ((1 << 6) +2)
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
    
    double time_start = MPI_Wtime();    
    
    int part_size = (N - 2) / size;
    int ind_first = part_size * rank, ind_last = ind_first + part_size;
    if (rank == size - 1) { //last process
        ind_last = N - 2;
    } 

    double *matrix = (double *)calloc(N * N * (ind_last - ind_first), sizeof(double));
    
    int ind1, ind2, ind3;
    for (ind1 = 0; ind1 < ind_last - ind_first; ind1++) {
        for (ind2 = 0; ind2 <= N - 1; ind2++) {
            for (ind3 = 0; ind3 <= N - 1; ind3++) {
                if (ind2 == 0 || ind3 == 0 || ind3 == N - 1 || ind2 == N - 1 ) {
                    *(matrix + ind1 * N * N + ind2 * N + ind3) = 0;
                } else {
                    *(matrix + ind1 * N * N + ind2 * N + ind3) = 4. + (ind_first + ind1 + 1) + ind2 + ind3; 
                }
            }
        }
    }

    MPI_Status status;
//matrix initialized
    double *prev_values = (double *)calloc(N * N, sizeof(double)); //values from other processes
    double *next_values = (double *)calloc(N * N, sizeof(double)); //values form other processes... almost
    if (rank != size - 1) { //for last process next_values is always zero submatrix
        for (ind2 = 0; ind2 <= N - 1; ind2++) {
            for (ind3 = 0; ind3 <= N - 1; ind3++) {
                if (ind2 == 0 || ind3 == 0 || ind3 == N - 1 || ind2 == N - 1 ) {
                    *(next_values + ind2 * N + ind3) = 0;
                } else {
                    *(next_values + ind2 * N + ind3) = 4. + ind2 + ind3 + ind_last - ind_first;
                }
            }
        }
    }

    int it;
    for (it = 0; it < itmax; it++) {
    // get valuesi
        if (rank) { // process zero does not recieves data from other processes here
            //on first iteration left values could be counted by process itself
            MPI_Recv(prev_values, N * N, MPI_DOUBLE, rank - 1, it, MPI_COMM_WORLD, &status);
        }
        if (it && rank != size - 1) { // on first iteration next_values is already correct,
            //for last process next_values is ALWAYS correct
            MPI_Recv(next_values, N * N, MPI_DOUBLE, rank + 1, it - 1, MPI_COMM_WORLD, &status);
        }
        // process values
        eps = 0;
        // even if rank = 0, prev_values is correct
        for (ind2 = 1; ind2 < N - 1; ind2++) {
            for (ind3 = 1; ind3 < N - 1; ind3++) {
                double old = *(matrix + ind2 * N + ind3);
                *(matrix + ind2 * N + ind3) = (*(prev_values + ind2 * N + ind3) + 
                    *(matrix + (ind2 - 1) * N + ind3) + *(matrix + (ind2 + 1) * N + ind3) +
                    *(matrix + ind2 * N + ind3 - 1) + *(matrix + ind2 * N + ind3 + 1) + 
                    *(matrix + N * N + ind2 * N + ind3)) / 6.;
                eps = Max(eps, fabs(old - *(matrix + ind2 * N + ind3)));
            }
        }

        for (ind1 = 1; ind1 < ind_last - ind_first - 1; ind1++) {
            for (ind2 = 1; ind2 < N - 1; ind2++) {
                for (ind3 = 1; ind3 < N - 1; ind3++) {
                    double old = *(matrix + ind1 * N * N + ind2 * N + ind3);            
                    *(matrix + ind1 * N * N + ind2 * N + ind3) = (*(matrix + (ind1 - 1) * N * N + ind2 * N + ind3) + 
                        *(matrix + ind1 * N * N + (ind2 - 1) * N + ind3) + *(matrix + ind1 * N * N + (ind2 + 1) * N + ind3) +
                        *(matrix + ind1 * N * N + ind2 * N + ind3 - 1) + *(matrix + ind1 * N * N + ind2 * N + ind3 + 1) + 
                        *(matrix + (ind1 + 1) * N * N + ind2 * N + ind3)) / 6.;
                    eps = Max(eps, fabs(old - *(matrix + ind1 * N * N + ind2 * N + ind3)));
                }
            }
        }
    
        for (ind2 = 1; ind2 < N - 1; ind2++) {
            for (ind3 = 1; ind3 < N - 1; ind3++) {
                double old = *(matrix + (ind_last - ind_first - 1) * N * N + ind2 * N + ind3);
                *(matrix + (ind_last - ind_first - 1) * N * N + ind2 * N + ind3) = (*(matrix + (ind_last - ind_first - 2) * N * N + ind2 * N + ind3) +
                    *(matrix + (ind_last - ind_first - 1) * N * N + (ind2 - 1) * N + ind3) + *(matrix + (ind_last - ind_first - 1) * N * N + (ind2 + 1) * N + ind3) +
                    *(matrix + (ind_last - ind_first - 1) * N * N + ind2 * N + ind3 - 1) + *(matrix + (ind_last - ind_first - 1) * N * N + ind2 * N + ind3 + 1) +
                    *(next_values + ind2 * N + ind3)) / 6.;
                eps = Max(eps, fabs(old - *(matrix + (ind_last - ind_first - 1) * N * N + ind2 * N + ind3)));
            }
        }
      //  if (eps < maxeps) {
// stop work for all proccess... Wait, no. It is another epsilon, isn`t it?
// So I should have epsilon for each iteration and send their parts between processes...
// And somehow restore matrix values? Or may be i can just forget about this.
      //  }
    // send values
    //
        if (rank != size - 1) {
            MPI_Send(matrix + (ind_last - ind_first - 1) * N * N, N * N, MPI_DOUBLE, rank + 1, it, MPI_COMM_WORLD);
        }
        if (rank && it != itmax - 1) {
            MPI_Send(matrix, N * N, MPI_DOUBLE, rank - 1, it, MPI_COMM_WORLD);
        }
    }
//now we should sum all counted values and sent them to process 0
//process 0 should sum all recieved values and print them
    
    double S = 0.;
    for (ind1 = 0; ind1 < ind_last - ind_first; ind1++) {
        for (ind2 = 1; ind2 < N - 1; ind2++) {
            for (ind3 = 1; ind3 < N - 1; ind3++) {
                S += *(matrix + ind1 * N * N + ind2 * N + ind3) * (ind3 + 1) * (ind2 + 1) * (ind1 + ind_first + 2) / (N * N * N);
            }
        }
    }
    if (rank) {
        MPI_Send(&S, 1, MPI_DOUBLE, 0, itmax + 1, MPI_COMM_WORLD);
    }

    if (!rank) {
        double S_tmp;
        for (ind1 = 1; ind1 < size; ind1++) {
            MPI_Recv(&S_tmp, 1, MPI_DOUBLE, ind1, itmax + 1, MPI_COMM_WORLD, &status);
            S += S_tmp;
        }
        fprintf(stderr, "Result = %lf\n", S);
        fflush(stderr);
    }
    if (!rank) {
        double time_end = MPI_Wtime();
        fprintf(stderr, "time = %lf\n", time_end - time_start);
    }
    MPI_Finalize();
	return 0;
}
