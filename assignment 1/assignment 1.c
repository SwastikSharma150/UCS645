/*
 * UCS645 - LAB 1: OpenMP Basics
 * Compile: gcc solution.c -fopenmp -O2 -lm -o solution
 * Run:     ./solution
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#define N_DAXPY  (1 << 16)
#define N_MATRIX 1000
#define PI_STEPS 100000000L

static double A[N_MATRIX][N_MATRIX];
static double B[N_MATRIX][N_MATRIX];
static double C[N_MATRIX][N_MATRIX];

/* ── Q1: DAXPY ── */
void run_daxpy() {
    printf("\n===== Q1: DAXPY LOOP =====\n");
    printf("%-10s %-15s %-10s\n", "Threads", "Time (s)", "Speedup");

    double a = 2.5, baseline = 0.0;
    int tc[] = {1, 2, 4, 8};

    for (int t = 0; t < 4; t++) {
        double *X = (double *)malloc(N_DAXPY * sizeof(double));
        double *Y = (double *)malloc(N_DAXPY * sizeof(double));

        for (int i = 0; i < N_DAXPY; i++) { X[i] = i; Y[i] = N_DAXPY - i; }

        omp_set_num_threads(tc[t]);
        double start = omp_get_wtime();

        #pragma omp parallel for schedule(static)
        for (int i = 0; i < N_DAXPY; i++)
            X[i] = a * X[i] + Y[i];

        double elapsed = omp_get_wtime() - start;
        if (tc[t] == 1) baseline = elapsed;

        printf("%-10d %-15.6f %-10.2fx\n", tc[t], elapsed, baseline / elapsed);
        free(X); free(Y);
    }
}

/* ── Q2: Matrix Multiply ── */
void init_matrices() {
    for (int i = 0; i < N_MATRIX; i++)
        for (int j = 0; j < N_MATRIX; j++) {
            A[i][j] = (double)(i + j + 1) / N_MATRIX;
            B[i][j] = (double)(i - j + 1) / N_MATRIX;
            C[i][j] = 0.0;
        }
}

void run_matrix_multiply() {
    printf("\n===== Q2: MATRIX MULTIPLICATION (%dx%d) =====\n", N_MATRIX, N_MATRIX);
    printf("%-10s %-15s %-10s %-15s %-10s\n",
           "Threads", "1D Time(s)", "1D Spdup", "2D Time(s)", "2D Spdup");

    int tc[] = {1, 2, 4, 8};
    double base1D = 0.0, base2D = 0.0;

    for (int t = 0; t < 4; t++) {
        omp_set_num_threads(tc[t]);

        /* Version 1: 1D - parallelise outer i-loop only */
        init_matrices();
        double start = omp_get_wtime();
        #pragma omp parallel for schedule(static)
        for (int i = 0; i < N_MATRIX; i++)
            for (int j = 0; j < N_MATRIX; j++) {
                double tmp = 0.0;
                for (int k = 0; k < N_MATRIX; k++) tmp += A[i][k] * B[k][j];
                C[i][j] = tmp;
            }
        double t1 = omp_get_wtime() - start;

        /* Version 2: 2D - collapse i and j loops */
        init_matrices();
        start = omp_get_wtime();
        #pragma omp parallel for collapse(2) schedule(static)
        for (int i = 0; i < N_MATRIX; i++)
            for (int j = 0; j < N_MATRIX; j++) {
                double tmp = 0.0;
                for (int k = 0; k < N_MATRIX; k++) tmp += A[i][k] * B[k][j];
                C[i][j] = tmp;
            }
        double t2 = omp_get_wtime() - start;

        if (tc[t] == 1) { base1D = t1; base2D = t2; }
        printf("%-10d %-15.4f %-10.2fx %-15.4f %-10.2fx\n",
               tc[t], t1, base1D / t1, t2, base2D / t2);
    }
}

/* ── Q3: PI Calculation ── */
void run_pi() {
    printf("\n===== Q3: CALCULATION OF PI =====\n");
    printf("%-10s %-20s %-12s %-10s\n", "Threads", "PI Approx", "Error", "Time (s)");

    double step = 1.0 / (double)PI_STEPS;
    int tc[] = {1, 2, 4, 8};

    for (int t = 0; t < 4; t++) {
        omp_set_num_threads(tc[t]);
        double sum = 0.0;
        double start = omp_get_wtime();

        #pragma omp parallel for reduction(+:sum) schedule(static)
        for (long i = 0; i < PI_STEPS; i++) {
            double x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
        }

        double pi = step * sum;
        printf("%-10d %-20.15f %-12.2e %-10.4f\n",
               tc[t], pi, fabs(pi - M_PI), omp_get_wtime() - start);
    }
}

int main() {
    run_daxpy();
    run_matrix_multiply();
    run_pi();
    return 0;
}