#include <cuda.h>
#include <iostream>
#include <chrono>


using namespace std;

__global__ void kernel(uint8_t* image, long* dest, int N){
    int start_idx = blockIdx.x * N;
    dest[start_idx] = image[start_idx];
    for (size_t i = 1; i < N; i++){
        dest[start_idx + i] = dest[start_idx + i - 1] + image[start_idx + i];
    }
    __syncthreads();
    start_idx = blockIdx.x;
    for (size_t i = 1; i < N; i++){
        dest[i * N + start_idx] = dest[(i - 1) * N + start_idx] + dest[i * N + start_idx];
    }
}

int main(int argc, char const *argv[]){
    int N = 25600;
    uint8_t* image = new uint8_t[N * N];
    long* output = new long[N * N];
    for (size_t i = 0; i < N * N; i++){
        image[i] = 1;
        output[i] = 0;
    }
    auto start = chrono::high_resolution_clock::now();
    uint8_t* image_dev;
    long* out_dev;
    cudaMalloc((void**)&image_dev, sizeof(uint8_t) * N * N);
    cudaMemcpy(image_dev, image, sizeof(uint8_t) * N * N, cudaMemcpyHostToDevice);
    cudaMalloc((void**)&out_dev, sizeof(long) * N * N);
    cudaMemcpy(out_dev, output, sizeof(long) * N * N, cudaMemcpyHostToDevice);

    int numberOfBlocks = N;
    int threadsPerBlock = 1;
    auto kernel_start = chrono::high_resolution_clock::now();
    kernel<<<numberOfBlocks, threadsPerBlock>>>(image_dev, out_dev, N);
    auto kernel_end = chrono::high_resolution_clock::now();
    auto kernel_duration = chrono::duration_cast<chrono::microseconds>(kernel_end - kernel_start);
    cout << "Calculated in " << kernel_duration.count() << " microseconds." << endl;

    cudaMemcpy(output, out_dev, sizeof(long) * N * N, cudaMemcpyDeviceToHost);
    cudaFree(out_dev);
    cudaFree(image_dev);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "Integral image in " << duration.count() << " microseconds." << endl;
    // for (size_t i = 0; i < N; i++){
    //     for (size_t j = 0; j < N; j++){
    //         cout << output[i* N + j] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;

    return 0;
}
