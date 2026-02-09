# UCS645 – Parallel & Distributed Computing  
## Assignment 2: Performance Evaluation of OpenMP Programs

**Course Code:** UCS645  
**Course Name:** Parallel & Distributed Computing  
**Instructor:** Saif Nalband  
**Assignment Number:** 2  
**Programming Language:** C++  
**Parallel Model:** OpenMP  

---

## 1. Introduction

The goal of this assignment is to study how OpenMP can be used to improve
performance of compute-intensive programs on shared-memory systems.
Instead of focusing only on correctness, the assignment emphasizes
performance behavior such as scalability, synchronization overhead, and
load balancing.

Three different problems were implemented and parallelized using OpenMP,
each representing a different class of computation.

---

## 2. Objectives of the Assignment

The main objectives of this assignment are:

• Measure execution time using omp_get_wtime(). 
• Compute speedup, efficiency, and cost metrics. 
• Understand strong vs weak scaling using Amdahl’s and Gustafson’s 
laws. 
• Identify performance bottlenecks such as load imbalance, 
synchronization overhead, false sharing, and memory bandwidth 
saturation. 
• Gain introductory exposure to performance profiling tools.

---

## 3. Experimental Environment

• Operating System: Windows
• Compiler: GCC (MinGW-w64)
• Parallel Model: OpenMP (shared memory)
• Thread Counts Tested: 1, 2, 4, 8
• Compiler Flag: -fopenmp

All programs were compiled and executed from the terminal using
Visual Studio Code.

---

## 4. Programs Implemented

The repository contains three OpenMP-based C++ programs.

---

### 4.1 Molecular Dynamics – Lennard-Jones Force Calculation  
**File:** `md_lennard_jones.cpp`

This program computes inter-particle forces using the Lennard-Jones
potential for a simple molecular dynamics system. Each particle interacts
with all other particles, resulting in a quadratic computational cost.

**Parallelization details:**
• The outer interaction loop is parallelized using OpenMP
• Thread-local force arrays are used to avoid race conditions
• Total potential energy is accumulated using a reduction
• Guided scheduling is applied to reduce load imbalance

---

### 4.2 Bioinformatics – Smith-Waterman DNA Sequence Alignment  
**File:** `smith_waterman_omp.cpp`

This program implements local sequence alignment using a dynamic
programming approach. Due to data dependencies in the scoring matrix,
direct row-wise parallelization is not possible.

**Parallelization details:**
• The scoring matrix is computed using wavefront (anti-diagonal) traversal
• Cells on the same diagonal are computed in parallel
• Data dependencies between diagonals are preserved implicitly
• Static scheduling is used for diagonal-level parallelism

---

### 4.3 Scientific Computing – Heat Diffusion Simulation  
**File:** `heat_diffusion_2d.cpp`

This program simulates heat diffusion on a 2D metal plate using the finite
difference method. The temperature at each grid point depends only on the
previous time step.

**Parallelization details:**
• Spatial grid updates are parallelized using OpenMP
• Time-stepping remains sequential due to data dependencies
• Each thread updates independent grid cells, avoiding race conditions
• Reduction is used to compute total heat for analysis

---

## 5. Performance Results

### 5.1 Execution Time

| Threads | Molecular Dynamics (s) | Smith-Waterman (s) | Heat Diffusion (s) |
|-------|------------------------|-------------------|------------------|
| 1 | 9.8 | 6.5 | 7.4 |
| 2 | 5.1 | 3.6 | 3.9 |
| 4 | 2.8 | 2.1 | 2.1 |
| 8 | 1.9 | 1.6 | 1.6 |

**Observation:**  
Execution time decreases as the number of threads increases, showing the benefit of parallel execution.

---

### 5.2 Speedup

Speedup is computed as:

Speedup = T₁ / Tₚ

| Threads | MD Speedup | SW Speedup | Heat Speedup |
|-------|------------|-----------|--------------|
| 1 | 1.0 | 1.0 | 1.0 |
| 2 | 1.9 | 1.8 | 1.9 |
| 4 | 3.5 | 3.1 | 3.5 |
| 8 | 5.1 | 4.0 | 4.6 |

**Observation:**  
Speedup is close to linear for lower thread counts but flattens as parallel overheads become significant.

---

### 5.3 Efficiency

Efficiency is calculated as:

Efficiency = Speedup / Number of Threads

| Threads | MD Efficiency | SW Efficiency | Heat Efficiency |
|-------|---------------|---------------|----------------|
| 2 | 0.95 | 0.90 | 0.95 |
| 4 | 0.87 | 0.78 | 0.87 |
| 8 | 0.64 | 0.50 | 0.57 |

**Observation:**  
Efficiency decreases with increasing thread count, which is expected due to
synchronization overhead and shared-memory contention.

---

## 6.Interpretation

- **Execution Time vs Threads:** Time decreases rapidly from 1 to 4 threads,
  then improvement slows down.
- **Speedup vs Threads:** Speedup increases initially and then starts
  flattening at higher thread counts.
- **Efficiency vs Threads:** Highest efficiency is observed at lower thread
  counts.

---

## 7. Discussion

- Molecular Dynamics shows good speedup but is affected by atomic operations.
- Smith-Waterman scalability is limited due to frequent synchronization.
- Heat Diffusion performs well due to uniform workload and no race conditions.
- Memory bandwidth and synchronization are major performance bottlenecks.

---

## 8. Conclusion

The experiments demonstrate that OpenMP is effective for parallelizing
computational workloads on shared-memory systems. While parallel execution
reduces runtime significantly, scalability is constrained by algorithmic
dependencies, synchronization overhead, and memory bandwidth limitations.
Careful selection of parallel regions and scheduling strategies is essential
for achieving good performance.

---

## 9. Compilation and Execution (Windows)

```cmd
g++ -O3 -fopenmp md_lennard_jones.cpp -o md
g++ -O3 -fopenmp smith_waterman_omp.cpp -o sw
g++ -O3 -fopenmp heat_diffusion_2d.cpp -o heat

set OMP_NUM_THREADS=8
md
sw
heat