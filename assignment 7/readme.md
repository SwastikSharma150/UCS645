# Assignment 7 – CUDA Programming (Part II)

## Student Information

* **Name:** Swastik Sharma
* **Roll Number:** 102497025
* **Course:** Parallel and Distributed Computing (UCS645)
* **Execution Platform:** Google Colab (Tesla T4 GPU)
* **CUDA Toolkit:** 12.x

---

## 📌 Objective

This assignment explores advanced CUDA concepts by implementing:

* Task-level divergence using threads
* Comparison of CPU vs GPU sorting
* Memory bandwidth analysis using vector addition

---


## ▶️ Execution Steps

Compile and run using:

```bash id="r1x2a9"
nvcc problem1.cu -o run1 && ./run1
nvcc problem2.cu   -o run2 && ./run2
nvcc -O3 problem3.cu -o run3 && ./run3
```

---

# 🔹 Problem 1 – Thread-Level Task Allocation

## Concept

CUDA allows threads within the same kernel to perform **different operations** based on their index. This is achieved through conditional branching.

## Implementation Logic

* Thread 0 → computes sum using iteration
* Thread 1 → computes sum using mathematical formula
* Remaining threads → perform independent parallel operations

## Observation

Both computation methods yield identical results, validating correctness.

## Key Insight

Although threads execute different instructions, they still follow the SIMT model. Divergence may impact performance but is useful for demonstrating flexibility.

---

# 🔹 Problem 2 – Merge Sort (CPU vs GPU)

## Approach

### CPU Version

* Uses recursive divide-and-conquer
* Splits array until single elements
* Merges sorted subarrays

### GPU Version

* Uses iterative bottom-up strategy
* Each kernel handles multiple merge operations in parallel
* Merge width doubles every iteration

---

## Execution Behavior

* For small input sizes, CPU performs better
* GPU execution includes:

  * Kernel launch overhead
  * Device memory operations

---

## Performance Discussion

| Method | Behavior                              |
| ------ | ------------------------------------- |
| CPU    | Faster for small datasets             |
| GPU    | Better scalability for large datasets |

## Reasoning

* Small arrays fit in CPU cache → faster access
* GPU requires overhead to initiate parallel execution
* Efficiency improves as problem size increases

---

# 🔹 Problem 3 – Vector Addition & Bandwidth Analysis

## Objective

To evaluate GPU memory performance by comparing theoretical and measured bandwidth.

---

## Kernel Description

Each thread computes:

```text
C[i] = A[i] + B[i]
```

---

## Configuration

| Parameter         | Value              |
| ----------------- | ------------------ |
| Vector Size       | 1,048,576 elements |
| Threads per Block | 256                |
| Total Blocks      | 4096               |

---

## Bandwidth Calculation

### Theoretical Bandwidth

Based on hardware specifications:

* Memory clock rate
* Bus width
* Double data rate (DDR) factor

---

### Measured Bandwidth

Computed using:

```text
Bandwidth = Total Bytes Transferred / Execution Time
```

---

## Observations

* Measured bandwidth is significantly lower than theoretical peak
* Efficiency is limited by:

  * Memory latency
  * Kernel launch overhead
  * Non-ideal access patterns

---

## Interpretation

Achieving peak bandwidth is rare in real applications. Actual performance depends on workload size and memory behavior.

---

# 📊 Summary of Results

| Problem | Description           | Outcome                          |
| ------- | --------------------- | -------------------------------- |
| P1      | Thread task variation | Correct execution                |
| P2      | Sorting comparison    | CPU faster (small N)             |
| P3      | Vector addition       | Successful + bandwidth evaluated |

---

# 🧠 Key Learnings

* CUDA enables fine-grained control over thread behavior
* GPU acceleration depends heavily on **problem size**
* Memory performance plays a crucial role in CUDA programs
* Theoretical hardware limits are rarely achieved in practice
* Efficient GPU usage requires balancing computation and memory access

---

# ✅ Conclusion

This assignment demonstrates how CUDA can be used to solve computational problems in parallel. While GPUs provide significant advantages for large-scale workloads, careful consideration of overhead and memory usage is necessary to achieve optimal performance.

---