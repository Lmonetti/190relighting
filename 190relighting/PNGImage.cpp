#include "PNGImage.h"
#include <iostream>

//Constructor
PNGImage::PNGImage(std::vector<unsigned char>* img, int w, int h)
{
	//set width and height
	width = w;
	height = h;

	//local vars for each pixel
	double r, g, b;

	//initialize vectors storing color channel values for each pixel with correct length
	redChannel = new Eigen::VectorXd(width * height);
	greenChannel = new Eigen::VectorXd(width * height);
	blueChannel = new Eigen::VectorXd(width * height);

	std::cerr << "Number of pixels: " << redChannel->size() << std::endl;
	std::cerr << "Number of unsigned chars in incoming data: " << img->size() << std::endl;

	//Read in data parsed by dirent, place separate color 
	//channels into their respective arrays.
	for (int i = 0, j = 0; i < redChannel->size(); ++i, j += 3)
	{
		//convert unsigned char to double
		r = static_cast<double>(img->at(j));

		//first value goes into Red channel
		(*redChannel)(i) = r;

		//std::cerr << "Green pixel: " << static_cast<double>(img->at(i)) << std::endl;

		g = static_cast<double>(img->at(j + 1));

		//second value goes into Green channel
		(*greenChannel)(i) = g;

		b = static_cast<double>(img->at(j + 2));

		//third value goes into Blue channel
		(*blueChannel)(i) = b;

		//loop increment will place index at next pixel, so no need to increment i again

		//std::cerr << "channel index: " << i << std::endl;
		//std::cerr << "image data index: " << j << std::endl;
	}
}

void PNGImage::print()
{
	std::cerr << "RED CHANNEL: " << std::endl;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			std::cerr << (*redChannel)(i * width + j);
		}


		std::cerr << std::endl;
	}
}