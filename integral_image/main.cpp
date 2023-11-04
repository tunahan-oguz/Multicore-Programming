#include "IntegralImage.hpp"
#include "../Report.hpp"

using namespace std;
bool verify(cv::Mat calculated, cv::Mat actual);

int main(int argc, char const *argv[]) {
    int NTHRD = atoi(argv[1]);
    int height = atoi(argv[2]);
    int width = atoi(argv[3]);

    cv::Mat img = cv::imread("/home/tuntun/Desktop/Multicore-Programming/cpp_qs/lena_grayscale_hq.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat resizedImage;
    cv::resize(img, resizedImage, cv::Size(height, width));
    if (img.empty()) {
        std::cerr << "Failed to load the image." << std::endl;
        return -1;
    }
    STIntegralImage* st = new STIntegralImage(resizedImage); 
    MTIntegralImage* mt = new MTIntegralImage(resizedImage, NTHRD);
    
    // cv::Mat integ;
    // cv::integral(resizedImage, integ);
    // cv::Mat st_out = (*st)();
    
    // cv::Mat mt_out = (*mt)();
    
    // cout << verify(mt_out, integ) << endl;
    // cout << verify(st_out, integ) << endl;
    
    measure_run_time(st);
    measure_run_time(mt);


    return 0;
}

bool verify(cv::Mat calculated, cv::Mat actual){
    for (size_t i = 0; i < actual.rows; i++){
        for (size_t j = 0; j < actual.cols; j++){
            uint16_t act = actual.at<uint16_t>(i, j);
            uint16_t calc = calculated.at<uint16_t>(i, j);
            if(act != calc){
                cout << act << " " << calc << endl;
                return false;
            }
        }
    }
    return true;
}