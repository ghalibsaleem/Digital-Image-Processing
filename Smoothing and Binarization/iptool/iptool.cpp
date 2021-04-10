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
	image src, tgt;
	ifstream fp(argv[1]);
	char str[MAXLEN];
	
	char str2[MAXLEN];

	rsize_t strmax = sizeof str;
	char outfile[MAXLEN];
	char *pch, *next_pch;
	int nOP, nROI;

	
	int checkROI = 1;

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
	//int startXY[3][2], sizeXY[3][2];
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

	fp >> checkROI;
	int flag = 0;
	for (int OP = 0; OP < nOP; OP++) {
		fp >> str;
		src.read(str);
		
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
        if (strncmp(str,"add",3)==0) {
			/* Add Intensity */
			fp >> str;
        	utility::addGrey(src,tgt,atoi(str));
        }

        else if (strncmp(str,"binarize",8)==0) {
			/* Thresholding */
			fp >> str;
			fp >> str2;
			//TODO
			utility::binarize(src,tgt,atoi(str), atoi(str2));
		}

		else if (strncmp(str,"scale",5)==0) {
			/* Image scaling */
			fp >> str;
			utility::scale(src,tgt,atof(str));
		}
		else if (strncmp(str, "roiBinarize",11) == 0) {
			/* Binarize ROI */
			
			int *threshold1, *threshold2;
			threshold1 = (int*)malloc(nROI * sizeof(int));
			threshold2 = (int*)malloc(nROI * sizeof(int));
			for (int i = 0; i < nROI; i++)
			{
				fp >> (threshold1[i]);
				fp >> (threshold2[i]);
			}
			
			utility::binarizeROI(src, tgt, threshold1, threshold2, startXY, sizeXY, nROI);
		}
		else if (strncmp(str, "smoothingROI", 12) == 0) {
			/* Image ROI smoothing */
			utility::smoothingROI(src, tgt, startXY, sizeXY, nROI);
		}
		else if (strncmp(str, "colorbinarize", 13) == 0) {
			/* Image ROI color thresholding */
			int *thresholdColor,thresholdDist = 0;
			thresholdColor = (int*)malloc(nROI * sizeof(int));
			for (int i = 0; i < 3; i++)
			{
				fp >> thresholdColor[i];
			}
			fp >> thresholdDist;
			utility::colorBinarization(src, tgt, startXY, sizeXY, nROI, thresholdDist, thresholdColor);
		}
		else {
			printf("No function: %s\n", str);
			continue;
		}
       
		tgt.save(outfile);
	}
	//fclose(fp);
	fp.close();
	return 0;
}

