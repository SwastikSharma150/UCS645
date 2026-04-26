#include <cuda_runtime.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1024

// CPU merge
void merge(int *arr, int l, int m, int r) {
    int i=l, j=m+1, k=0;
    int *temp = (int*)malloc((r-l+1)*sizeof(int));

    while(i<=m && j<=r)
        temp[k++] = (arr[i] < arr[j]) ? arr[i++] : arr[j++];
    while(i<=m) temp[k++] = arr[i++];
    while(j<=r) temp[k++] = arr[j++];

    for(int x=0; x<k; x++)
        arr[l+x] = temp[x];

    free(temp);
}

void mergeSort(int *arr, int l, int r) {
    if(l < r) {
        int m = (l+r)/2;
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
        merge(arr, l, m, r);
    }
}

// GPU kernel
__global__ void mergeKernel(int *arr, int *temp, int width, int n) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    int left = tid * 2 * width;

    if (left >= n) return;

    int mid = min(left + width - 1, n - 1);
    int right = min(left + 2*width - 1, n - 1);

    int i = left, j = mid + 1, k = left;

    while (i <= mid && j <= right)
        temp[k++] = (arr[i] < arr[j]) ? arr[i++] : arr[j++];
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];

    for (int x = left; x <= right; x++)
        arr[x] = temp[x];
}

int main() {
    int *h_arr = (int*)malloc(N*sizeof(int));
    for(int i=0;i<N;i++) h_arr[i] = rand()%1000;

    int *d_arr, *d_temp;
    cudaMalloc(&d_arr, N*sizeof(int));
    cudaMalloc(&d_temp, N*sizeof(int));

    cudaMemcpy(d_arr, h_arr, N*sizeof(int), cudaMemcpyHostToDevice);

    for (int width = 1; width < N; width *= 2) {
        int blocks = (N + 2*width - 1) / (2*width);
        mergeKernel<<<blocks, 1>>>(d_arr, d_temp, width, N);
        cudaDeviceSynchronize();
    }

    cudaMemcpy(h_arr, d_arr, N*sizeof(int), cudaMemcpyDeviceToHost);

    printf("Sorted Output (first 10):\n");
    for(int i=0;i<10;i++) printf("%d ", h_arr[i]);
    printf("\n");

    cudaFree(d_arr);
    cudaFree(d_temp);
    free(h_arr);

    return 0;
}