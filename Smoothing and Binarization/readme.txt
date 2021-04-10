*** This project is built under Visual Studio 2019


This software is architectured as follows

iptool/iptool.cpp- This file includes the main function.

iptool/iptools -This folder hosts the files that are compiled into a static library. 
	image - This folder hosts the files that define an image.
	utility- this folder hosts the files that students store their implemented algorithms.



*** INSTALATION ***

On Windows

Open the project by double click iptool.sln.

*** FUNCTIONS ***

1. Binarize the ROI: roiBinarize
Binarize the pixels with the two threshold for every single ROI.

2. Smoothing ROI: smoothingROI
Implement the soomting on image using adaptive window for ROI

3. Color Binarization: colorbinarize
Binarisation of an Colored(RGB) image using color threshold and threshold distance.



*** PARAMETERS ***

-The first parameter of the parameters.txt is the number of region of interest and the number of operations (eg. 3 3 -> first one is ROI operation and second one is no of Op) (lines).
-The second line of parameters.txt is start coordinate ROI in x y pairs (eg 2 2 3 3  the two points are (2,2) and (3,3))
-the third line id the size of the ROI in x y pairs in pixel  (eg 100 100 150 200  the two points are (100,100) and (150,200))
-Fourth line is to check overlap or not  0 -> Not check and 1 -> check 
-Parameters for each operation (line):
  1. the input file name;
  2. the output file name;
  3. the name of the filter. Use "roiBinarize", "smoothingROI", and "colorbinarize" for your filters;
  4. Special parameters for roiBinarize and colorbinarize
-Special Parameters
  1. roiBinarize -> Two Threshold value for all ROI (eg. for 3 ROI 100 120 150 180 200 240  -> (TH1,TH2)=> (100,120) (150,180) (200,240))
  2. colorbinarize -> Threshold color in RGB and threshold distance (eg 170 109 73 50  -> (Thcolor)=> (170,109,73) & (THDistance)=>(50))


*** Run the program:

Directly debug in Visual Studio.
You can find the output image in output folder.


*** Important information ***

Application assumes the next format of input image (ppm/pgm) file:
line1: <version>
line2: <#columns> <#rows>
line3: <max_value>
line4-end-of-file:<pix1><pix2>...<pix_n>

if it is a grayscale image then every pixel is a char value. If it is a RGB image then every pixel is a set of 3 char values: <red><green><blue>

Thus, if you have a problem with reading image, the first thing you should check is input file format.
