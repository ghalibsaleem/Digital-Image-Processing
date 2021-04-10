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

		static string strMerge(char* str1, string str2);
		static bool checkROIOverlap(image& src, int** startXY, int** sizeXY, int nROI, int checkOverlap);

		static int** GradientToIntensityImg(image& src, int threshold, int sobelOperator, char *outLocation, int angleFlag, int angleTH[2]);
		static void EdgeDetection(image& src, int ** intensityImg, int** startXY, int** sizeXY, int nROI, int threshold[2], char* outLocation);
		static int sobelX3[3][3], sobelY3[3][3];
		static int sobelX5[5][5], sobelY5[5][5];

		static void cvEdgeDetection(char* imgLocation, int thresholdImg, int sobelOperator, int** startXY, int** sizeXY, int nROI, int thresholdArr[2], char* outLocation);
		static void cvCannyOperation(char* imgLocation, int minThreshold, int maxThreshold, char* outLocation);
		static void cvHistogramEqu(char* imgLocation, char* outLocation);
		static double cvOTSUAlgo(char* imgLocation, char* outLocation);
		static void cvHistogramStrechingOTSU(char* imgLocation, char* outLocation);

};

#endif

