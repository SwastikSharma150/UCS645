#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int value;
    int next_rank = (rank + 1) % size;
    int prev_rank = (rank - 1 + size) % size;

    if (rank == 0) {
        value = 100;
        printf("Process 0 starts with value: %d\n", value);
        MPI_Send(&value, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD);
        MPI_Recv(&value, 1, MPI_INT, prev_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 0 received final value: %d\n", value);
    } else {
        MPI_Recv(&value, 1, MPI_INT, prev_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        value += rank;
        printf("Process %d received value, added rank, now: %d\n", rank, value);
        MPI_Send(&value, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}