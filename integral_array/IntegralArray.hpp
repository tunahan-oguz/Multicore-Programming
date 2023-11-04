#include<iostream>
#include<thread>
#include<mutex>
#include<atomic>
#include<vector>
#include <numeric>
#include <condition_variable>

using namespace std;

class STIntegralArray{
    private:
    vector<long>* integral;
    vector<int>* input;
    public:
        STIntegralArray(vector<int>* list);
        vector<long>* operator()();
        string getClass(){
            return "STIntegralArray";
        }
};

class MTIntegralArray{
    private:
        vector<long>* integral;
        vector<int>* input;
        void partial_count(int start, int end, int i);
        void merge_partial(int end, int inc, int i);
        int n;
        int step;
        long* critical_elements;
    public:
        MTIntegralArray(vector<int>* list, int n_threads);
        vector<long>* operator()();
        string getClass(){
            return "MTIntegralArray";
        }
};