#include "utility.h"

#define MAXRGB 255
#define MINRGB 0


std::string utility::intToString(int number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

int utility::checkValue(int value)
{
	if (value > MAXRGB)
		return MAXRGB;
	if (value < MINRGB)
		return MINRGB;
	return value;
}

/*-----------------------------------------------------------------------**/
void utility::addGrey(image &src, image &tgt, int value)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			tgt.setPixel(i,j,checkValue(src.getPixel(i,j)+value)); 
		}
}

/*-----------------------------------------------------------------------**/

void utility::binarize(image& src, image& tgt, int threshold1, int threshold2)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i = 0; i < src.getNumberOfRows(); i++)
	{
		for (int j = 0; j < src.getNumberOfColumns(); j++)
		{
			int temp = src.getPixel(i, j);
			if (threshold1 < temp && temp < threshold2)
				tgt.setPixel(i, j, MAXRGB);
			else
				tgt.setPixel(i, j, MINRGB);
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::scale(image &src, image &tgt, float ratio)
{
	int rows = (int)((float)src.getNumberOfRows() * ratio);
	int cols  = (int)((float)src.getNumberOfColumns() * ratio);
	tgt.resize(rows, cols);
	for (int i=0; i<rows; i++)
	{
		for (int j=0; j<cols; j++)
		{	
			/* Map the pixel of new image back to original image */
			int i2 = (int)floor((float)i/ratio);
			int j2 = (int)floor((float)j/ratio);
			if (ratio == 2) {
				/* Directly copy the value */
				tgt.setPixel(i,j,checkValue(src.getPixel(i2,j2)));
			}

			if (ratio == 0.5) {
				/* Average the values of four pixels */
				int value = src.getPixel(i2,j2) + src.getPixel(i2,j2+1) + src.getPixel(i2+1,j2) + src.getPixel(i2+1,j2+1);
				tgt.setPixel(i,j,checkValue(value/4));
			}
		}
	}
}

/*-----------------------------------------------------------------------**/



bool utility::checkROIOverlap(image& src, int** startXY, int** sizeXY, int nROI, int checkOverlap)
{
	int count = 0;
	for (int i = 0; i < nROI - 1; i++)
	{
		for (int j = i + 1; j < nROI; j++)
		{
			//Condition Check
			if (!(abs(startXY[i][0] - startXY[j][0]) > fmax(sizeXY[i][0], sizeXY[j][0])
				&& abs(startXY[i][1] - startXY[j][1]) > fmax(sizeXY[i][1], sizeXY[j][1]))) {
				if (checkOverlap == 1)
					return false;
				startXY[j][2] = 0;
				count++;
			}
		}
	}
	if (nROI - count < 3)
	{
		cout << "No of non overlaping ROI should be more than 2";
		exit(1);
	}
	return true;
}

void utility::binarizeROI(image& src, image& tgt, int* threshold1, int* threshold2, int** startXY, int** sizeXY, int nROI)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	tgt.copyImage(src);
	for (int k = 0; k < nROI; k++)
	{
		if (startXY[k][2] == 1)
			for (int i = startXY[k][0]; i < src.getNumberOfRows() && i < startXY[k][0] + sizeXY[k][0]; i++)
			{
				for (int j = startXY[k][1]; j < src.getNumberOfColumns() && j < startXY[k][1] + sizeXY[k][1]; j++)
				{
					int temp = src.getPixel(i, j);
					if (threshold1[k] < temp && temp < threshold2[k])
						tgt.setPixel(i, j, MAXRGB);
					else
						tgt.setPixel(i, j, MINRGB);
				}
			}
	}
}

//window size 3
void utility::smoothingROI(image& src, image& tgt, int** startXY, int** sizeXY, int nROI)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	tgt.copyImage(src);
	for (int k = 0; k < nROI; k++)
	{
		if (startXY[k][2] == 1)
			for (int i = startXY[k][0]; i < src.getNumberOfRows() && i < startXY[k][0] + sizeXY[k][0]; i++)
			{
				for (int j = startXY[k][1]; j < src.getNumberOfColumns() && j < startXY[k][1] + sizeXY[k][1]; j++)
				{
					int mean = 0;
					mean = calculateMean(src, i, j);
					tgt.setPixel(i, j, (int)mean);
				}
			}
	}
}

void utility::colorBinarization(image& src, image& tgt, int** startXY, int** sizeXY, int nROI, int thresholdDist, int* thresholdColor)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	tgt.copyImage(src);
	for (int k = 0; k < nROI; k++) {
		if (startXY[k][2] == 1)
			for (int i = startXY[k][0]; i < src.getNumberOfRows() && i < startXY[k][0] + sizeXY[k][0]; i++)
			{
				for (int j = startXY[k][1]; j < src.getNumberOfColumns() && j < startXY[k][1] + sizeXY[k][1]; j++)
				{
					int tempPixel[3]; 
					double distance = 0;
					tempPixel[0] =  src.getPixel(i, j, 0);
					tempPixel[1] = src.getPixel(i, j, 1);
					tempPixel[2] = src.getPixel(i, j, 2);

					distance = calculateDistance(tempPixel, thresholdColor);
					if (distance < thresholdDist)
					{
						tgt.setPixel(i, j, 0, MAXRGB);
						tgt.setPixel(i, j, 1, MAXRGB);
						tgt.setPixel(i, j, 2, MAXRGB);
					}
					else {
						tgt.setPixel(i, j, 0, MINRGB);
						tgt.setPixel(i, j, 1, MINRGB);
						tgt.setPixel(i, j, 2, MINRGB);
					}
				}
			}
	}
}



double utility::calculateMean(image& src, int a, int b) {
	int sum = 0;
	int no = 0;
	for (int i = a - 1; i < a + 3; i++)
	{
		if (i >= 0 && i < src.getNumberOfRows())
		{
			for (int j = b - 1; j < b + 3; j++)
			{
				if (j >= 0 && j < src.getNumberOfColumns())
				{
					no++;
					sum += src.getPixel(i, j);
				}
			}
		}
	}
	return sum/no;
}

double utility::calculateDistance(int src[3], int dst[3])
{
	return sqrt((src[0] - dst[0]) * (src[0] - dst[0]) + (src[1] - dst[1]) * (src[1] - dst[1]) + (src[2] - dst[2]) * (src[2] - dst[2]));
}





