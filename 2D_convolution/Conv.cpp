#include "Conv.hpp"

STConv::STConv(cv::Mat image, cv::Mat weights, int kernel_sz){
    
    image.convertTo(this->image, CV_32F);
    weights.convertTo(this->weights, CV_32F);
    this->kernel_sz = kernel_sz;
    this->padding = kernel_sz / 2;

    this->padded.create(this->image.rows + 2*padding, this->image.cols + 2*padding, this->image.type());
    this->padded.setTo(cv::Scalar::all(0));
    this->image.copyTo(this->padded(cv::Rect(padding, padding, this->image.cols, this->image.rows)));

    this->res = cv::Mat::zeros(this->image.rows, this->image.rows, CV_32F);
}

cv::Mat STConv::operator()(){
    _Float32 w;
    _Float32 pixel;
    _Float32 w_sum;
    int pix_loc1;
    int pix_loc2;
    int numel = weights.total();
    for (size_t i = padding; i < padded.rows - padding; i++){
        for (size_t j = padding; j < padded.cols - padding; j++){
            w_sum = 0;
            for (size_t k = 0; k < weights.rows; k++){
                for (size_t l = 0; l < weights.cols; l++){
                    w = weights.at<_Float32>(k, l);
                    pix_loc1 = i  - padding + k;
                    pix_loc2 = j - padding + l;   
                    pixel = padded.at<_Float32>(pix_loc1, pix_loc2);
                    w_sum += (pixel * w);
                }   
            }
            res.at<_Float32>(i - padding, j - padding) = w_sum;
        }
    }
    return res;
}


MTConv::MTConv(cv::Mat image, cv::Mat weights, int kernel_sz, int n_threads){
    this->n_threads = n_threads;
    image.convertTo(this->image, CV_32F);
    weights.convertTo(this->weights, CV_32F);
    this->kernel_sz = kernel_sz;
    this->padding = kernel_sz / 2;

    this->padded = cv::Mat::zeros(this->image.rows + 2 * padding, this->image.cols + 2 * padding, CV_32F);
    image.copyTo(padded(cv::Rect(padding, padding, image.cols, image.rows)));
    
    this->res = cv::Mat::zeros(this->image.rows, this->image.cols, CV_32F);
}


cv::Mat MTConv::operator()(){
    thread** threads = new thread*[n_threads];
    for (size_t i = 0; i < n_threads; i++){
        threads[i] = new thread(&MTConv::partial_conv, this, i);    
    }
    
    for (size_t i = 0; i < n_threads; i++){
        threads[i]->join();
    }

    return this->res;
}

void MTConv::partial_conv(int t_idx){
    int step = image.rows / n_threads;
    int s = t_idx * step;
    int e = s + step;
    if(t_idx == n_threads - 1)
        e = image.rows;
    step = e - s;
    int s_pad = s + padding;
    int e_pad = e + padding;


    cv::Mat region = cv::Mat::zeros(step + 2 * padding, padded.cols, padded.type());;
    cv::Mat target_region = padded.rowRange(s_pad - padding, e_pad + padding);
    target_region.copyTo(region);
    _Float32 w;
    _Float32 pixel;
    _Float32 w_sum;
    cv::Mat local_Weights = weights.clone();
    int pix_loc1;
    int pix_loc2;
    int numel = weights.total();
    for (size_t i = padding; i < region.rows - padding; i++){
        for (size_t j = padding; j < region.cols - padding; j++){
            w_sum = 0;
            for (size_t k = 0; k < weights.rows; k++){
                for (size_t l = 0; l < weights.cols; l++){
                    w = local_Weights.at<_Float32>(k, l);
                    pix_loc1 = i  - padding + k;
                    pix_loc2 = j - padding + l;   
                    pixel = region.at<_Float32>(pix_loc1, pix_loc2);
                    w_sum += (pixel * w);
                }   
            }
            res.at<_Float32>(s + (i - padding), j - padding) = w_sum;
        }
    }
}
