#include<iostream>
#include<thread>
#include<mutex>
#include<atomic>

using namespace std;

class STMatMul{
    private:
        int n;
        int m;
        int l;
        float** A;
        float** B;
        float** C;
    public:
        STMatMul(float** A, float** B, int n, int m, int l);
        float** operator()();  
        string getClass(){
            return "STMatMul";
        }
};

class MTMatMul{
    private:
        int n_threads;
        int n;
        int m;
        int l;
        float** A;
        float** B;
        float** C;
        void partial_mul(int t_idx);
    public:
        MTMatMul(float** A, float** B, int n, int m, int l, int n_threads);
        float** operator()();
        string getClass(){
            return "MTMatMul";
        }
};
