# CUDA Programming Assignment – 6

## Parallel and Distributed Computing (UCS645)

---

## 👤 Student Information

* **Name:** Swastik Sharma
* **Roll Number:** 102497025

---

## 🎯 Aim of the Assignment

The purpose of this assignment is to explore GPU-based parallel computing using CUDA. The work focuses on understanding GPU hardware properties and implementing parallel algorithms for numerical computation.

---

## 🧩 Tasks Covered

This assignment is divided into three main sections:

* **Part A:** Extracting GPU specifications using CUDA
* **Part B:** Computing the sum of a large array using parallel reduction
* **Part C:** Performing matrix addition using 2D parallelization

---

## 🖥️ Execution Environment

* **Platform Used:** Google Colab
* **GPU:** Tesla T4
* **CUDA Support:** Enabled via Colab runtime

---

# 🔍 Part A – GPU Exploration

A CUDA-based device query program was executed to inspect hardware details of the GPU.

### 📊 Observed Specifications

* GPU belongs to the **Turing architecture**
* Compute capability identified as **7.5**
* Warp size is **32 threads**
* Maximum threads per block: **1024**
* Grid dimensions support very large parallel launches
* GPU supports **double precision arithmetic**
* **ECC memory** is enabled for reliability

---

### 💾 Memory Hierarchy (Summary)

| Memory Type     | Description                              |
| --------------- | ---------------------------------------- |
| Global Memory   | Large but high latency memory (≈14.5 GB) |
| Shared Memory   | Fast memory within a block (48 KB)       |
| Constant Memory | Read-only cached memory (64 KB)          |

---

### ✍️ Conceptual Answers

**Maximum Threads Calculation**
If grid size = 65535 and block size = 512:
Total threads = **33,553,920**

---

**Why maximum threads are not always ideal**

* Some threads may remain idle
* Hardware resources (registers/shared memory) are limited
* Performance depends on occupancy, not just thread count

---

**Key Limiting Factors**

* Memory availability
* Register usage
* Shared memory per block
* Hardware scheduling limits

---

# ➕ Part B – Parallel Array Summation

### ⚙️ Method Used

A reduction-based approach was implemented:

* Input array divided across thread blocks
* Each block computes a partial sum using shared memory
* Final result aggregated on the CPU

---

### 📐 Configuration Used

| Parameter         | Value     |
| ----------------- | --------- |
| Total Elements    | 4,194,304 |
| Threads per Block | 256       |
| Number of Blocks  | 16,384    |

---

### 📈 Performance Observation

* CPU execution was faster for this task
* GPU performance affected by:

  * Memory transfer overhead
  * Kernel launch latency
  * Low computational intensity

---

### ⚠️ Precision Note

Due to floating-point arithmetic, small numerical differences were observed between CPU and GPU outputs. This is expected due to differences in operation ordering.

---

# ➗ Part C – Matrix Addition on GPU

### ⚙️ Implementation Strategy

* Two matrices of size **2048 × 2048** were processed
* Each thread computes a single element of the result matrix
* A 2D grid and block structure was used

---

### 📐 Execution Setup

| Parameter         | Value       |
| ----------------- | ----------- |
| Matrix Size       | 2048 × 2048 |
| Threads per Block | 32 × 32     |
| Grid Size         | 64 × 64     |

---

### 📊 Computational Analysis

* Each element requires **1 addition**
* Total operations: **4,194,304**
* Memory reads: **2 per element**
* Memory writes: **1 per element**

---

### 🚀 Performance Insight

Matrix addition benefits from GPU parallelism because:

* Workload is evenly distributed
* Large dataset ensures better GPU utilization
* Parallel threads operate independently

---

# 📊 Overall Performance Comparison

| Task            | CPU vs GPU Observation         |
| --------------- | ------------------------------ |
| Array Sum       | CPU faster (memory-bound)      |
| Matrix Addition | GPU faster (parallel-friendly) |

---

# 🧠 Key Learnings

* GPU performance depends heavily on **problem type**
* Not all problems benefit from parallelization
* Understanding **memory hierarchy** is essential
* Efficient CUDA programs require balancing:

  * Threads
  * Memory usage
  * Execution configuration

---

# 📁 Project Files

* `device_query.cu` – Retrieves GPU specifications
* `array_sum.cu` – Performs parallel reduction
* `matrix_add.cu` – Implements matrix addition
* `Assignment6.ipynb` – Google Colab implementation

---

# ▶️ How to Execute

### Using Google Colab

1. Enable GPU runtime
2. Execute all cells in the notebook

### Using Local CUDA Setup

```bash
nvcc device_query.cu -o dq
./dq

nvcc array_sum.cu -o sum
./sum

nvcc matrix_add.cu -o mat
./mat
```

---

# ✅ Conclusion

This assignment demonstrated how CUDA enables parallel computation on GPUs. While GPUs significantly accelerate structured parallel workloads like matrix operations, they may not always outperform CPUs for memory-bound tasks.

---

## Authors

- [@SwastikSharma150](https://github.com/SwastikSharma150) 