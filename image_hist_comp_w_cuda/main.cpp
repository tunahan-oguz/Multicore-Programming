#include "./../histogram_calculation/HistogramCalculators.hpp"
#include "Hist.h"
#include "./../Report.hpp"

int main(int argc, char const *argv[]){
    int max_th = thread::hardware_concurrency();
    cv::Mat image = cv::imread("./../cpp_qs/lena_grayscale_hq.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat resizedImage;
    cv::resize(image, resizedImage, cv::Size(image.rows * 100, image.cols * 100));
    uint8_t* imageptr = resizedImage.data;
    SingleThreadCalculator* sc = new SingleThreadCalculator(resizedImage);
    MultiThreadCalculator* mc = new MultiThreadCalculator(max_th, resizedImage);
    CudaCalculator* cc = new CudaCalculator(imageptr, resizedImage.rows, resizedImage.cols);
    measure_run_time(sc);
    measure_run_time(mc);
    measure_run_time(cc);
    
    return 0;
}
