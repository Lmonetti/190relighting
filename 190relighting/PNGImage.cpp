#include "PNGImage.h"
#include <iostream>

//Constructor
PNGImage::PNGImage(unsigned char** img, int w, int h)
{
	//set width and height
	width = w;
	height = h;

	//local vars for each pixel
	double r, g, b;

	//initialize vectors storing color channel values for each pixel with correct length
	redChannel(width * height);
	greenChannel(width * height);
	blueChannel(width * height);

	//Read in data parsed by dirent, place separate color 
	//channels into their respective arrays.
	for (int i = 0; i < sizeof(*img) / sizeof(*img[0]); i++)
	{
		//convert unsigned char to double
		r = static_cast<double>(*img[i]);

		//first value goes into Red channel
		redChannel(i) = r;

		i++;

		g = static_cast<double>(*img[i]);

		//second value goes into Green channel
		greenChannel(i) = g;

		i++;

		b = static_cast<double>(*img[i]);

		//third value goes into Blue channel
		blueChannel(i) = b;

		//loop increment will place index at next pixel, so no need to increment i again
	}
}