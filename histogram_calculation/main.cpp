#include <iostream>
#include "HistogramCalculators.hpp"
using namespace std;
template <typename T>
void measure_run_time(T* obj);

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
    
    // atomic<int>* a = (*ac)();
    // int* b = (*sc)();
    // int* c = (*mc)();
    // int* d = (*lc)();
    // for (size_t i = 0; i < 256; i++){
    //     if(a[i] != b[i] ||  a[i] != c[i] || a[i] != d[i])
    //         cout << "FAILLURE" << endl;
    // }
    
    return 0;
}

template <typename T>
void measure_run_time(T* obj){
    auto s = chrono::high_resolution_clock::now();
    (*obj)();
    auto f = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(f - s);
    cout << (*obj).getClass() << " run time " << duration.count() << " microseconds." << endl;

}