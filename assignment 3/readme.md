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

# Speedup Analysis

Matrix Size Tested: **1000 × 1000**

Speedup is calculated as:

Speedup(p) = T(1) / T(p)

where:
- T(1) = execution time with 1 thread
- T(p) = execution time with p threads

Baseline time (1 thread): **0.480 sec**

| Threads | Time (sec) | Speedup | Efficiency |
|----------|------------|----------|-------------|
| 1 | 0.480 | 1.00× | 100% |
| 2 | 0.378 | 1.27× | 63% |
| 4 | 0.248 | 1.94× | 48% |
| 8 | 0.156 | 3.07× | 38% |

---

## 🔍 Observations

- Speedup increases as the number of threads increases.
- Maximum observed speedup is **3.07×** with 8 threads.
- Efficiency decreases at higher thread counts due to:
  - Memory bandwidth limitations
  - Thread scheduling overhead
  - Cache contention
- The workload (1000 × 1000) is moderately sized, so parallel overhead has noticeable impact.
