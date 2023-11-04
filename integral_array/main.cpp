#include "IntegralArray.hpp"
#include <iostream>
#include <chrono>
#include "../Report.hpp"

using namespace std;

int main(int argc, char const *argv[]){
    int N = 4001;
    int N_THR = 6;
    vector<int>* input = new vector<int>(N);
    iota(input->begin(), input->end(), 1);

    STIntegralArray* st = new STIntegralArray(input);
    MTIntegralArray* mt = new MTIntegralArray(input, N_THR);
    measure_run_time(st);
    measure_run_time(mt);

     
    return 0;
}
