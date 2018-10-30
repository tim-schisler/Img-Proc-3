/*
Tim Schisler
University of Missouri - St. Louis
CS 5420 Fall 2018
Assignment 3: Image Enhancement in Spatial Domain
10/29/2018
*/
/*
  HW3.H: header file for functions used in Assignment 3 answer sources.
*/

#include <opencv2/core/core.hpp>

void imgAvg(const cv::Mat *, double *);
void thresholdByChannel(const cv::Mat *, cv::Mat *);
