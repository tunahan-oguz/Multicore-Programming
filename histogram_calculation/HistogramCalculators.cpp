#include "HistogramCalculators.hpp"

SingleThreadCalculator::SingleThreadCalculator(cv::Mat image){
    this->image = image;
    this->histogram = new int[256];
    for (size_t i = 0; i < 256; i++){
        this->histogram[i] = 0;
    }
            
};

int* SingleThreadCalculator::operator()(){
    for (size_t i = 0; i < this->image.rows; i++){
        for (size_t j = 0; j < this->image.cols; j++){
            this->histogram[image.at<uint8_t>(i, j)]++;
        }
    }
    return this->histogram;
}

MultiThreadCalculator::MultiThreadCalculator(int n_threads, cv::Mat image){
    this->histogram = (int*) malloc(sizeof(int) * 256);
    this->thread_hists = new int*[n_threads];
    this->n_threads = n_threads;
    this->image = image;
    // initialize the histogram buffers
    for(int i = 0; i < n_threads; i++){
        thread_hists[i] = new int[256];
        for (size_t j = 0; j < 256; j++){
            thread_hists[i][j] = 0;
        }
    }
    for(int i = 0; i < 256; i++){
        histogram[i] = 0;
    }
}

int* MultiThreadCalculator::operator()(){
    // just create threads and spawn them on calculate, join them and merge the solutions in one histogram
    thread** threads = new thread*[this->n_threads];
    for(auto i = 0; i < this->n_threads; i++){
        *(threads + i) = new thread(&MultiThreadCalculator::calculate, this, i);
    }
    for(auto i = 0; i < this->n_threads; i++){
        threads[i]->join();
    }
    for (auto i = 0; i < this->n_threads; i++){
        for(int j = 0; j < 256; j++){
            this->histogram[j] += this->thread_hists[i][j];
        }
    }
    return this->histogram;
}

void MultiThreadCalculator::calculate(int i){
    int slice_height = (this->image.rows / this->n_threads);
    int start = i * slice_height;
    int stop = (i + 1) * slice_height;
    if (i + 1 == this->n_threads)
        stop = this->image.rows;
    for (size_t r = start; r < stop; r++){
        for (size_t c = 0; c < this->image.cols; c++){
            this->thread_hists[i][this->image.at<uchar>(r, c)]++;
        }
    }
}

AtomicCalculator::AtomicCalculator(int n_threads, cv::Mat image){
    this->image = image;
    this->n_threads = n_threads;
    for (size_t i = 0; i < 256; i++){
        this->histogram[i].store(0);
    }
}

atomic<int>* AtomicCalculator::operator()(){
    thread** threads = new thread*[this->n_threads];
    for (size_t i = 0; i < this->n_threads; i++){
        threads[i] = new thread(&AtomicCalculator::calculate, this, i);
        threads[i]->join();
    }
    // for (size_t i = 0; i < this->n_threads; i++){
    // }
    return this->histogram;
}

void AtomicCalculator::calculate(int i){
    int slice_height = (this->image.rows / this->n_threads);
    int start = i * slice_height;
    int stop = (i + 1) * slice_height;
    if (i + 1 == this->n_threads)
        stop = this->image.rows;
    for (size_t i = start; i < stop; i++){
        for (size_t j = 0; j < this->image.cols; j++){
            this->histogram[static_cast<int>(this->image.at<uchar>(i, j))]++;
        }
        
    }
}

LockedCalculator::LockedCalculator(int n_threads, cv::Mat image){
    this->image = image;
    this->n_threads = n_threads;
    this->histogram = new int[256];
    for (size_t i = 0; i < 256; i++){
        this->histogram[i] = 0;
    }
}

int* LockedCalculator::operator()(){
    thread** threads = new thread*[this->n_threads];
    for (size_t i = 0; i < this->n_threads; i++){
        threads[i] = new thread(&LockedCalculator::calculate, this, i);
    }
    for (size_t i = 0; i < this->n_threads; i++){
        threads[i]->join();
    }
    return this->histogram;
}

void LockedCalculator::calculate(int i){
    int slice_height = (this->image.rows / this->n_threads);
    int start = i * slice_height;
    int stop = (i + 1) * slice_height;
    if (i + 1 == this->n_threads)
        stop = this->image.rows;
    for (size_t i = start; i < stop; i++){
        for (size_t j = 0; j < this->image.cols; j++){
            this->l.lock();
            this->histogram[(this->image.at<uchar>(i, j))]++;
            this->l.unlock();
        }
        
    }
}