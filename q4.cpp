/*
Tim Schisler
University of Missouri - St. Louis
CS 5420 Fall 2018
Assignment 3: Image Enhancement in Spatial Domain
11/02/2018
*/
/*
  Q4.CPP: performs histogram matching
*/

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "HW3.h"

const char * DISPLAY = "Histogram Matching";

void help(char *s) {
  std::cout << s << " can be invoked with the following arguments:\n"
    << "\t-h, which reports this help message." << std::endl
    << "\t-s sourceImageFileName, to select the original image" << std::endl
    << "\t\t\tN.B.: This option is required." << std::endl
    << "\t-f textFileName, to specify a histogram from a file." << std::endl
    << "\t-i imageFileName, to extract a histogram from another image."
    << std::endl << "\t\t\tN.B.: Choose one or the other of -f and -i."
    << std::endl;
}

int main(int argc, char *argv[]) {
  cv::Mat sourceImg, referenceImg, matchImg;
  std::ifstream hstFile;
  int hst[256], refhst[256], matchHst[256];

  //Command line argument processing:
  int c, index;
  opterr = 0;
  bool fFlag = false,
       iFlag = false,
       sFlag = false;
	while ((c = getopt (argc, argv, "f:hi:s:")) != -1)
	switch (c) {
  case 'f': //histogram from file
    hstFile.open(optarg);
    fFlag = true;
    break;
  case 'h': //help message
    help(argv[0]);
    break;
  case 'i': //histogram from image
    referenceImg = cv::imread(optarg, cv::IMREAD_GRAYSCALE);
    iFlag = true;
    break;
  case 's': //source, i.e. the image to equalize
    sourceImg = cv::imread(optarg, cv::IMREAD_GRAYSCALE);
    sFlag = true;
    break;
  case '?':
    if (optopt == 'f')
      fprintf (stderr, "Option -%c requires an image file argument.\n", optopt);
    else if (optopt == 'i')
      fprintf (stderr, "Option -%c requires a text file argument.\n", optopt);
    else if (optopt == 's')
      fprintf (stderr, "Option -%c requires an image file argument.\n", optopt);
    else if (isprint (optopt))
      fprintf (stderr, "Unknown option `-%c'.\n", optopt);
    else
      fprintf (stderr,
      "Unknown option character `\\x%x'.\n",
      optopt);
      help(argv[0]);
      return -1;
  default:
    abort ();
  }
  if(sFlag == 0) {
    std::cerr << "Must specify -s" << std::endl;
    help(argv[0]);
    return -1;
  }
  if (iFlag == fFlag) {
    std::cerr << "Use one or the other of -f and -i" << std::endl;
    help(argv[0]);
    return -1;
  }

  //Compute the frequency array from the reference image or file.
  if (iFlag) {
    simpleHist(&referenceImg, refhst);
  } else if (fFlag) {
    for (size_t k = 0; k < 256; k++) {
      hstFile >> refhst[k];
    }
  } else {
    std::cerr << "No reference provided for histogram matching." << std::endl;
    help(argv[0]);
  }

  //Compute the frequency array from the source image.
  simpleHist(&sourceImg, hst);

  //Compute the LUT from the source and reference frequency arrays.
  simpleLUT(hst, refhst, matchHst);

  //Produce the histogram-matched image
  for (size_t r = 0; r < sourceImg.rows; r++) {
    for (size_t c = 0; c < sourceImg.cols; c++) {
      int f = sourceImg.at<uchar>(r,c);
      matchImg.at<uchar>(r,c) = matchHst[f];
    }
  }

  cv::namedWindow ( DISPLAY );
  cv::imshow ( DISPLAY, matchImg );
  cv::waitKey ( 0 );

  return 0;
}
