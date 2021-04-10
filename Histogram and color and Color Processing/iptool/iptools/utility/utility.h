#ifndef UTILITY_H
#define UTILITY_H

#include "../image/image.h"
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

		static bool checkROIOverlap(image& src, int** startXY, int** sizeXY, int nROI, int checkOverlap);

		static int OptimalThresholding(image& src, image& tgt, int limit, int** startXY, int** sizeXY, int nROI);
		static int OptimalThresholding(image& src, image& tgt, image& tgt1, image& tgt2, int limit, int** startXY, int** sizeXY, int nROI);
		 
		static void histogramStreaching(image& src, image& tgt, int** startXY, int** sizeXY, int nROI, int* min, int* max, char** HistoOrignal, char** HistoNew);
		static void histogramStreaching(image& src, image& tgt, int** startXY, int** sizeXY, int nROI);

		
		static void convertRGB2HSI(double *rgb, double* hsi);
		static void convertHSI2RGB(double* rgb, double* hsi);
		static void histogramStrechingHSI(image& src, image& tgt, int* startXY, int* sizeXY, bool channelFlag[3]);
		
};

#endif

