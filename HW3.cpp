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
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

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

  for(int channel = 0; channel < 3; channel++)
    avg[channel] = (double)acc[channel] / (double)n;

}

void stats(const cv::Mat *img, double *avg, double *sd, int *frq, double *relFrq) {
  //First pass: record frequencies and sum pixel values
  long int acc = 0;
  int n = img->cols * img->rows;
  for(int c = 0; c < img->cols; c++)
    for(int r = 0; r < img->rows; r++) {
      int pxl = (int)img->at<uchar>(r,c);
      acc += pxl;
      frq[pxl] += 1;
    }
  //compute average
  *avg = (double)acc / (double)n;
  //compute relative frequencies
  for(int j = 0; j < 256; j++) {
    relFrq[j] = (double)frq[j] / (double)n;
  }
  //Second pass: record deviation for each pixel and sum the variance
  double devAcc = 0.0;
  for(int c = 0; c < img->cols; c++)
    for(int r = 0; r < img->rows; r++) {
      double pxl = (double)(img->at<uchar>(r,c));
      devAcc += ( pow( (pxl - *avg), 2.0 ) * relFrq[(unsigned char)pxl] );
    }
  //compute standard deviation from the accumulated variance
  *sd = pow(devAcc, 0.5);
}

void thresholdByChannel(const cv::Mat *usrImg, cv::Mat *thrshImg) {

  //Prepare for thresholding
  *thrshImg = cv::Mat::zeros(usrImg->rows, usrImg->cols, CV_8UC1);
  double avgs[3];
  imgAvg(usrImg, avgs);

  //Threshold channel by channel
  for(int c = 0; c < usrImg->cols; c++)
    for(int r = 0; r < usrImg->rows; r++)
      for(int a = 0; a < 3; a++)
        if( (double)(usrImg->at<cv::Vec3b>(r,c))[a] >= avgs[a])
          thrshImg->at<uchar>(r,c) = 255;

}

void histEq(const cv::Mat *usrImg, std::vector<cv::Mat>hstImg, int *c) {

  *c = usrImg->channels();
  if (*c > 1) {
    cv::split(*usrImg, hstImg);
  } else {
    hstImg[0] = *usrImg;
  }


  int frqncy[*c][256],
      LUT[*c][256];
  double cdf[*c][256],
         rltvFrqncy[*c][256];

  //compute the relative frequencies
  for (size_t chan = 0; chan < *c; chan++)
    for (size_t rw = 0; rw < usrImg->rows; rw++)
      for (size_t clm = 0; clm < usrImg->cols; clm++) {
        int pxl = hstImg[chan].at<uchar>(rw,clm);
        frqncy[chan][pxl] += 1;
      }
  //compute the cdfs and complete the lookup table
  int n = usrImg->rows * usrImg->cols;
  for (size_t chan = 0; chan < *c; chan++) {
    for (size_t lvl = 0; lvl < 256; lvl++) {
      rltvFrqncy[chan][lvl] = (double)frqncy[chan][lvl] / (double)n;
      if (lvl == 0) {
        cdf[chan][lvl] = rltvFrqncy[chan][lvl];
      } else {
        cdf[chan][lvl] = cdf[chan][lvl-1] + rltvFrqncy[chan][lvl];
      }
      LUT[chan][lvl] = (int)(255.0 * cdf[chan][lvl]);
    }
  }

  //pass input image through LUTs to produce output image(s)
  for (size_t chan = 0; chan < *c; chan++)
    for (size_t rw = 0; rw < usrImg->rows; rw++)
      for (size_t clm = 0; clm < usrImg->cols; clm++) {
        hstImg[chan].at<uchar>(rw,clm) = LUT[chan][ hstImg[chan].at<uchar>(rw,clm) ];
      }
  if(*c > 1) cv::merge(hstImg, hstImg[*c]);
}
