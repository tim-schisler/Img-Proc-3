/*
Tim Schisler
University of Missouri - St. Louis
CS 5420 Fall 2018
Assignment 3: Image Enhancement in Spatial Domain
11/02/2018
*/
/*
  Q6.CPP: subtracts an image from its histogram-equalized self.
*/

#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "HW3.h"


int main(int argc, char *argv[]) {

  //Command line argument processing
  if(argc != 2 /*&& argc!= 5*/) {
    std::cerr << "Usage: " << argv[0] << " imgfile" << std::endl;
    return -1;
  }

  //Open image from command line argument.
  cv::Mat inImg = cv::imread ( argv[1], cv::IMREAD_ANYCOLOR ),
          subImg(inImg.rows, inImg.cols, CV_8UC1),
		  mergeHistImg;
  if ( inImg.empty() ) {
    std::cerr << "Unable to open picture file " << argv[1] << std::endl;
    return -1;
  }

  //Perform histogram equalization.
  int ch;
  std::vector<cv::Mat> histImg;
  histEq(&inImg, &histImg, &ch);

  //Perform the subtraction.
  if (ch > 1) {
    cv::merge(histImg, mergeHistImg);
    cvtColor(mergeHistImg, mergeHistImg, CV_RGB2GRAY);
  }
  else {
	mergeHistImg = histImg[0];
  } 
  cvtColor(inImg, inImg, CV_RGB2GRAY);
  for (int r = 0; r < inImg.rows; r++) {
    for (int c = 0; c < inImg.cols; c++) {
      int subPxl = histImg[0].at<uchar>(r,c) - inImg.at<uchar>(r,c);
      if (subPxl < 0) subImg.at<uchar>(r,c) = 0;
      else if (subPxl > 255)  subImg.at<uchar>(r,c) = 255;
      else  subImg.at<uchar>(r,c) = subPxl;
    }
  }

  //Show the result.
  cv::imshow("Input image", inImg);
  cv::imshow("Histogram equalized image", inImg);
  cv::imshow("Input subtracted from histogram equalized image", subImg);
  cv::waitKey(0);

  return 0;
}
