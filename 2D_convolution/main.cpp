#include "Conv.hpp"
#include "../Report.hpp"

using namespace std;
bool verify(cv::Mat st, cv::Mat mt);

int main(int argc, char const *argv[])
{
    int NTHRDS = atoi(argv[1]);
    cv::Mat image = cv::imread("./../cpp_qs/lena_grayscale_hq.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat weights = cv::Mat::ones(3, 3, CV_8U);    
    STConv* st = new STConv(image, weights, 3);
    MTConv* mt = new MTConv(image, weights, 3, NTHRDS);
    
    // cv::Mat st_out = (*st)();
    // cv::Mat mt_out = (*mt)();
    // cout << verify(st_out, mt_out) << endl;

    measure_run_time(st);
    measure_run_time(mt);

    return 0;
}

bool verify(cv::Mat st, cv::Mat mt){
    for (size_t i = 0; i < st.rows; i++){
        for (size_t j = 0; j < st.cols; j++){
            if (st.at<_Float32>(i, j) != mt.at<_Float32>(i, j))
                return false;
        }
    }
    return true;
}