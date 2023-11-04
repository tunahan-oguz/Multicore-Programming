#include<iostream>
#include<thread>
#include<mutex>
#include<atomic>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
class STIntegralImage{
    private:
        cv::Mat image;
        cv::Mat integral;
    public:
        STIntegralImage(cv::Mat image);
        cv::Mat operator()();
        string getClass(){
            return "STIntegralImage";
        }
};


class MTIntegralImage{
    private:
        cv::Mat image;
        cv::Mat integral;
        cv::Mat critical_rows;
        int n_threads;
        void partial_integral(int thread_idx);
        void merge(int thread_idx);
        mutex l;
    public:
        MTIntegralImage(cv::Mat image, int n_threads);
        cv::Mat operator()();
        string getClass(){
            return "MTIntegralImage";
        }
};