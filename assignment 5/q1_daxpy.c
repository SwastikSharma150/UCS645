#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N (1 << 16)

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int chunk = N / size;
    double a = 2.5;

    double *X = (double*)malloc(chunk * sizeof(double));
    double *Y = (double*)malloc(chunk * sizeof(double));

    for (int i = 0; i < chunk; i++) {
        int global_i = rank * chunk + i;
        X[i] = (double)global_i;
        Y[i] = (double)(N - global_i);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    for (int i = 0; i < chunk; i++)
        X[i] = a * X[i] + Y[i];

    double elapsed = MPI_Wtime() - start;

    double max_time;
    MPI_Reduce(&elapsed, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0)
        printf("DAXPY | Processes = %d | Time = %.6f seconds\n", size, max_time);

    free(X);
    free(Y);
    MPI_Finalize();
    return 0;
}
