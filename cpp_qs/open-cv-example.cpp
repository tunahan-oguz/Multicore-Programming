#include <opencv2/highgui.hpp>
#include <iostream>

int main( int argc, char** argv ) {
  
  cv::Mat image;
  image = cv::imread("lena_grayscale_hq.jpg" , cv::IMREAD_GRAYSCALE);
  
  if(! image.data ) {
      std::cout <<  "Could not open or find the image" << std::endl ;
      return -1;
    }
  
  cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );
  cv::imshow( "Display window", image );
  
  cv::waitKey(0);
  return 0;
}