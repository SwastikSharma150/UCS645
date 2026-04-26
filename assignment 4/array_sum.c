#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 100;
    int chunk = n / size;

    int *array = NULL;
    if (rank == 0) {
        array = (int *)malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) array[i] = i + 1;
    }

    int *local = (int *)malloc(chunk * sizeof(int));
    MPI_Scatter(array, chunk, MPI_INT, local, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    int local_sum = 0;
    for (int i = 0; i < chunk; i++) local_sum += local[i];
    printf("Process %d local sum: %d\n", rank, local_sum);

    int global_sum = 0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Global sum = %d (expected 5050)\n", global_sum);
        printf("Average    = %.2f\n", (double)global_sum / n);
        free(array);
    }

    free(local);
    MPI_Finalize();
    return 0;
}