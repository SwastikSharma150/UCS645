# Parallel Correlation Matrix Computation

## Problem Statement:

Given a matrix of size ny × nx containing m input vectors (rows),  
compute the correlation coefficient between every pair of input vectors.

For all **0 ≤ j ≤ i < ny**, compute:

   `correlation(row_i, row_j)`

Store the result in the lower triangular matrix location:

   `result[i + j * ny]`

All arithmetic operations are performed using double precision.

---

## Implementations

Three versions are implemented:

| Version      | Description                                           |
|--------------|-------------------------------------------------------|
| Sequential   | Baseline single-threaded implementation               |
| OpenMP       | Multi-threaded parallel implementation                |
| Fast         | OpenMP + SIMD + -O3 compiler optimizations            |

---

## Compilation
`make`

---

## Execution

`./correlate <ny> <nx>`

---

# Performance Evaluation

**Matrix Size Tested:** 1000 × 1000

## Execution Time Comparison

| Threads | Execution Time (sec) | Speedup | Efficiency |
|----------|----------------------|----------|-------------|
| 1 | 0.480 | 1.00× | 100% |
| 2 | 0.378 | 1.27× | 63% |
| 4 | 0.248 | 1.94× | 48% |
| 8 | 0.156 | 3.07× | 38% |

## Speedup Analysis

Speedup = T(1) / T(n)

| Threads | Speedup |
|----------|----------|
| 2 | 1.27× |
| 8 | 3.07× |

## Observations

- Parallel execution significantly reduces execution time.
- Speedup increases with number of threads.
- Efficiency decreases at higher thread counts due to parallel overhead and memory bandwidth limitations.
- Larger matrices show better scaling efficiency.
