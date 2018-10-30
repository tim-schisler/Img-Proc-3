/*
Tim Schisler
University of Missouri - St. Louis
CS 5420 Fall 2018
Assignment 3: Image Enhancement in Spatial Domain
10/29/2018
*/
/*
  HW3.CPP: source file for functions used in Assignment 3 answer sources.
*/
#include "HW3.h"

void imgAvg(const cv::Mat *img, double *avg) {

  long int acc[3];
  int n = img->cols * img->rows;

  for(int c = 0; c < img->cols; c++) {
    for(int r = 0; r < img->rows; r++) {
      cv::Vec3b clrPxl = img->at<cv::Vec3b>(r,c);
      for (int ch = 0; ch < 3; ch++) {
        acc[ch] += clrPxl[ch];
      }
    }
  }

  for(int channel = 0; channel < 3; channel++) {
    avg[channel] = (double)acc[channel] / (double)n;
  }

}

void thresholdByChannel(const cv::Mat *usrImg, cv::Mat *thrshImg) {

  //Prepare for thresholding
  *thrshImg = cv::Mat::zeros(usrImg->rows, usrImg->cols, CV_8UC1);
  double avgs[3];
  imgAvg(usrImg, avgs);

  //Threshold channel by channel
  for(int c = 0; c < usrImg->cols; c++) {
    for(int r = 0; r < usrImg->rows; r++) {
      for(int a = 0; a < 3; a++) {
        if( (double)(usrImg->at<cv::Vec3b>(r,c))[a] >= avgs[a]) {
          thrshImg->at<uchar>(r,c) = 255;
        }
      }
    }
  }

}
