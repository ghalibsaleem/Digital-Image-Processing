*** This project is built under Visual Studio 2015


This software is architectured as follows

iptool/iptool.cpp- This file includes the main function.

iptool/iptools -This folder hosts the files that are compiled into a static library. 
	image - This folder hosts the files that define an image.
	utility- this folder hosts the files that students store their implemented algorithms.



*** INSTALATION ***

On Windows

Open the project by double click iptool.sln.

*****************************************************************************************
* I was having some error with $(OPENCV_DIR) in project setup even after creating a user*
* path in envirometal variable. So I Specified the OpenCV path instead of $(OPENCV_DIR) *
* You might get error due to different installation location so you have to change the  *
* opencv location according to you installation location path                           *
*****************************************************************************************


*** FUNCTIONS ***

1. Edge detetion w/o Open CV: edgenative
Edge detetion using Sobel operator and without OpenCV this operation also makes call for OpenCV Sobel function
so that both of them have the same image and paarameter for comparison

2. Edge detetion Angle threshold w/o Open CV: edgeangle
Edge detetion using Sobel operator and angle threshold without OpenCV

3. Canny Edge detection: cvcanny
This operation is for canny edge detection by Open CV

4. Histogram Equalization: histequ
This operation is for Histogram Equalisation by Open CV

5. OTSU thresholding: otsualgo
This operation is for canny OTSU thresholding by Open CV

6. Histogram Eqalization to the foreground of OTSU: histotsu
This operation is for Histogram Equalization the foreground of image as determined by OTSU thresholding by Open CV



*** PARAMETERS ***

-The first parameter of the parameters.txt is the number of region of interest and the number of operations (eg. 3 4 -> first one is ROI operation and second one is no of Op) (lines).
-The second line of parameters.txt is start coordinate ROI in x y pairs (eg 2 2 3 3  the two points are (2,2) and (3,3))
-the third line id the size of the ROI in x y pairs in pixel  (eg 100 100 150 200  the two points are (100,100) and (150,200))
-Fourth line is to check overlap or not  0 -> Not check and 1 -> check 
-Parameters for each operation (line):
  1. the input file name;
  2. the output file name;
  3. the name of the Operation. Use "edgenative", "edgeangle" as described above;
  4. Special parameters for that perticular operation

eg. src\File\Loc dst\FIle\Loc oprationName 23 45 12

{23 45 12} -> Special Parameter


-Special Parameters
 1. edgenative -> a b c d -> a= Intesity Image Threshold, b = Th1 for Image, c= Th2 for Image, d= Sobel Kernel Size
 2. edgeangle -> a b c d -> a -> threshold for image(sobel), b= AngleTh1 for Image, c=AngleTh2 for Image, d=sobel kernel Size
 3. cvcanny -> a b -> a = canny minimum threshold, b = canny maximum threshold


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
