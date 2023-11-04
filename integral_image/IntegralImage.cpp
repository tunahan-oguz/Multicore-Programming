#include "IntegralImage.hpp"

STIntegralImage::STIntegralImage(cv::Mat image){
    this->image = image;
    cv::Mat newImage(image.rows + 1, image.cols + 1, CV_32SC1);
    this->integral = newImage;
    integral.row(0).setTo(0);
    integral.col(0).setTo(0);
}

cv::Mat STIntegralImage::operator()(){
    for (size_t i = 1; i < integral.rows; i++){
        for(int j = 1; j < integral.cols; j++){
            integral.at<int>(i, j) = image.at<uint8_t>(i - 1, j - 1) + integral.at<int>(i - 1, j) 
            + integral.at<int>(i, j - 1) - integral.at<int>(i - 1, j -1);
        }
    }
    return integral;
}

MTIntegralImage::MTIntegralImage(cv::Mat image, int n_threads){
    this->image = image;
    this->n_threads = n_threads;
    cv::Mat newImage(image.rows + 1, image.cols + 1, CV_32SC1);
    this->integral = newImage;
    integral.row(0).setTo(0);
    integral.col(0).setTo(0);
    cv::Mat crows(n_threads - 1, image.cols, CV_32SC1);
    critical_rows = crows;    
}

cv::Mat MTIntegralImage::operator()(){
    // Part the rows
    thread** threads = new thread*[n_threads];
    for (size_t i = 0; i < n_threads; i++){
        *(threads + i) = new thread(&MTIntegralImage::partial_integral, this, i);
    }
    for (size_t i = 0; i < n_threads; i++){
        threads[i]->join();
    }
    // Set critical rows
    for (size_t i = 0; i < n_threads - 1; i++){
        int step = image.rows / n_threads;
        int s = i * step + 1;
        int e = (i + 1) * step + 1;
        for (size_t j = 0; j < integral.cols; j++){
            critical_rows.at<int>(i, j) = integral.at<int>(e - 1, j);
            if(i > 0){
                critical_rows.at<int>(i, j) += critical_rows.at<int>(i - 1, j);
            }
        }
    }
    delete[] threads;
    threads = new thread*[n_threads - 1];
    for (size_t i = 0; i < n_threads - 1; i++){
        threads[i] = new thread(&MTIntegralImage::merge, this, i);
    }
    
    for (size_t i = 0; i < n_threads - 1; i++){
        threads[i]->join();
    }
    delete[] threads;
    return integral;
}

void MTIntegralImage::merge(int thread_idx){
    int step = image.rows / n_threads;
    int s = (thread_idx + 1) * step + 1;
    int e = (thread_idx + 2) * step + 1;
    
    if(thread_idx == n_threads - 2)
        e = integral.rows;

    for (size_t i = s; i < e; i++){
        for (size_t j = 0; j < integral.cols; j++){
            integral.at<int>(i, j) += critical_rows.at<int>(thread_idx, j);
        }
    }
}

void MTIntegralImage::partial_integral(int thread_idx){
    
    int step = image.rows / n_threads;
    int s = thread_idx * step + 1;
    int e = s  + step;
    if (thread_idx == n_threads - 1){
        e = integral.rows;
    }
    integral.at<int>(s, 1) = image.at<uint8_t>(s - 1, 0);
    for(int i = 2; i < integral.cols; i++){
        integral.at<int>(s, i) =  integral.at<int>(s, i - 1) + image.at<uint8_t>(s - 1, i - 1);
    }
    for (size_t i = s + 1; i < e; i++){
        for(int j = 1; j < integral.cols; j++){
            integral.at<int>(i, j) = image.at<uint8_t>(i - 1, j - 1) + integral.at<int>(i - 1, j) 
            + integral.at<int>(i, j - 1) - integral.at<int>(i - 1, j -1);
        }
    }

}