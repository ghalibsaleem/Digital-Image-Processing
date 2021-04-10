#define _USE_MATH_DEFINES
#include "utility.h"
#include <math.h>
#include<string.h>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#define MAXRGB 255
#define MINRGB 0
using namespace std;
using namespace cv;

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



int utility::sobelX3[3][3] = { {-1,0,1},
								  {-2,0,2},
								  {-1,0,1} };
int utility::sobelY3[3][3] = { {1,2,1},
								{0,0,0},
								{-1,-2,-1} };

int utility::sobelX5[5][5] = {  {-4,-5,0,5,4},
								{-8,-10,0,10,8},
								{-10,-20,0,20,10},
								{-8,-10,0,10,8},
								{-4,-5,0,5,4} };
int utility::sobelY5[5][5] = { {4,8,10,8,4},
								{5,10,20,10,5},
								{0,0,0,0,0},
								{-5,-10,-20,-10,-5},
								{-4,-8,-10,-8,-4} };

string utility::strMerge(char* str1, string str2)
{
	
	string output(str1);
	
	return output.append(str2);
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

int** utility::GradientToIntensityImg(image& src, int threshold, int sobelOperator, char* outLocation, int angleFlag, int angleTH[2])
{
	image tgt;
	int rows = src.getNumberOfRows();
	int columns = src.getNumberOfColumns();
	tgt.resize(rows, columns);

	int** intensityImg, ** angleImg;

	int** windowPxl, ** sobelX, ** sobelY;
	windowPxl = (int**)malloc(sobelOperator * sizeof(int*));
	sobelX = (int**)malloc(sobelOperator * sizeof(int*));
	sobelY = (int**)malloc(sobelOperator * sizeof(int*));
	for (int i = 0; i < sobelOperator; i++)
	{
		*(windowPxl + i) = (int*)malloc(sobelOperator * sizeof(int));
		*(sobelX + i) = (int*)malloc(sobelOperator * sizeof(int));
		*(sobelY + i) = (int*)malloc(sobelOperator * sizeof(int));
	}
	for (int i = 0; i < sobelOperator; i++)
	{
		for (int j = 0; j < sobelOperator; j++)
		{
			sobelX[i][j] = (sobelOperator == 3) ? sobelX3[i][j] : sobelX5[i][j];
			sobelY[i][j] = (sobelOperator == 3) ? sobelY3[i][j] : sobelY5[i][j];
		}
	}

	intensityImg = (int**)malloc(rows * sizeof(int*));

	angleImg = (int**)malloc(rows * sizeof(int*));
	for (int i = 0; i < rows; i++)
	{
		*(intensityImg + i) = (int*)malloc(columns * sizeof(int));
		if (angleFlag > 0)
			*(angleImg + i) = (int*)malloc(columns * sizeof(int));
	}
	int max = 0;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			int gX = 0, gY = 0;


			int x = 0, y = 0;
			for (int a = i - sobelOperator / 2; x < sobelOperator; a++)
			{
				y = 0;
				for (int b = j - sobelOperator / 2; y < sobelOperator; b++)
				{
					if (a < 0 || b < 0 || a >= rows || b >= columns)
					{
						*(*(windowPxl + x) + y) = 0;
					}
					else
					{
						*(*(windowPxl + x) + y) = src.getPixel(a, b);
					}
					y++;
				}
				x++;
			}


			for (int a = 0; a < sobelOperator; a++)
			{
				for (int b = 0; b < sobelOperator; b++)
				{
					gX += *(*(windowPxl + a) + b) * sobelX[a][b];
					gY += *(*(windowPxl + a) + b) * sobelY[a][b];
				}
			}


			*(*(intensityImg + i) + j) = sqrt(gX * gX + gY * gY);
			if (*(*(intensityImg + i) + j) > max)
				max = *(*(intensityImg + i) + j);


			if (angleFlag > 0) {
				if (gX != 0) {
					*(*(angleImg + i) + j) = atan(((double)gY / (double)gX)) * 180.0 / M_PI;
				}
				else {
					if (gY > 0)
						*(*(angleImg + i) + j) = 90;
					else
						*(*(angleImg + i) + j) = 270;
				}


			}
		}
	}

	image atgt1, atgt2;
	if (angleFlag > 0)
	{
		atgt1.resize(rows, columns);
		atgt2.resize(rows, columns);
	}


	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			if (max != 0)
			{
				int newPxl;
				if (max > 255) {
					newPxl = *(*(intensityImg + i) + j) * 255 / max;
					*(*(intensityImg + i) + j) = newPxl;
				}
				else
					newPxl = *(*(intensityImg + i) + j);
				if (newPxl > 50)
				{
					tgt.setPixel(i, j, newPxl);
				}

				if (angleFlag > 0)
				{
					if (*(*(angleImg + i) + j) > angleTH[0] - 10 && *(*(angleImg + i) + j) < angleTH[0] + 10)
					{
						atgt1.setPixel(i, j, newPxl);
					}
					if (*(*(angleImg + i) + j) > angleTH[1] - 10 && *(*(angleImg + i) + j) < angleTH[1] + 10)
					{
						atgt2.setPixel(i, j, newPxl);
					}
				}
			}
		}
	}

	if (angleFlag > 0)
	{
		tgt.save(strMerge(outLocation, "AngleIntensityImg.pgm").c_str());
		atgt1.save(strMerge(outLocation, "AngleImg1.pgm").c_str());
		atgt2.save(strMerge(outLocation, "AngleImg2.pgm").c_str());
	}
	else {
		tgt.save(strMerge(outLocation, "intensityImg.pgm").c_str());
	}
	return intensityImg;
}

void utility::EdgeDetection(image& src, int** intensityImg, int** startXY, int** sizeXY, int nROI, int threshold[2], char *outLocation)
{
	image tgt1, tgt2;
	int rows = src.getNumberOfRows();
	int columns = src.getNumberOfColumns();
	tgt1.resize(rows, columns);
	tgt2.resize(rows, columns);
	tgt1.copyImage(src);
	tgt2.copyImage(src);

	for (int k = 0; k < nROI; k++)
	{
		if (startXY[k][2] == 1) {
			for (int i = startXY[k][0]; i < rows && i < startXY[k][0] + sizeXY[k][0]; i++)
			{
				for (int j = startXY[k][1]; j < columns && j < startXY[k][1] + sizeXY[k][1]; j++)
				{

					if (intensityImg[i][j] >= threshold[0])
						tgt1.setPixel(i, j, intensityImg[i][j]);
					else
						tgt1.setPixel(i, j, MINRGB);

					if (intensityImg[i][j] >= threshold[1])
						tgt2.setPixel(i, j, intensityImg[i][j]);
					else
						tgt2.setPixel(i, j, MINRGB);

					

				}
			}
			
		}
	}
	
	tgt1.save(strMerge(outLocation,"Threshold1.pgm").c_str());
	tgt2.save(strMerge(outLocation, "Threshold2.pgm").c_str());
	
}

void utility::cvEdgeDetection(char* imgLocation, int thresholdImg, int sobelOperator, int** startXY, int** sizeXY, int nROI, int thresholdArr[2], char* outLocation)
{
	Mat src = imread(imgLocation);
	Mat gradx, grady,grad, finalgrad;

	int tempDepth = CV_32F, origDept = CV_8U;

	if (!src.data)
		return;

	Sobel(src, gradx, tempDepth, 1, 0, sobelOperator, 1, 0, BORDER_DEFAULT);
	Sobel(src, grady, tempDepth, 0, 1, sobelOperator, 1, 0, BORDER_DEFAULT);
	magnitude(gradx, grady, grad);
	normalize(grad, grad, 0, 255, NORM_MINMAX);
	threshold(grad, finalgrad, thresholdImg, MAXRGB, 3);
	imwrite(strMerge(outLocation, "CVintensityImg.png"), finalgrad);
	for (int i = 0; i < 2; i++)
	{
		Mat copyImg = src.clone();
		for (int k = 0; k < nROI; k++)
		{
			Mat roi = copyImg(Rect(startXY[k][0], startXY[k][1], sizeXY[k][0], sizeXY[k][1]));
			Mat Gx, Gy, GFinal;
			Sobel(roi, Gx, tempDepth, 1, 0, sobelOperator, 1, 0, BORDER_DEFAULT);
			Sobel(roi, Gy, tempDepth, 0, 1, sobelOperator, 1, 0, BORDER_DEFAULT);
			magnitude(Gx, Gy, GFinal);
			normalize(GFinal, GFinal, 0, 255, NORM_MINMAX);
			threshold(GFinal, GFinal, thresholdArr[i], MAXRGB, 3);
			GFinal.copyTo(roi);
		}
		string s = "CVThreshold" + to_string(i) + ".png";
		//imwrite(strMerge(outLocation, s), copyImg);
	}
	
}

void utility::cvCannyOperation(char* imgLocation, int minThreshold, int maxThreshold, char* outLocation)
{
	Mat src = imread(imgLocation),outImg;
	Canny(src, outImg, minThreshold, maxThreshold);
	imwrite(strMerge(outLocation, "CannyEdge.png"), outImg);
}

void utility::cvHistogramEqu(char* imgLocation, char* outLocation)
{
	Mat src = imread(imgLocation, CV_8UC1);
	Mat outImg;
	equalizeHist(src, outImg);
	imwrite(strMerge(outLocation, "HistoEQ.png"), outImg);
}

double utility::cvOTSUAlgo(char* imgLocation, char* outLocation)
{
	Mat src = imread(imgLocation, CV_8UC1);
	Mat outImg;
	int finalOut = threshold(src,outImg, 0, MAXRGB, THRESH_BINARY | THRESH_OTSU);
	imwrite(strMerge(outLocation, "OTSUAlgo.png"), outImg);
	return finalOut;
}

void utility::cvHistogramStrechingOTSU(char* imgLocation, char* outLocation){
	Mat src = imread(imgLocation, CV_8UC1);
	Mat outImg, outImg1, outImg2;
	double th = threshold(src, outImg1, 0, MAXRGB, THRESH_BINARY | THRESH_OTSU);
	threshold(src, outImg1, th, MAXRGB, THRESH_TOZERO);
	threshold(src, outImg2, th, MAXRGB, THRESH_TOZERO_INV);
	equalizeHist(outImg1, outImg1);
	addWeighted(outImg1, 1, outImg2, 1, 0, outImg);
	//normalize(outImg, outImg, 0, 255, NORM_MINMAX);
	imwrite(strMerge(outLocation, "HistoOTSU.png"), outImg);
}



