*** This project is built under Visual Studio 2019 with OpenCV 4+ under Windows 10 64-bit


This software is architectured as follows

iptool/iptool.cpp- This file includes the main function.

iptool/iptools -This folder hosts the files that are compiled into a static library. 
	image - This folder hosts the files that define an image.
	utility- this folder hosts the files that students store their implemented algorithms.



*** INSTALATION ***

On Windows

Open the project by double click iptool.sln.
The environment is setting follow the link:
http://opencv-srf.blogspot.com/2013/05/installing-configuring-opencv-with-vs.html
*****************************************************************************************
* I was having some error with $(OPENCV_DIR) in project setup even after creating a user*
* path in envirometal variable. So I Specified the OpenCV path instead of $(OPENCV_DIR) *
* You might get error due to different installation location so you have to change the  *
* opencv location according to you installation location path                           *
*****************************************************************************************




*** Run the program:

Directly debug in Visual Studio.
You can find the output image in output folder.


*** FUNCTIONS ***

1. Low Pass Filter: lowpass
This function perform smoothing or Low pass filtering in Fourier domain
parameter: threshold distance for circle

2. High Pass Filter: highpass
This function perform Edge detection or High pass filtering in Fourier domain
parameter: threshold distance for circle

3. Band Pass Filter: bandpass
This function perform Edge Enhancement or Band pass filtering in Fourier domain
parameter: Two threshold distance for circles

4. Notch Filter: notch
This function perform Notch filtering in Fourier domain
parameter: Two threshold distance for circles

5. Combine Notch Filter and Low Pass Filter: combine
This function perform combined Notch filtering and Low Pass Filteering in Fourier domain
parameter: Three threshold distance for circles

*** Input for operation *****
A PGM Image type

*** PARAMETERS ***

-The first parameter of the parameters.txt is the number of region of interest and the number of operations (eg. 3 4 -> first one is ROI operation and second one is no of Op) (lines).
-The second line of parameters.txt is start coordinate ROI in x y pairs (eg 2 2 3 3  the two points are (2,2) and (3,3))
-the third line id the size of the ROI in x y pairs in pixel  (eg 100 100 150 200  the two points are (100,100) and (150,200))
-Fourth line is to check overlap or not  0 -> Not check and 1 -> check 
-Parameters for each operation (line):
  1. the input file name;
  2. the output file name; (partial locaaton is specified like ../output/OutLowPass_) and rest of the location 
	is specied by code according to the image like finalImg.jpg, frquencyImg.jpg. So the final image path will be
	"../output/OutLowPass_finalImg.jpg"
  3. "opencv" parameter to specify that opencv is used to perform some operation 
  4. the name of the Operation. Use "lowpass", "highpass" as described above;
  5. Special parameters for that particular operation

eg. src\File\Loc dst\FIle\Loc oprationName 23 45

{23 45 12} -> Special Parameter


-Special Parameters
 1. lowpass/highpass -> a -> a= threshold distance for circle
 2. bandpass/notch -> a b -> a= threshold 1 distance for circle (smaller), b = threshold 2 distance for circle (bigger)
 3. combine -> a b c -> a = threshold 1 distance for circle (smaller), b = threshold 2 distance for circle (bigger), c = threshold distance for circle (lowpass)



*** Important information ***

Application assumes the next format of input image (ppm/pgm) file:
line1: <version>
line2: <#columns> <#rows>
line3: <max_value>
line4-end-of-file:<pix1><pix2>...<pix_n>

if it is a grayscale image then every pixel is a char value. If it is a RGB image then every pixel is a set of 3 char values: <red><green><blue>

Thus, if you have a problem with reading image, the first thing you should check is input file format.
