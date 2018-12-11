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
  if(argc != 6) {
    std::cerr << "Usage: " << argv[0] << " imgfile x y width height" << std::endl;
    return -1;
  }
  
  cv::Mat img = cv::imread ( argv[1], cv::IMREAD_ANYCOLOR );
  if ( img.empty() ) {
    std::cerr << "Unable to open picture file " << argv[1] << std::endl;
    return -1;
  }
  
  int x = atoi(argv[2]),
      y = atoi(argv[3]),
      width = atoi(argv[4]),
      height = atoi(argv[5]);
  if(x >= img.cols || y >= img.rows) {
    std::cerr << "ROI start point out of bounds" << std::endl
	    << argv[1] << ": " << img.cols << "x" << img.rows << std::endl;
	return -1;
  }
  if(img.cols - x < width) width = img.cols - x;
  if(img.rows - y < height) height = img.rows - y;


  //Select the region of interest and apply histogram equalization to it.
  cv::Rect roiRect(x, y, width, height);
  std::vector<cv::Mat> roiMat;
  roiMat.push_back( img(roiRect) );
  int c;
  histEq(&roiMat[0], &roiMat, &c);
  if (c > 1) {
    cv::merge(roiMat, roiMat[0]);
  }
  //Replace the ROI in the image with its histogram-equalized counterpart.
  for (int rw = roiRect.y; rw < roiRect.y + roiRect.height; rw++) {
    for (int col = roiRect.x; col < roiRect.x + roiRect.width; col++) {
	  //Outline the border of the ROI.
	  if(rw == roiRect.y 
      || rw == roiRect.y + roiRect.height - 1 
      || col == roiRect.x 
	  || col == roiRect.x + roiRect.width - 1) {
		img.at<cv::Vec3b>(rw,col) = {0,0,0};
	  }
	  else {
	    img.at<cv::Vec3b>(rw,col) = roiMat[0].at<cv::Vec3b>(rw-roiRect.y, col-roiRect.x);
	  }
    }
  }
  //Display the image with ROI replaced.
  cv::namedWindow ( DISPLAY );
  cv::imshow ( DISPLAY, img );
  cv::waitKey ( 0 );

  return 0;
}
