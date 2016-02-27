#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv )
{
    Mat image;
	VideoCapture inputVideo(0);              // Open input

	if (!inputVideo.isOpened())
	{
		std::cout  << "Could not open camera: " << std::endl;
		return -1;
	}

	for(;;) //Show the image captured in the window and repeat
	{
		inputVideo >> image;              // read
		if (image.empty()) break;         // check if at end

		imshow("Display Image", image);

		if(waitKey(5) >= 0) break;
	}

	return 0;
}
