#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include<thread>
#include<atomic>
#include <mutex>

using namespace std;

class SingleThreadCalculator{
    private:
        int* histogram;
        cv::Mat image;
    public:
        SingleThreadCalculator(cv::Mat image);
        int* operator()();
        string getClass(){
            return "SingleThreadCalculator";
        }
};

class AtomicCalculator{
    private:
        atomic<int> histogram[256];
        int n_threads;
        cv::Mat image;
        void calculate(int i);
    public:
        atomic<int>* operator()();
        AtomicCalculator(int n_threads, cv::Mat image);
        string getClass(){
            return "AtomicCalculator";
        }
};

class LockedCalculator{
    private:
        int* histogram;
        int n_threads;
        cv::Mat image;
        void calculate(int i);
        mutex l;
    public:
        int* operator()();
        LockedCalculator(int n_threads, cv::Mat image);
        string getClass(){
            return "LockedCalculator";
        }
};

class MultiThreadCalculator{
    private:
        int* histogram;
        cv::Mat image;
        int** thread_hists;
        cv::Mat* slices;
        int n_threads;
        void calculate(int i);
        mutex l;
    public:
        int* operator()();
        MultiThreadCalculator(int n_threads, cv::Mat image);
         string getClass(){
            return "MultiThreadCalculator";
        }
};