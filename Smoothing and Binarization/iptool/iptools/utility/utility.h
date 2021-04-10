#ifndef UTILITY_H
#define UTILITY_H

#include "../image/image.h"
#include <sstream>
#include <math.h>

class utility
{
	private:
		static double calculateMean(image& src, int a, int b);
		static double calculateDistance(int src[3], int dst[3]);

	public:
		utility();
		virtual ~utility();
		static std::string intToString(int number);
		static int checkValue(int value);
		static void addGrey(image &src, image &tgt, int value);
		static void binarize(image &src, image &tgt, int threshold1, int threshold2);
		static void scale(image &src, image &tgt, float ratio);

		static bool checkROIOverlap(image& src, int** startXY, int** sizeXY, int nROI, int checkOverlap);
		static void binarizeROI(image& src, image& tgt, int* threshold1, int* threshold2, int** startXY, int** sizeXY, int nROI);
		static void smoothingROI(image& src, image& tgt, int** startXY, int** sizeXY, int nROI);
		static void colorBinarization(image& src, image& tgt, int** startXY, int** sizeXY, int nROI, int thresholdDist, int* thresholdColor);
};

#endif

