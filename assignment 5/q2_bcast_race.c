#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 10000000

void my_bcast(double *buf, int rank, int size) {
    if (rank == 0) {
        for (int i = 1; i < size; i++)
            MPI_Send(buf, ARRAY_SIZE, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
    } else {
        MPI_Recv(buf, ARRAY_SIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double *buf = (double*)malloc(ARRAY_SIZE * sizeof(double));

    if (rank == 0)
        for (int i = 0; i < ARRAY_SIZE; i++) buf[i] = (double)i;

    /* Part A: MyBcast (linear for-loop) */
    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();
    my_bcast(buf, rank, size);
    double t_mybcast = MPI_Wtime() - start;

    double max_mybcast;
    MPI_Reduce(&t_mybcast, &max_mybcast, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    /* Part B: MPI_Bcast */
    if (rank == 0)
        for (int i = 0; i < ARRAY_SIZE; i++) buf[i] = (double)i;

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();
    MPI_Bcast(buf, ARRAY_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    double t_bcast = MPI_Wtime() - start;

    double max_bcast;
    MPI_Reduce(&t_bcast, &max_bcast, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Processes = %d\n", size);
        printf("MyBcast time  = %.4f seconds\n", max_mybcast);
        printf("MPI_Bcast time = %.4f seconds\n", max_bcast);
        printf("Speedup = %.2fx\n", max_mybcast / max_bcast);
    }

    free(buf);
    MPI_Finalize();
    return 0;
}
