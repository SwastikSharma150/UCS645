#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    srand(time(NULL) + rank);

    int local_max = 0, local_min = 1001;
    printf("Process %d numbers: ", rank);
    for (int i = 0; i < 10; i++) {
        int val = rand() % 1001;
        printf("%d ", val);
        if (val > local_max) local_max = val;
        if (val < local_min) local_min = val;
    }
    printf("\n");

    struct { int val; int rank; } max_loc = {local_max, rank};
    struct { int val; int rank; } min_loc = {local_min, rank};
    struct { int val; int rank; } global_max, global_min;

    MPI_Reduce(&max_loc, &global_max, 1, MPI_2INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);
    MPI_Reduce(&min_loc, &global_min, 1, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Global max = %d (from process %d)\n", global_max.val, global_max.rank);
        printf("Global min = %d (from process %d)\n", global_min.val, global_min.rank);
    }

    MPI_Finalize();
    return 0;
}