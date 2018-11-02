/*
Tim Schisler
University of Missouri - St. Louis
CS 5420 Fall 2018
Assignment 3: Image Enhancement in Spatial Domain
10/31/2018
*/
/*
  Q3.CPP: performs histogram equalization channel-by-channel
*/

#include <iostream>
#include <string>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "HW3.h"


int main(int argc, char *argv[]) {
  //Check proper invocation.
  if(argc != 2) {
    std::cerr << "Usage: " << argv[0] << " ImageFile" << std::endl;
    return -1;
  }

  //Open image from command line argument.
  cv::Mat inputImg = cv::imread ( argv[1], cv::IMREAD_ANYCOLOR ), cvtHistImg;
  std::vector<cv::Mat> histImg;
  if ( inputImg.empty() ) {
    std::cerr << "Unable to open picture file " << argv[1] << std::endl;
    return -1;
  }

  int ch, dummych;
  histEq(&inputImg, &histImg, &ch);
  /*
  if (ch > 1) {
    cv::cvtColor(inputImg, cvtHistImg, cv::COLOR_BGR2GRAY);
    histEq(&cvtHistImg[0], cvtHistImg, &dummych);
  }
  */
  for(size_t c = 0; c < ch; c++) {
    std::string title = "Channel ";
    title += std::to_string(c);
    cv::namedWindow ( title );
    cv::imshow ( title, histImg[c] );
  }
  cv::waitKey(0);

  return 0;
}
