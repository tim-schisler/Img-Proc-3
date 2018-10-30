/*
Tim Schisler
University of Missouri - St. Louis
CS 5420 Fall 2018
Assignment 3: Image Enhancement in Spatial Domain
10/29/2018
*/
/*
  Q2.CPP: thresholding an image at the average value channel-by-channel.
*/

#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "HW3.h"

const char * DISPLAY = "Thresholded Image";

int main(int argc, char *argv[]) {

  //Command line argument processing
  if(argc != 2) {
    std::cerr << "Usage: " << argv[0] << " imgfile" << std::endl;
    return -1;
  }

  //Open image from command line argument.
  cv::Mat usrImg = cv::imread ( argv[1], cv::IMREAD_ANYCOLOR );
  if ( usrImg.empty() ) {
    std::cerr << "Unable to open picture file " << argv[1] << std::endl;
    return -1;
  }

  //Initialize a blank image for thresholding.
  cv::Mat thrshImg;

  thresholdByChannel(&usrImg, &thrshImg);

  std::string thrshImgName = "Q2out.jpg";
  cv::imwrite(thrshImgName, thrshImg);

  /*
  cv::namedWindow ( DISPLAY );
  cv::imshow ( DISPLAY, thrshImg );
  cv::waitKey ( 0 );
  */

  return 0;
}
