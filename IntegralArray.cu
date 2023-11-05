#include <cuda.h>
#include <iostream>


using namespace std;
// MAX numberOfBlocks allowed in my system is 128
#define NBLOCKS 128

__global__ void integral_array(int* src, int* res, int N){
    int numel_per_block = N / blockDim.x;
    int start = blockIdx.x * numel_per_block + threadIdx.x;
    int end = start + numel_per_block;
    if (start < N){
        res[start] = src[start];
        for (size_t i = start + 1; i < end; i++){
            res[i] = res[i - 1] + src[i];
        }
    }
    
}

int main(int argc, char const *argv[]){

    int N = atoi(argv[1]);
    int* A = new int[N];
    int* res = new int[N];

    for (size_t i = 0; i < N; i++){
        A[i] = 1;
        res[i] = 0;
    }
    int* A_device;
    int* res_device;

    cudaMalloc((void**)&A_device, sizeof(int) * N);
    cudaMalloc((void**)&res_device, sizeof(int) * N);
    cudaMemset(res_device, 0, sizeof(int) * N);
    cudaMemcpy(A_device, A, sizeof(int) * N, cudaMemcpyHostToDevice);
    
    int numberOfBlocks = NBLOCKS;
    int threadsPerBlock = (N + NBLOCKS - 1) / NBLOCKS;

    integral_array<<<numberOfBlocks, threadsPerBlock>>>(A_device, res_device, N);

    cudaMemcpy(res, res_device, sizeof(int) * N, cudaMemcpyDeviceToHost);

    cudaFree(res_device);
    cudaFree(A);

    for (size_t i = 0; i < N; i++){
        cout << res[i]<< " ";
    }
    cout << endl;

    return 0;
}
