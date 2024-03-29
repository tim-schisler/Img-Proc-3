/*
Tim Schisler
University of Missouri - St. Louis
CS 5420 Fall 2018
Assignment 3: Image Enhancement in Spatial Domain
10/31/2018
*/
/*
  HW3.CPP: source file for functions used in Assignment 3 answer sources.
*/
#include "HW3.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>


void thresholdByChannel(const cv::Mat *usrImg, cv::Mat *thrshImg) {

  //Prepare for thresholding
  int c = usrImg->channels();
  double avgs[c];
  long int acc[c];

  std::vector<cv::Mat> imgChan;
  if (c > 1) {
    cv::split(*usrImg, imgChan);
  } else {
    imgChan[0] = *usrImg;
  }

  int n = usrImg->cols * usrImg->rows;
  for (size_t chan = 0; chan < c; chan++)
    acc[chan] = avgs[chan] = 0;

  for (int ch = 0; ch < c; ch++)
    for(int col = 0; col < usrImg->cols; col++)
      for(int rw = 0; rw < usrImg->rows; rw++) {
        int clrPxl = imgChan[ch].at<uchar>(rw,col);
        acc[ch] += clrPxl;
      }

  for(int channel = 0; channel < c; channel++)
    avgs[channel] = (double)acc[channel] / (double)n;

  //Threshold channel by channel
  for(int col = 0; col < usrImg->cols; col++)
    for(int r = 0; r < usrImg->rows; r++)
      for(int a = 0; a < c; a++) {
        if( (double)(imgChan[a].at<uchar>(r,col)) >= avgs[a])
          imgChan[a].at<uchar>(r,col) = 255;
        else imgChan[a].at<uchar>(r,col) = 0;
      }

  if (c > 1) {
    cv::merge(imgChan, *thrshImg);
  }
  else *thrshImg = imgChan[0];

}

void histEq(const cv::Mat *usrImg, std::vector<cv::Mat> *hstImg, int *c) {

  *c = usrImg->channels();
  if (*c > 1) {
    cv::split(*usrImg, *hstImg);
  } else {
    (*hstImg).push_back(*usrImg);
  }


  long int frqncy[*c][256],
      LUT[*c][256];
  long double cdf[*c][256],
         rltvFrqncy[*c][256];
  //Initialize the arrays
  for (size_t chan = 0; chan < *c; chan++)
    for (size_t val = 0; val < 256; val++) {
      frqncy[chan][val] = LUT[chan][val] = 0;
      cdf[chan][val] = rltvFrqncy[chan][val] = 0.0;
    }

  //compute the frequencies
  for (size_t chan = 0; chan < *c; chan++)
    for (size_t rw = 0; rw < usrImg->rows; rw++)
      for (size_t clm = 0; clm < usrImg->cols; clm++) {
        int pxl = (*hstImg)[chan].at<uchar>(rw,clm);
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
        (*hstImg)[chan].at<uchar>(rw,clm) = LUT[chan][ (*hstImg)[chan].at<uchar>(rw,clm) ];
      }
  //if(*c > 1) cv::merge(*hstImg, (*hstImg)[*c]);
}

void simpleHist(const cv::Mat *img, int *freq) {
  //Initialize the histogram array.
  for (size_t i = 0; i < 256; i++) {
    freq[i] = 0;
  }
  //Parse the image pixels and fill the histogram bins.
  for (size_t r = 0; r < img->rows; r++) {
    for (size_t c = 0; c < img->cols; c++) {
      int pxl = img->at<uchar>(r,c);
      freq[pxl] += 1;
    }
  }
}

static void cdf(int *freq, int sz, double *h) {
  int n = 0;
  for (size_t i = 0; i < sz; i++) n += freq[i];
  for (size_t j = 0; j < sz; j++) h[j] = (double)freq[j] / (double)n;
  for (size_t k = 1; k < sz; k++)  h[k] += h[k-1];
}

void simpleLUT(int *src, int *ref, int *dst) {

  //Compute CDFs.
  double hst[2][256];
  cdf(src, 256, hst[0]);
  cdf(ref, 256, hst[1]);

  //Compute the lookup table.
  for (int l = 0; l < 256; l++) {
    int lprime = 255;
    do {
      dst[l] = lprime;
      lprime -= 1;
    } while(lprime >= 0 && hst[0][lprime] > hst[1][l]);
  }
}
