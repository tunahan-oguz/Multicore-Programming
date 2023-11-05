#include <cuda.h>
#include <iostream>
using namespace std;
// MAX THREADS ALLOWED PER BLOCK IS 1024 IN MY SYSTEM
#define THRDS 1024

__global__ void vecAdd(float* A, float* B, float* C, int N){
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if(i < N)
        C[i] = A[i] + B[i];
}

int main(int argc, char const *argv[]){
    int N = atoi(argv[1]);
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

    int numberOfBlocks = (N + THRDS - 1) / THRDS;
    int threadsPerBlock = THRDS;

    vecAdd<<<numberOfBlocks, threadsPerBlock>>>(cu_A, cu_B, cu_C, N);

    cudaMemcpy(C, cu_C, sizeof(float) * N, cudaMemcpyDeviceToHost);
    cudaFree(cu_C);
    cudaFree(cu_A);
    cudaFree(cu_B);
    
    // for (size_t i = 0; i < N; i++){
    //     cout << C[i]<< " ";
    // }
    // cout << endl;

    return 0;
}
