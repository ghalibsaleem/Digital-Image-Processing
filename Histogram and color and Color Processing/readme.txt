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

1. Optimal Threshold: optimalThreshold
Binarize the pixels with the threshold calculated by optimal threshold algorithm

2. HistogramStreching: histogramStreching
It is used to increse the contrast of an image

3. Combine : combine
Combined operation of optimal threshold and Histogram Streching

4.Histogram Streching of colored image: hsiexp
Hitogram Streching of a Colored Image IN HSI



*** PARAMETERS ***

-The first parameter of the parameters.txt is the number of region of interest and the number of operations (eg. 3 4 -> first one is ROI operation and second one is no of Op) (lines).
-The second line of parameters.txt is start coordinate ROI in x y pairs (eg 2 2 3 3  the two points are (2,2) and (3,3))
-the third line id the size of the ROI in x y pairs in pixel  (eg 100 100 150 200  the two points are (100,100) and (150,200))
-Fourth line is to check overlap or not  0 -> Not check and 1 -> check 
-Parameters for each operation (line):
  1. the input file name;
  2. the output file name;
  3. the name of the filter. Use "roiBinarize", "smoothingROI", and "colorbinarize" for your filters;
  4. Special parameters for that perticular operation
-Special Parameters
  1. optimalThreshold ->The user specified limit should be provided eg. (5)
  2. histogramStreching -> The user specified a,b values in pairs for each ROI eg(100 200 100 200 100 200) => (a,b)=> (100,200)(100,200)(100,200)
                           and next three lines for original and new histogram location
  3.combine -> for this operation the first number is the limit for optional threshold and after that alocation is to be provided for combine background image
 		after that the ROI parameter are provided like startx starty sizex sizey
		in new line the location ato be provided for combine foreground, combine background histogram streched and combine foreground histogram streched.

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
