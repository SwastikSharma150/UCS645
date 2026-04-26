#include <cuda_runtime.h>
#include <stdio.h>

#define N 1024

__global__ void differentTasks(int *input, long long *output, int n) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;

    // Thread 0: iterative sum
    if (tid == 0) {
        long long sum = 0;
        for (int i = 1; i <= n; i++)
            sum += i;
        output[0] = sum;
    }
    // Thread 1: formula sum
    else if (tid == 1) {
        output[1] = (long long)n * (n + 1) / 2;
    }
    // Other threads: parallel work
    else if (tid < n) {
        output[tid] = input[tid] * 2;  // simple parallel task
    }
}

int main() {
    int *h_input = (int*)malloc(N * sizeof(int));
    long long *h_output = (long long*)malloc(N * sizeof(long long));

    for (int i = 0; i < N; i++)
        h_input[i] = i + 1;

    int *d_input;
    long long *d_output;

    cudaMalloc(&d_input, N * sizeof(int));
    cudaMalloc(&d_output, N * sizeof(long long));

    cudaMemcpy(d_input, h_input, N * sizeof(int), cudaMemcpyHostToDevice);

    differentTasks<<<1, N>>>(d_input, d_output, N);
    cudaDeviceSynchronize();

    cudaMemcpy(h_output, d_output, N * sizeof(long long), cudaMemcpyDeviceToHost);

    printf("Thread 0 Sum: %lld\n", h_output[0]);
    printf("Thread 1 Sum: %lld\n", h_output[1]);

    cudaFree(d_input);
    cudaFree(d_output);
    free(h_input);
    free(h_output);

    return 0;
}