#include "Hist.h"

using namespace std;

__global__ void kernel(uint8_t* image, int* d_hist, int im_size);


CudaCalculator::CudaCalculator(uint8_t* image, int y, int x){
    this->image = image;
    this->x = x;
    this->y = y;
    this->histogram = new int[256];
}

int* CudaCalculator::operator()(){
    // allocate device memory
    uint8_t* cuda_image;
    cudaMalloc((void**)&cuda_image, x * y * sizeof(uint8_t));
    cudaMalloc((void**)&d_histogram, 256 * sizeof(int));
    cudaMemcpy(cuda_image, image, x * y * sizeof(uint8_t), cudaMemcpyHostToDevice);
    cudaMemset(d_histogram, 0, 256 * sizeof(int));
    //launch kernel
    int threadsPerBlock = 256;
    int numberOfBlocks = (x * y + threadsPerBlock - 1) / threadsPerBlock;
    kernel<<<numberOfBlocks, threadsPerBlock>>>(cuda_image, d_histogram, x * y);
    // copy results to host
    cudaMemcpy(histogram, d_histogram, 256 * sizeof(int), cudaMemcpyDeviceToHost);
    // free device mem
    cudaFree(cuda_image);
    cudaFree(d_histogram);
    return this->histogram;
}

__global__ void kernel(uint8_t* image, int* d_hist, int im_size){
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid < im_size) {
        atomicAdd(&d_hist[image[tid]], 1);
    }
}