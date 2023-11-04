#include<iostream>
#include<chrono>

using namespace std;

template <typename T>
void measure_run_time(T* obj){
    auto s = chrono::high_resolution_clock::now();
    (*obj)();
    auto f = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(f - s);
    cout << (*obj).getClass() << " run time " << duration.count() << " microseconds." << endl;
}