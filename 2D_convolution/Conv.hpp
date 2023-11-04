#include<iostream>
#include<thread>
#include<mutex>
#include<atomic>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;

class STConv{
    private:
        cv::Mat image;
        int kernel_sz;
        int padding;
        cv::Mat weights;
        cv::Mat padded;
        cv::Mat res;
    public:
        STConv(cv::Mat image, cv::Mat weights, int kernel_sz);
        cv::Mat operator()();  
        string getClass(){
            return "STConv";
        }
};

class MTConv{
    private:
        int n_threads;
        cv::Mat image;
        int kernel_sz;
        int padding;
        cv::Mat weights;
        cv::Mat padded;
        cv::Mat res;
        mutex l;
        void partial_conv(int t_idx);
    public:
        MTConv(cv::Mat image, cv::Mat weights, int kernel_sz, int n_threads);
        cv::Mat operator()();
        string getClass(){
            return "MTConv";
        }
};
