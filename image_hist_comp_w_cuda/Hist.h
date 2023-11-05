#include "cuda.h"


class CudaCalculator{
    private:
        uint8_t* image;
        int* histogram;
        int* d_histogram;
        int y;
        int x;
    public:
        CudaCalculator(uint8_t* image, int y, int x);
        int* operator()();
        char* getClass(){
            return "CudaCalculator";
        }
};
