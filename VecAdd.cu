#include <cuda.h>
#include <iostream>
using namespace std;

__global__ void vecAdd(float* A, float* B, float* C, int N){
    int i = threadIdx.x;
    // if(i < N)
        C[i] = A[i] + B[i];
}

int main(int argc, char const *argv[]){
    int N = 4096;
    float* A = new float[N];
    float* B = new float[N];
    float* C = new float[N];
    for (size_t i = 0; i < N; i++){
        A[i] = 1;
        B[i] = 2;
        C[i] = 0;
    }
    float* cu_A;
    float* cu_B;
    float* cu_C;
    cudaMalloc((void**)&cu_A, N * sizeof(float));
    cudaMalloc((void**)&cu_B, N * sizeof(float));
    cudaMalloc((void**)&cu_C, N * sizeof(float));
    cudaMemcpy(cu_A, A, sizeof(float) * N, cudaMemcpyHostToDevice);
    cudaMemcpy(cu_B, B, sizeof(float) * N, cudaMemcpyHostToDevice);
    cudaMemset(cu_C, 0, sizeof(float) * N);

    int numberOfBlocks = 1;
    int numberOfThreads = N;

    vecAdd<<<numberOfBlocks, numberOfThreads>>>(cu_A, cu_B, cu_C, N);

    cudaMemcpy(C, cu_C, sizeof(float) * N, cudaMemcpyDeviceToHost);
    cudaFree(cu_C);
    cudaFree(cu_A);
    cudaFree(cu_B);
    

    return 0;
}
