/*
Tim Schisler
University of Missouri - St. Louis
CS 5420 Fall 2018
Assignment 3: Image Enhancement in Spatial Domain
10/29/2018
*/
/*
  Q1.CPP: shows pixel value at a mouse click
*/

#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "HW3.h"

const char * DISPLAY = "Click For Pixel Value";
cv::Mat clrImg;

//Prepare the mouse click handling.
static void mouseCall (int event, int x, int y, int flags, void *userdata) {
  switch (event) {
  case cv::EVENT_LBUTTONDOWN:
    std::string text = "ROW:";
    text.append(std::to_string(y));
    text.append("  COL:");
    text.append(std::to_string(x));

    //differentiate between greyscale and color images
    if( clrImg.channels() > 1 ) {
      cv::Vec3b clrPxl = clrImg.at<cv::Vec3b>(y,x);
      text += "  B:";
      text += std::to_string(clrPxl[0]);
      text += "  G:";
      text += std::to_string(clrPxl[1]);
      text += "  R:";
      text += std::to_string(clrPxl[2]);
    }
    else {
      int gryPxl = clrImg.at<uchar>(y,x);
      text += "  value:";
      text += std::to_string(gryPxl);
    }

    //cv::addText( *((cv::Mat *)(userdata)), text, cv::Point(0,0), cv::fontQt("Helvetica") );
	//cv::displayOverlay(DISPLAY, text, 1500);
	cv::Mat tmp;
	clrImg.copyTo(tmp);
	cv::putText(tmp, text, cv::Point(20,20), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar::all(255));
	cv::imshow ( DISPLAY, tmp );
    break;
  }
};

int main(int argc, char *argv[]) {

  //Command line argument processing
  if(argc != 2) {
    std::cerr << "Usage: " << argv[0] << " imgfile" << std::endl;
    return -1;
  }

  //Open image from command line argument.
  clrImg = cv::imread ( argv[1], cv::IMREAD_ANYCOLOR );
  if ( clrImg.empty() ) {
    std::cerr << "Unable to open picture file " << argv[1] << std::endl;
    return -1;
  }

  cv::namedWindow ( DISPLAY );
  cv::setMouseCallback( DISPLAY, mouseCall );
  cv::imshow ( DISPLAY, clrImg );
  cv::waitKey ( 0 );

  return 0;
}
