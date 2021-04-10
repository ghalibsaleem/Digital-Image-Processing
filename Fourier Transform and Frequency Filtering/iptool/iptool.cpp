/************************************************************
 *															*
 * This sample project include three functions:				*
 * 1. Add intensity for gray-level image.					*
 *    Input: source image, output image name, value			*
 *															*
 * 2. Image thresholding: pixels will become black if the	*
 *    intensity is below the threshold, and white if above	*
 *    or equal the threhold.								*
 *    Input: source image, output image name, threshold		*
 *															*
 * 3. Image scaling: reduction/expansion of 2 for 			*
 *    the width and length. This project uses averaging 	*
 *    technique for reduction and pixel replication			*
 *    technique for expansion.								*
 *    Input: source image, output image name, scale factor	*
 *															*
 ************************************************************/

#include "./iptools/core.h"
#include <string.h>
#include <fstream>

using namespace std;

#define MAXLEN 256

int main (int argc, char** argv)
{
	ifstream fp(argv[1]);
	char str[MAXLEN];
	char infile[MAXLEN];
	char outfile[MAXLEN];
	int nOP, nROI;
	if (!fp.is_open()) {
		fprintf(stderr, "Can't open file: %s\n", argv[1]);
		exit(1);
	}

	fp >> nROI;
	fp >> nOP;

	if (nROI < 3)
	{
		cout << "No of ROI should be more than 2";
		exit(1);
	}

	int** startXY = (int**)malloc(nROI * sizeof(int*));
	int** sizeXY = (int**)malloc(nROI * sizeof(int*));

	//Get ROI
	for (int i = 0; i < nROI; i++)
	{
		*(startXY + i) = (int*)malloc(3 * sizeof(int));
		for (int j = 0; j < 2; j++)
		{
			fp >> (startXY[i][j]);
		}
		startXY[i][2] = 1;
	}
	for (int i = 0; i < nROI; i++)
	{
		*(sizeXY + i) = (int*)malloc(3 * sizeof(int));
		for (int j = 0; j < 2; j++)
		{
			fp >> (sizeXY[i][j]);
		}
		sizeXY[i][2] = 1;
	}
	int checkROI = 1;
	fp >> checkROI;
	int flag = 0;

	for (int OP = 0; OP < nOP; OP++) {

		
		fp >> str;
		strcpy_s(infile, MAXLEN, str);
		if (flag == 0)
		{
			image src;
			src.read(str);
			bool IsValid = utility::checkROIOverlap(src, startXY, sizeXY, nROI, checkROI);
			if (!IsValid)
				exit(1);
			flag = 1;
		}
		fp >> str;
		strcpy_s(outfile, MAXLEN, str);
		
		fp >> str;
		if (strncmp(str, "opencv", 6) == 0) {
			cv::Mat src = cv::imread(infile);
			cv::Mat tgt;
			if (src.empty()) {
				cout << "Could not open or find the image: " << infile << endl;
				return -1;
			}

			fp >> str;
			if (strncmp(str, "highpass", 8) == 0) {
				int threshold[1];
				fp >> threshold[0];
				utility::Filter(src, tgt, 1, startXY, sizeXY, nROI, threshold, outfile);
			}
			else if (strncmp(str, "lowpass", 7) == 0) {
				int threshold[1];
				fp >> threshold[0];
				utility::Filter(src, tgt,0, startXY, sizeXY, nROI, threshold, outfile);
			}
			else if (strncmp(str, "bandpass", 8) == 0) {
				int threshold[2];
				fp >> threshold[0];
				fp >> threshold[1];
				utility::Filter(src, tgt, 2, startXY, sizeXY, nROI, threshold, outfile);
			}
			else if (strncmp(str, "notch", 5) == 0) {
				int threshold[2];
				fp >> threshold[0];
				fp >> threshold[1];
				utility::Filter(src, tgt, 3, startXY, sizeXY, nROI, threshold, outfile);
			}
			else if (strncmp(str, "combine", 7) == 0) {
				int threshold[3];
				fp >> threshold[0];
				fp >> threshold[1];
				fp >> threshold[2];
				utility::Filter(src, tgt, 4, startXY, sizeXY, nROI, threshold, outfile);
			}
			else {
				printf("No function: %s\n", str);
				continue;
			}
			string asd = outfile;
			cv::imwrite(asd + "final.jpg", tgt);
			//cv::imwrite("../output/Outputtree_t1.jpg", tgt);
			
		}
		else {
			image src, tgt;
			src.read(infile);
			if (strncmp(str, "add", 3) == 0) {
				/* Add Intensity */
				fp >> str;
				utility::addGrey(src, tgt, atoi(str));
			}

			else if (strncmp(str, "binarize", 8) == 0) {
				/* Thresholding */
				fp >> str;
				utility::binarize(src, tgt, atoi(str));
			}

			else if (strncmp(str, "scale", 5) == 0) {
				/* Image scaling */
				fp >> str;
				utility::scale(src, tgt, atof(str));
			}

			else {
				printf("No function: %s\n", str);
				continue;
			}

			tgt.save(outfile);
		}
	}
	//fclose(fp);
	fp.close();
	return 0;
}

