#ifndef UTILITY_H
#define UTILITY_H

#include "../image/image.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <sstream>
#include <math.h>

class utility
{
	public:
		utility();
		virtual ~utility();
		static std::string intToString(int number);
		static int checkValue(int value);
		static void addGrey(image &src, image &tgt, int value);
		static void binarize(image &src, image &tgt, int threshold);
		static void scale(image &src, image &tgt, float ratio);
		static void cv_gray(cv::Mat &src, cv::Mat &tgt);
		static void cv_avgblur(cv::Mat &src, cv::Mat &tgt, int WindowSize);
		static void cv_dft(cv::Mat &src, cv::Mat &tgt);

		static bool checkROIOverlap(image& src, int** startXY, int** sizeXY, int nROI, int checkOverlap);
		static void cvDFTFreq(cv::Mat& src, cv::Mat& tgt, string path, int writeFlag);
		static void FilterFreq(cv::Mat& src, int filterType, int** startXY, int** sizeXY, int nROI, int thresholdV[3], string outPath);
		static void cvDFT(cv::Mat& src, cv::Mat& tgt, int thresholdV[3], int filterType);
		static void Filter(cv::Mat& src, cv::Mat& tgt, int filterType, int** startXY, int** sizeXY, int nROI, int thresholdV[3], string outPath);
		static void quadShift(cv::Mat& src);

		static bool chooseFilter(int filterType, double dist, int th1[3]);
};

#endif

