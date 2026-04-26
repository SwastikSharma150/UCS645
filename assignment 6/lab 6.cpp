#include <stdio.h>
#include <cuda.h>

__global__ void sumKernel(float *input, float *result, int n) {
    __shared__ float partialSum[256];

    int tid = threadIdx.x;
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    partialSum[tid] = (i < n) ? input[i] : 0;
    __syncthreads();

    // Reduction
    for (int stride = blockDim.x / 2; stride > 0; stride /= 2) {
        if (tid < stride) {
            partialSum[tid] += partialSum[tid + stride];
        }
        __syncthreads();
    }

    if (tid == 0) {
        atomicAdd(result, partialSum[0]);
    }
}

int main() {
    int n = 1024;
    size_t size = n * sizeof(float);

    float *h_input = (float*)malloc(size);
    float h_result = 0;

    for (int i = 0; i < n; i++)
        h_input[i] = 1.0f;

    float *d_input, *d_result;

    // 1. Allocate device memory
    cudaMalloc((void**)&d_input, size);
    cudaMalloc((void**)&d_result, sizeof(float));

    // 2. Copy host to device
    cudaMemcpy(d_input, h_input, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_result, &h_result, sizeof(float), cudaMemcpyHostToDevice);

    // 3. Configure kernel
    int blockSize = 256;
    int gridSize = (n + blockSize - 1) / blockSize;

    // 4. Launch kernel
    sumKernel<<<gridSize, blockSize>>>(d_input, d_result, n);

    // 5. Copy result back
    cudaMemcpy(&h_result, d_result, sizeof(float), cudaMemcpyDeviceToHost);

    printf("Sum = %f\n", h_result);

    // 6. Free memory
    cudaFree(d_input);
    cudaFree(d_result);
    free(h_input);

    return 0;
}