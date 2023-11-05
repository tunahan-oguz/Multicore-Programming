#include <cuda.h>
#include <iostream>
#include "mat_mul/MatMul.hpp"
#include "Report.hpp"

using namespace std;

#define TILE_WIDTH 128


__global__ void matMul(float* A, float* B, float* C, int w){
    int col = blockDim.x * blockIdx.x + threadIdx.x;
    int row = blockDim.y * blockIdx.y + threadIdx.y;

    float sum = 0;
    for (size_t i = 0; i < w; i++){
        sum += A[row * w + i] * B[i * w + col];
    }
    C[row * w + col] = sum;
}


int main(int argc, char const *argv[]){
    int matrix_W = 4096 * 4;
    int size = matrix_W * matrix_W; 
    float* Mat1 = new float[size]; 
    float* Mat2 = new float[size]; 
    float* res = new float[size]; 
    for (size_t i = 0; i < matrix_W; i++){
        for (size_t j = 0; j < matrix_W; j++){
            Mat1[i* matrix_W + j] = 1;
            Mat2[i* matrix_W + j] = 1;
            
        }
    }
    
    // for (size_t i = 0; i < matrix_W; i++){
    //     for (size_t j = 0; j < matrix_W; j++){
    //         cout << Mat1[i* matrix_W + j] << " ";            
    //     }
    //     cout << endl;
    // }
    

    float* A;
    float* B;
    float* C;

    auto alloc_start = chrono::high_resolution_clock::now();
    cudaMalloc((void**)&A, sizeof(float) * size);
    cudaMalloc((void**)&B, sizeof(float) * size);
    cudaMalloc((void**)&C, sizeof(float) * size);
    auto alloc_finish = chrono::high_resolution_clock::now();
    cudaMemcpy(A, Mat1, sizeof(float) * size, cudaMemcpyHostToDevice);
    cudaMemcpy(B, Mat2, sizeof(float) * size, cudaMemcpyHostToDevice);
    cudaMemset(C, 0, sizeof(float) * size);
    auto cpy_finish = chrono::high_resolution_clock::now();

    dim3 gridDim(matrix_W / TILE_WIDTH, matrix_W / TILE_WIDTH);
    dim3 blockDim(TILE_WIDTH, TILE_WIDTH);
    
    auto mul_start = chrono::high_resolution_clock::now();
    matMul<<<gridDim, blockDim>>>(A, B, C, matrix_W);
    auto mul_finish = chrono::high_resolution_clock::now();    
    cudaMemcpy(res, C, sizeof(float) * size, cudaMemcpyDeviceToHost);
    auto copy_host_finish = chrono::high_resolution_clock::now();

    cudaFree(C);
    cudaFree(A);
    cudaFree(B);
    auto free_finish = chrono::high_resolution_clock::now();

    auto alloc_duration = chrono::duration_cast<chrono::microseconds>(alloc_finish - alloc_start);
    auto copy_host2device_duration = chrono::duration_cast<chrono::microseconds>(cpy_finish - alloc_finish);
    auto procces_dur = chrono::duration_cast<chrono::microseconds>(mul_finish - mul_start);
    auto copy_back = chrono::duration_cast<chrono::microseconds>(copy_host_finish - mul_finish);
    auto total = chrono::duration_cast<chrono::microseconds>(free_finish - alloc_start);

    cout<< "Total process "<< total.count() << endl;
    cout<< "Allocation "<< alloc_duration.count() << endl;
    cout<< "Copy to device "<< copy_host2device_duration.count() << endl;
    cout<< "Multiplication "<< procces_dur.count() << endl;
    cout<< "copy_back "<< copy_back.count() << endl;




    // for (size_t i = 0; i < matrix_W; i++){
    //     for (size_t j = 0; j < matrix_W; j++){
    //         cout << res[i* matrix_W + j] << " ";            
    //     }
    //     cout << endl;
    // }

    return 0;
}
