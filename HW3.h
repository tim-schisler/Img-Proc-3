/*
Tim Schisler
University of Missouri - St. Louis
CS 5420 Fall 2018
Assignment 3: Image Enhancement in Spatial Domain
10/31/2018
*/
/*
  HW3.H: header file for functions used in Assignment 3 answer sources.
*/

#ifndef HW3_H_
#define HW3_H_

#include <opencv2/core/core.hpp>


void thresholdByChannel(const cv::Mat *, cv::Mat *);
void histEq(const cv::Mat *, std::vector<cv::Mat> *, int *);

#endif
