#include <iostream>
#include <thread>
#include <numeric>
#include <vector>
#include <cmath>
#include <functional>
#include <memory>

using namespace std;

//STRUCT FUNCTOR
struct PartialSumFunctor
{
    vector < double >::iterator start;
    vector < double >::iterator end;
    double res = 0;

    void operator()();
};

//() OPERATOR OVERLOAD
void PartialSumFunctor::operator()()
{
    for (auto i = start; i < end; i++)
        res += *i;
}

int main(int argc, char** argv)
{
    int numThr = atoi(argv[1]);
    int N = atoi(argv[2]);
    int step = (int)ceil(N * 1.0 / numThr);

    vector < double >data(N);
    iota(data.begin(), data.end(), 1);
    vector < double >::iterator localStart = data.begin();
    vector < double >::iterator localEnd;

    //unique_ptr < thread > thr[numThr - 1];
    auto thr = unique_ptr<unique_ptr<thread>[]>(new unique_ptr<thread>[numThr - 1]);

    PartialSumFunctor* f = new PartialSumFunctor[numThr];

    //THREAD CREATION AND WROK ASSIGNMENT
    for (int i = 0; i < numThr - 1; i++)
    {
        localEnd = localStart + step;
        f[i].start = localStart;
        f[i].end = localEnd;
        
        //APPLY THE OVERLOADED () OPERATOR AND PASS BY REF - ref()
        thr[i] = make_unique<thread>(ref(f[i]));
        
        localStart += step;
    }
    //USE THE MAIN THREAD TO HANDLE THE "LAST" CHUNK
    f[numThr - 1].start = localStart;
    f[numThr - 1].end = data.end();
    f[numThr - 1]();

    //WE DO NOT NEED TO JOIN THE MAIN THREAD. WE -ARE- THE MAIN THREAD...
    double total = f[numThr - 1].res;

    for (int i = 0; i < numThr - 1; i++)
    {
        //MAKE SURE ALL THREADS FINISH
        thr[i]->join();

        //ADD PARTIAL RESULTS
        total += f[i].res;

        //WE DO NOT NEED TO DELETE[] THR BECAUSE OF THE SMART POINTER USE...
    }
    cout << "Average is : " << total / N << endl;

    //CLEAN UP
    delete[] f;

    return 0;
}