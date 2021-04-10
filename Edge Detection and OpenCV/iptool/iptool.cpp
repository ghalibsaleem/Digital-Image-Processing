#include "./iptools/core.h"
#include <string.h>
#include <fstream>

using namespace std;

#define MAXLEN 256

int main (int argc, char** argv)
{
	image src, tgt;
	ifstream fp(argv[1]);
	char str[MAXLEN];
	rsize_t strmax = sizeof str;
	char outfile[MAXLEN];
	char *pch, *next_pch;
	int nOP,nROI;
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
		src.read(str);
		char srcPath[MAXLEN];

		strcpy_s(srcPath, MAXLEN, str);

		if (flag == 0)
		{
			bool IsValid = utility::checkROIOverlap(src, startXY, sizeXY, nROI, checkROI);
			if (!IsValid)
				exit(1);
			flag = 1;
		}

		fp >> str;
		strcpy_s(outfile, MAXLEN, str);

		fp >> str;
        if (strncmp(str,"edgenative",10)==0) {
			/* Native Edge Detection */
			int threshold[2], baseTh, sobelOp;
			fp >> baseTh;
			fp >> threshold[0];
			fp >> threshold[1];
			fp >> sobelOp;
			int** intensityImg = utility::GradientToIntensityImg(src, baseTh, sobelOp, outfile, 0, nullptr);
			utility::EdgeDetection(src, intensityImg, startXY, sizeXY, nROI, threshold, outfile);
			
			utility::cvEdgeDetection(srcPath, baseTh, sobelOp, startXY, sizeXY, nROI, threshold, outfile);
        }

		else if (strncmp(str, "edgeangle", 10) == 0) {
			/* Edge Detection at some angle*/
			int threshold, angleTh[2], sobelOp;
			fp >> threshold;
			fp >> angleTh[0];
			fp >> angleTh[1];
			fp >> sobelOp;
			int** intensityImg = utility::GradientToIntensityImg(src, threshold, sobelOp, outfile, 1, angleTh);
		}
		else if (strncmp(str, "cvcanny", 7) == 0) {
			/* Edge Detection at some angle*/
			int minthreshold, maxthreshold;
			fp >> minthreshold;
			fp >> maxthreshold;
			utility::cvCannyOperation(srcPath, minthreshold,maxthreshold, outfile);
		}
		else if (strncmp(str, "histequ", 13) == 0) {
			utility::cvHistogramEqu(srcPath, outfile);
		}
		else if (strncmp(str, "otsualgo", 8) == 0) {
			utility::cvOTSUAlgo(srcPath, outfile);
		}
		else if (strncmp(str, "histotsu", 8) == 0) {
			utility::cvHistogramStrechingOTSU(srcPath, outfile);
		}
		else {
			printf("No function: %s\n", str);
			continue;
		}
       
		//tgt.save(outfile);
	}
	//fclose(fp);
	fp.close();
	return 0;
}

