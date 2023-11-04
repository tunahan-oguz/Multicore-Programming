#include <iostream>
#include "HistogramCalculators.hpp"
#include "../Report.hpp"

using namespace std;
int main(int argc, char const *argv[]){
    unsigned int max_threads = thread::hardware_concurrency();;
    cv::Mat image = cv::imread("./../cpp_qs/lena_grayscale_hq.jpg", cv::IMREAD_GRAYSCALE);
    SingleThreadCalculator* sc = new SingleThreadCalculator(image);
    MultiThreadCalculator* mc = new MultiThreadCalculator(max_threads, image);
    AtomicCalculator* ac = new AtomicCalculator(max_threads, image);
    LockedCalculator* lc = new LockedCalculator(max_threads, image);
    cout << max_threads << " threads will be used..." << endl;
    measure_run_time<SingleThreadCalculator>(sc);
    measure_run_time<MultiThreadCalculator>(mc);
    measure_run_time<AtomicCalculator>(ac);
    measure_run_time<LockedCalculator>(lc);
    return 0;
}