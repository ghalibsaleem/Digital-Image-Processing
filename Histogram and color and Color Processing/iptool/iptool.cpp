

#include "./iptools/core.h"
#include <string.h>
#include <fstream>

using namespace std;

#define MAXLEN 256

int main(int argc, char** argv)
{
	image src, tgt;
	ifstream fp(argv[1]);
	char str[MAXLEN];

	char str2[MAXLEN];

	rsize_t strmax = sizeof str;
	char outfile[MAXLEN];
	char* pch, * next_pch;
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
		if (strncmp(str, "combine", 7) == 0) {
			fp >> str;
			int limit = atof(str);
			char bghst[MAXLEN];

			char fghst[MAXLEN];
			nROI = 1;
			fp >> str2; // background

			image tgt1, tgt2;
			fp >> (startXY[0][0]);
			fp >> (startXY[0][1]);
			fp >> (sizeXY[0][0]);
			fp >> (sizeXY[0][1]);
			fp >> str; //Foreground
			fp >> bghst;
			fp >> fghst;
			startXY[0][2] = 1;
			sizeXY[0][2] = 1;
			utility::OptimalThresholding(src, tgt, tgt1, tgt2, limit, startXY, sizeXY, nROI);
			tgt1.save(str2);
			tgt2.save(str);
			image finalbg, finalfg;
			utility::histogramStreaching(tgt1, finalbg, startXY, sizeXY, nROI);
			utility::histogramStreaching(tgt2, finalfg, startXY, sizeXY, nROI);
			finalbg.save(bghst);
			finalfg.save(fghst);
		}

		else if (strncmp(str, "histogramStreching", 18) == 0) {
			
			int *min,*max;
			max = (int*)malloc(nROI * sizeof(int));
			min = (int*)malloc(nROI * sizeof(int));
			for (int i = 0; i < nROI; i++)
			{
				fp >> *(min+i);
				fp >> *(max+i);
			}
			char** HistoOrignal = (char**)malloc(nROI * sizeof(char*));
			char** HistoNew = (char**)malloc(nROI * sizeof(char*));
			for (int i = 0; i < nROI; i++)
			{
				*(HistoOrignal + i) = (char*)malloc (MAXLEN * sizeof(char));
				fp >> *(HistoOrignal + i);
				*(HistoNew + i) = (char*)malloc (MAXLEN * sizeof(char));
				fp >> *(HistoNew + i);
			}
			
			utility::histogramStreaching(src,tgt, startXY, sizeXY, nROI,min,max,HistoOrignal,HistoNew);
		}

		else if (strncmp(str, "optimalThreshold", 16) == 0) {
			
			fp >> str;
			utility::OptimalThresholding(src, tgt, atof(str),startXY,sizeXY,nROI);
		}
		else if (strncmp(str, "hsiexp", 6) == 0) {
			int flags[3];
			bool channelFlags[3];
			fp >> (startXY[0][0]);
			fp >> (startXY[0][1]);
			fp >> (sizeXY[0][0]);
			fp >> (sizeXY[0][1]);
			fp >> flags[0];
			fp >> flags[1];
			fp >> flags[2];
			for (int i = 0; i < 3; i++)
			{
				if (flags[i] == 1)
					channelFlags[i] = true;
				else
					channelFlags[i] = false;
			}
			utility::histogramStrechingHSI(src, tgt, *startXY, *sizeXY,channelFlags);
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

