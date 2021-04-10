#include "utility.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

#define MAXRGB 255
#define MINRGB 0

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
			int i2 = (int)floor((float)i/ratio);
			int j2 = (int)floor((float)j/ratio);
			if (ratio == 2) {
				tgt.setPixel(i,j,checkValue(src.getPixel(i2,j2)));
			}

			if (ratio == 0.5) {
				int value = src.getPixel(i2,j2) + src.getPixel(i2,j2+1) + src.getPixel(i2+1,j2) + src.getPixel(i2+1,j2+1);
				tgt.setPixel(i,j,checkValue(value/4));
			}
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::cv_gray(Mat &src, Mat &tgt)
{
	cvtColor(src, tgt, COLOR_BGR2GRAY);
}

/*-----------------------------------------------------------------------**/
void utility::cv_avgblur(Mat &src, Mat &tgt, int WindowSize)
{
	blur(src,tgt,Size(WindowSize,WindowSize));
}

/*-----------------------------------------------------------------------**/
void utility::cv_dft(Mat &src, Mat &tgt)
{
	double minV, maxV;
	minMaxLoc(src, &minV, &maxV);

	Mat planes[] = { Mat_<float>(src), Mat::zeros(src.size(), CV_32F) };
	Mat complexI;
	merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

	//dft(complexI, tgt);            // DFT
	dft(complexI, tgt, DFT_INVERSE + DFT_REAL_OUTPUT);	// Inverse DFT

	normalize(tgt, tgt, minV, maxV, NORM_MINMAX);
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

void utility::cvDFTFreq(cv::Mat& src, cv::Mat& tgt, string path, int writeFlag)
{
	double minV, maxV;
	minMaxLoc(src, &minV, &maxV);
	Mat planes[] = { Mat_<float>(src), Mat::zeros(src.size(), CV_32F) };
	Mat complexI;
	merge(planes, 2, complexI);
	dft(complexI, complexI);
	split(complexI, planes);
	Mat magnitudeImage;
	magnitude(planes[0], planes[1], magnitudeImage);
	magnitudeImage += Scalar::all(1);
	log(magnitudeImage, magnitudeImage);
	quadShift(magnitudeImage);
	normalize(magnitudeImage, tgt, 0, 255, NORM_MINMAX);
	if (writeFlag != 0) {
		imwrite(path + "FreqImg.jpg", tgt);
	}
}



void utility::FilterFreq(cv::Mat& src, int filterType, int** startXY, int** sizeXY, int nROI, int thresholdV[3], string outPath)
{
	Mat tgt;
	cvDFTFreq(src, tgt, outPath, 1);
	src.copyTo(tgt);
	for (int k = 0; k < nROI; k++)
	{
		Mat roi = src(Rect(startXY[k][0], startXY[k][1], sizeXY[k][0], sizeXY[k][1]));
		cvDFTFreq(roi, roi,"" ,0);
		
		
		roi.copyTo(tgt(Rect(startXY[k][0], startXY[k][1], sizeXY[k][0], sizeXY[k][1])));
		
		for (int i = 0; i < roi.rows; i++)
		{
			for (int j = 0; j < roi.cols; j++)
			{
				Point a(i, j);
				Point b(roi.rows / 2, roi.cols / 2);
				double dist = norm(a - b);
				if (chooseFilter(filterType, dist, thresholdV)) {
				//if (dist > thresholdV[0]) {
					roi.at<int>(i, j) = 0;
				}

			}
		}
		roi.copyTo(src(Rect(startXY[k][0], startXY[k][1], sizeXY[k][0], sizeXY[k][1])));
	}
	imwrite(outPath + "withFltr.jpg", src);
	imwrite(outPath + "noFltr.jpg", tgt);
}

void utility::cvDFT(cv::Mat& src, cv::Mat& tgt, int thresholdV[3], int filterType)
{
	double minV, maxV;
	minMaxLoc(src, &minV, &maxV);
	Mat planes[] = { Mat_<float>(src), Mat::zeros(src.size(), CV_32F) };
	Mat complexI;
	merge(planes, 2, complexI);

	dft(complexI, complexI);
	quadShift(complexI);
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			Point a(i, j);
			Point b(src.rows / 2, src.cols / 2);
			double dist = norm(a - b);
			if (chooseFilter (filterType,dist,thresholdV)) {
				complexI.at<Vec2i>(i, j) = Vec2i(0,0);
			}
			
		}
	}
	quadShift(complexI);
	dft(complexI, complexI, DFT_INVERSE + DFT_REAL_OUTPUT);
	normalize(complexI, tgt, minV, maxV, NORM_MINMAX);
}

void utility::Filter(cv::Mat& src, cv::Mat& tgt, int filterType, int** startXY, int** sizeXY, int nROI, int thresholdV[3], string outPath)
{
	utility::cv_gray(src, src);
	Mat temp;
	src.copyTo(temp);
	FilterFreq(temp, filterType, startXY, sizeXY, nROI, thresholdV, outPath);
	for (int k = 0; k < nROI; k++)
	{
		Mat roi = src(Rect(startXY[k][0], startXY[k][1], sizeXY[k][0], sizeXY[k][1])),temp;
		cvDFT(roi, temp, thresholdV, filterType);
		temp.copyTo(src(Rect(startXY[k][0], startXY[k][1], sizeXY[k][0], sizeXY[k][1])));
	}
	tgt = src;
}

void utility::quadShift(cv::Mat& src)
{
	// This implementation quadrant shift is from official OpenCV documentation
	//https://docs.opencv.org/2.4/doc/tutorials/core/discrete_fourier_transform/discrete_fourier_transform.html

	int cx = src.cols / 2.0;
	int cy = src.rows / 2.0;

	Mat q0(src, Rect(0, 0, cx, cy));   
	Mat q1(src, Rect(cx, 0, cx, cy));  
	Mat q2(src, Rect(0, cy, cx, cy));  
	Mat q3(src, Rect(cx, cy, cx, cy)); 

	Mat tmp;                          
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);                    
	q2.copyTo(q1);
	tmp.copyTo(q2);
}

bool utility::chooseFilter(int filterType, double dist, int th1[3])
{
	if (filterType == 0) // Low Pass
	{
		if (dist > th1[0])
			return true;
		else
			return false;
	}
	if (filterType == 1) // High Pass
	{
		if (dist < th1[0])
			return true;
		else
			return false;
	}
	if (filterType == 2) // Band Pass
	{
		if (dist < th1[0] || dist > th1[1])
			return true;
		else
			return false;
	}
	if (filterType == 3) // Band Stop or Notch
	{
		if (dist > th1[0] && dist < th1[1])
			return true;
		else
			return false;
	}
	if (filterType == 4) // Combine
	{
		//TODO
		if ((dist > th1[0] && dist < th1[1]) && dist > th1[3])
			return true;
		else
			return false;
	}
	return false;
}


