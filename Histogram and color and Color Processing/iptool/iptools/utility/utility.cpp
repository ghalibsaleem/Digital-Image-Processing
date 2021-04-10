#define _USE_MATH_DEFINES
#include "utility.h"
#include <algorithm>
#include <cmath>


#define MAXRGB 255
#define MINRGB 0
using namespace std;

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
void utility::binarize(image &src, image &tgt, int threshold)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			if (src.getPixel(i,j) < threshold)
				tgt.setPixel(i,j,MINRGB);
			else
				tgt.setPixel(i,j,MAXRGB);
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

int utility::OptimalThresholding(image& src, image& tgt, int limit, int** startXY, int** sizeXY, int nROI)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	tgt.copyImage(src);
	int median = 0;
	for (int k = 0; k < nROI; k++)
	{
		if (startXY[k][2] == 1) {
			int size = sizeXY[k][0] * sizeXY[k][1];
			int data[100000],a=0;
			for (int i = startXY[k][0]; i < src.getNumberOfRows() && i < startXY[k][0] + sizeXY[k][0]; i++)
			{
				for (int j = startXY[k][1]; j < src.getNumberOfColumns() && j < startXY[k][1] + sizeXY[k][1]; j++)
				{
					data[a] = src.getPixel(i, j);
					a++;
				}
			}
			//vector<int> data = src.getGreyPixelData();
			
			sort(data,data + size);
			
			median = size % 2 == 0 ? data[(size / 2) + 1] : data[size / 2];
			int foreground = 0, background = 0;

			for (int i = 0; i < size; i++)
			{
				if (i < size / 2)
					background += data[i];
				else
					foreground += data[i];
			}
			background = (background) / (int)(size/2);
			foreground = (foreground) / (int)(size / 2);

			int newThreshold = (background + foreground) / 2;
			if (abs(newThreshold-median) >= limit)
			{
				median = newThreshold;
			}

			for (int i = startXY[k][0]; i < src.getNumberOfRows() && i < startXY[k][0] + sizeXY[k][0]; i++)
			{
				for (int j = startXY[k][1]; j < src.getNumberOfColumns() && j < startXY[k][1] + sizeXY[k][1]; j++)
				{
					if (src.getPixel(i, j) < median)
						tgt.setPixel(i, j, MINRGB);
					else
						tgt.setPixel(i, j, MAXRGB);
					
				}
			}
			
		}
		
	}
	return median;
}

int utility::OptimalThresholding(image& src, image& tgt, image& tgt1, image& tgt2, int limit, int** startXY, int** sizeXY, int nROI)
{
	int columns = src.getNumberOfColumns(), rows = src.getNumberOfRows();
	tgt1.resize(rows, columns);
	tgt1.copyImage(src);
	tgt.resize(rows, columns);
	tgt.copyImage(src);
	tgt2.resize(rows, columns);
	tgt2.copyImage(src);
	for (int k = 0; k < nROI; k++)
	{
		if (startXY[k][2] == 1) {
			int size = sizeXY[k][0] * sizeXY[k][1];
			int a = 0;
			int* data;
			data = (int*)malloc(size * sizeof(int));
			for (int i = startXY[k][0]; i < rows && i < startXY[k][0] + sizeXY[k][0]; i++)
			{
				for (int j = startXY[k][1]; j < columns && j < startXY[k][1] + sizeXY[k][1]; j++)
				{
					data[a] = src.getPixel(i, j);
					a++;
				}
			}
			//vector<int> data = src.getGreyPixelData();

			sort(data, data + size);

			int median = size % 2 == 0 ? data[(size / 2) + 1] : data[size / 2];
			int foreground = 0, background = 0;

			for (int i = 0; i < size; i++)
			{
				if (i < size / 2)
					background += data[i];
				else
					foreground += data[i];
			}
			background = (background) / (int)(size / 2);
			foreground = (foreground) / (int)(size / 2);

			int newThreshold = (background + foreground) / 2;
			if (abs(newThreshold - median) >= limit)
			{
				median = newThreshold;
			}

			for (int i = startXY[k][0]; i < rows && i < startXY[k][0] + sizeXY[k][0]; i++)
			{
				for (int j = startXY[k][1]; j < columns && j < startXY[k][1] + sizeXY[k][1]; j++)
				{
					if (src.getPixel(i, j) < median) {
						tgt.setPixel(i, j, MINRGB);
						//tgt1.setPixel(i, j, MINRGB);
						tgt2.setPixel(i, j, MINRGB);
					}
					else {
						tgt.setPixel(i, j, MAXRGB);
						tgt1.setPixel(i, j, MINRGB);
						//tgt2.setPixel(i, j, MINRGB);
					}

				}
			}
			return median;
		}
	}
	return 0;
}

void utility::histogramStreaching(image& src, image& tgt, int** startXY, int** sizeXY, int nROI, int *min, int *max, char** HistoOrignal, char** HistoNew)
{
	int rows = src.getNumberOfRows(), columns = src.getNumberOfColumns();
	tgt.resize(rows, columns);
	tgt.copyImage(src);
	for (int k = 0; k < nROI; k++)
	{
		if (startXY[k][2] == 1)
		{
			const int intensityPtrSize = sizeXY[k][0] * sizeXY[k][1];
			int *intensityPtr;
			intensityPtr = (int*)malloc(intensityPtrSize * sizeof(int));
			int* histogram;
			histogram = (int*)malloc(256 * sizeof(int));
			for (int i = 0; i < 256; i++)
			{
				*(histogram + i) = 0;
			}
			int m = 0;
			for (int i = startXY[k][0]; i < rows && i < startXY[k][0] + sizeXY[k][0]; i++)
			{
				for (int j = startXY[k][1]; j < columns && j < startXY[k][1] + sizeXY[k][1]; j++)
				{
					intensityPtr[m] = src.getPixel(i, j);
					m++;
				}
			}
			sort(intensityPtr, intensityPtr + intensityPtrSize);
			int minIntensity = intensityPtr[0];
			int maxIntensity = intensityPtr[intensityPtrSize - 1];
			
			

			for (int i = 0; i < intensityPtrSize; i++)
			{
				histogram[*(intensityPtr + i)]++;
			}


			image histogramOriginalFile, histogramNewFile;
			histogramOriginalFile.resize( 300 , *max_element(histogram, histogram + 256) + 50);
			for (int i = 0; i < 256; i++)
			{
				int tes = *(histogram + i);
				for (int j = 0; j < tes; j++)
				{
					histogramOriginalFile.setPixel(i, j, MAXRGB);
				}
			}
			histogramOriginalFile.save(HistoOrignal[k]);

			for (int i = startXY[k][0]; i < rows && i < startXY[k][0] + sizeXY[k][0]; i++)
			{
				for (int j = startXY[k][1]; j < columns && j < startXY[k][1] + sizeXY[k][1]; j++)
				{
					int temp = src.getPixel(i, j);
					int t = max[k];
					if (temp < min[k])
					{
						tgt.setPixel(i, j, MINRGB);
					}
					else if (temp > max[k])
					{
						tgt.setPixel(i, j, MAXRGB);
					}
					else
					{
						int newIntensity = (((temp - minIntensity) * (255 - 0)) / (maxIntensity - minIntensity)) + 0;
						if (newIntensity > 255)
							newIntensity = 255;
						if (newIntensity < 0)
							newIntensity = 0;
						tgt.setPixel(i, j, newIntensity);
					}
				}
			}
			m = 0;
			for (int i = startXY[k][0]; i < rows && i < startXY[k][0] + sizeXY[k][0]; i++)
			{
				for (int j = startXY[k][1]; j < columns && j < startXY[k][1] + sizeXY[k][1]; j++)
				{
					intensityPtr[m] = tgt.getPixel(i, j);
					m++;
				}
			}
			for (int i = 0; i < intensityPtrSize; i++)
			{
				histogram[*(intensityPtr + i)]++;
			}
			histogramNewFile.resize(300, *max_element(histogram, histogram + 256) + 50);
			for (int i = 0; i < 256; i++)
			{
				int tes = *(histogram + i);
				for (int j = 0; j < tes; j++)
				{
					histogramNewFile.setPixel(i, j, MAXRGB);
				}
			}
			histogramNewFile.save(HistoNew[k]);
		}
	}
}

void utility::histogramStreaching(image& src, image& tgt, int** startXY, int** sizeXY, int nROI)
{
	int rows = src.getNumberOfRows(), columns = src.getNumberOfColumns();
	tgt.resize(rows, columns);
	tgt.copyImage(src);
	for (int k = 0; k < nROI; k++)
	{
		if (startXY[k][2] == 1)
		{
			int intensityPtrSize = sizeXY[k][0] * sizeXY[k][1];
			int* intensityPtr;
			intensityPtr = (int*)malloc(intensityPtrSize * sizeof(int));
			int m = 0;
			for (int i = startXY[k][0]; i < rows && i < startXY[k][0] + sizeXY[k][0]; i++)
			{
				for (int j = startXY[k][1]; j < columns && j < startXY[k][1] + sizeXY[k][1]; j++)
				{
					intensityPtr[m] = src.getPixel(i, j);
					m++;
				}
			}
			sort(intensityPtr, intensityPtr + intensityPtrSize);
			int minIntensity = intensityPtr[0];
			int maxIntensity = intensityPtr[intensityPtrSize - 1];


			for (int i = startXY[k][0]; i < rows && i < startXY[k][0] + sizeXY[k][0]; i++)
			{
				for (int j = startXY[k][1]; j < columns && j < startXY[k][1] + sizeXY[k][1]; j++)
				{
					int temp = src.getPixel(i, j);
					
					
					int newIntensity =(((temp - minIntensity) * (255 - 0))/ (maxIntensity - minIntensity)) + 0;
					if (newIntensity > 255)
						newIntensity = 255;
					if (newIntensity < 0)
						newIntensity = 0;
					tgt.setPixel(i, j, newIntensity);
					
				}
			}
		}
	}
}

void utility::convertRGB2HSI(double* rgb, double* hsi)
{
	double r = rgb[0] / (rgb[0] + rgb[1] + rgb[2]);
	double g = rgb[1] / (rgb[0] + rgb[1] + rgb[2]);
	double b = rgb[2] / (rgb[0] + rgb[1] + rgb[2]);

	

	double rg = (r - g);
	double rb = (r - b);
	hsi[0] = acos((0.5 * rg * rb) / sqrt(rg * rg + rb * (g - b)));
	if (b > g)
	{
		hsi[0] = 2 * M_PI - hsi[0];
	}
	hsi[0] = hsi[0] * 180 / M_PI;

	hsi[1] = 1 - 3 * min({ r,g,b });
	hsi[1] = hsi[1] * 100;

	hsi[2] = (rgb[0] + rgb[1] + rgb[2]) / (3 * 255);
	hsi[2] = hsi[2] * 255;
}

void utility::convertHSI2RGB(double* rgb, double* hsi)
{
	hsi[0] = hsi[0] * M_PI / 180;
	hsi[1] = hsi[1] / 100;
	hsi[2] = hsi[2] / 255;

	double x = hsi[2] * (1 - hsi[1]);
	double y = hsi[2] * (1 + (hsi[1] * cos(hsi[0]) / cos((M_PI / 3) - hsi[0])));
	double z = 3 * hsi[2] - (x + y);

	if (hsi[0] < 2 * M_PI / 3)
	{
		rgb[0] = y;
		rgb[1] = z;
		rgb[2] = x;
	}
	else if(hsi[0] < 4 * M_PI / 3)
	{
		rgb[0] = x;
		rgb[1] = y;
		rgb[2] = z;
	}
	else if (hsi[0] < 2 * M_PI)
	{
		rgb[0] = z;
		rgb[1] = x;
		rgb[2] = y;
	}
	rgb[0] *= 255;
	rgb[1] *= 255;
	rgb[2] *= 255;
}

void utility::histogramStrechingHSI(image& src, image& tgt, int* startXY, int* sizeXY, bool channelFlag[3])
{
	int rows = src.getNumberOfRows(), columns = src.getNumberOfColumns();
	tgt.resize(rows, columns);
	tgt.copyImage(src);

	int intensityPtrSize = sizeXY[0] * sizeXY[1];
	double* intensityPtr, * intensityPtrS, * intensityPtrI;
	intensityPtr = (double*)malloc(intensityPtrSize * sizeof(double));
	intensityPtrS = (double*)malloc(intensityPtrSize * sizeof(double));
	intensityPtrI = (double*)malloc(intensityPtrSize * sizeof(double));
	int m = 0;
	for (int i = startXY[0]; i < rows && i < startXY[0] + sizeXY[0]; i++)
	{
		for (int j = startXY[1]; j < columns && j < startXY[1] + sizeXY[1]; j++)
		{
			double temp[] = { src.getPixel(i, j,0),src.getPixel(i, j,1),src.getPixel(i, j,2) };
			double tempHSI[3];
			convertRGB2HSI(temp, tempHSI);
			intensityPtr[m] = tempHSI[0];
			intensityPtrS[m] = tempHSI[1];
			intensityPtrI[m] = tempHSI[2];
			m++;
		}
	}
	double minIntensity, maxIntensity, minIntensityS, maxIntensityS, minIntensityI, maxIntensityI;
	if (channelFlag[0]) {
		sort(intensityPtr, intensityPtr + intensityPtrSize);
	}
	if (channelFlag[1] ) {
		sort(intensityPtrS, intensityPtrS + intensityPtrSize);
	}
	if (channelFlag[2]) {
		sort(intensityPtrI, intensityPtrI + intensityPtrSize);
	}
	minIntensity = intensityPtr[0];
	maxIntensity = intensityPtr[intensityPtrSize - 1];
	minIntensityS = intensityPtrS[0];
	maxIntensityS = intensityPtrS[intensityPtrSize - 1];
	minIntensityI = intensityPtrI[0];
	maxIntensityI = intensityPtrI[intensityPtrSize - 1];

	for (int i = startXY[0]; i < rows && i < startXY[0] + sizeXY[0]; i++)
	{
		for (int j = startXY[1]; j < columns && j < startXY[1] + sizeXY[1]; j++)
		{

			double temp[] = { src.getPixel(i, j,0),src.getPixel(i, j,1),src.getPixel(i, j,2) };
			double tempHSI[3];
			convertRGB2HSI(temp, tempHSI);
			if (channelFlag[0]) {
				double newIntensity = (((tempHSI[0] - minIntensity) * (360 - 0)) / (maxIntensity - minIntensity)) + 0;
				if (newIntensity > 360)
					newIntensity = 360;
				if (newIntensity < 0)
					newIntensity = 0;
				tempHSI[0] = newIntensity;
			}
			if (channelFlag[1]) {
				double newIntensity = (((tempHSI[1] - minIntensityS) * (100 - 0)) / (maxIntensityS - minIntensityS)) + 0;
				if (newIntensity > 100)
					newIntensity = 100;
				if (newIntensity < 0)
					newIntensity = 0;
				tempHSI[1] = newIntensity;
			}
			if (channelFlag[2]) {
				double newIntensity = (((tempHSI[2] - minIntensityI) * (255 - 0)) / (maxIntensityI - minIntensityI)) + 0;
				if (newIntensity > 255)
					newIntensity = 255;
				if (newIntensity < 0)
					newIntensity = 0;
				tempHSI[2] = newIntensity;
			}
			
			
			convertHSI2RGB(temp, tempHSI);
			tgt.setPixel(i, j, 0, temp[0]);
			tgt.setPixel(i, j, 1, temp[1]);
			tgt.setPixel(i, j, 2, temp[2]);

		}
	}
}





