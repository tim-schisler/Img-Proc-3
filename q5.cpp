/*
Tim Schisler
University of Missouri - St. Louis
CS 5420 Fall 2018
Assignment 3: Image Enhancement in Spatial Domain
11/03/2018
*/
/*
  Q5.CPP: performs histogram equalization on a region of interest
*/

#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp> //for selectROI()
#include "HW3.h"

const char * DISPLAY = "Histogram Equalized ROI";

int main(int argc, char *argv[]) {

  //Command line argument processing
  if(argc != 2) {
    std::cerr << "Usage: " << argv[0] << " imgfile" << std::endl;
    return -1;
  }

  //Open image from command line argument.
  cv::Mat img = cv::imread ( argv[1], cv::IMREAD_GRAYSCALE );
  if ( img.empty() ) {
    std::cerr << "Unable to open picture file " << argv[1] << std::endl;
    return -1;
  }

  //Select the region of interest and apply histogram equalization to it.
  cv::Rect roiRect = cv::selectROI("Select ROI with Mouse", img);
  std::vector<cv::Mat> roiMat;
  roiMat[0] = img(roiRect);
  int c;
  histEq(&roiMat[0], &roiMat, &c);

  //Replace the ROI in the image with its histogram-equalized counterpart.
  for (int rw = roiRect.y; rw < roiRect.y + roiRect.height; rw++)
    for (int col = roiRect.x; col < roiRect.x + roiRect.width; col++)
      img.at<uchar>(rw,col) = roiMat[0].at<uchar>(rw-roiRect.y, col-roiRect.x);

  //Display the image with ROI replaced.
  cv::namedWindow ( DISPLAY );
  cv::imshow ( DISPLAY, img );
  cv::waitKey ( 0 );

  return 0;
}
