#include <iostream>
#include <thread>
#include <numeric>
#include <vector>
#include <cmath>

using namespace std;

void partialSum(vector<double>::iterator start, vector<double>::iterator end, double* res)
{
    *res = 0;
    for (auto i = start; i < end; i++)
        *res += *i;
}

int main(int argc, char** argv)
{
    int numThr = atoi(argv[1]);
    int N = atoi(argv[2]);
    int step = (int)ceil(N * 1.0 / numThr);
    double* res = new double[numThr];

    vector<double> data(N);
    iota(data.begin(), data.end(), 1);

    thread** thr = new thread* [numThr];
    vector<double>::iterator localStart = data.begin();
    vector<double>::iterator localEnd;

    //THREAD CREATION AND THREAD - WORK MAPPING HAPPENS HERE
    for (int i = 0; i < numThr; i++)
    {
        localEnd = localStart + step;
        if (i == numThr - 1) 
            localEnd = data.end();
        
        //CREATE NEW THREAD AND ASSIGN WORK
        thr[i] = new thread(partialSum, localStart, localEnd, res + i);
        
        localStart += step;
    }

    double total = 0;
    for (int i = 0; i < numThr; i++)
    {
        //MAKE SURE ALL THREADS FINISH
        thr[i]->join();

        delete thr[i];
        
        //ADD PARTIAL RESULTS
        total += res[i];
    }

    cout << "Average is : " << total / N << endl;

    //CLEAN UP
    delete[] thr;
    delete[] res;
    return 0;
}