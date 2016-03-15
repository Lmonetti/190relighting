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

	//std::cerr << "Number of pixels: " << redChannel->size() << std::endl;
	//std::cerr << "Number of unsigned chars in incoming data: " << img->size() << std::endl;

	//Read in data parsed by dirent, place separate color 
	//channels into their respective arrays.
	for (unsigned int i = 0, j = 0; j < img->size(); ++i, j += 4)
	{
		//convert unsigned char to double
		//first value goes into Red channel
		r = static_cast<double>(img->at(j)) / 255.0f;
		(*redChannel)(i) = r;

		//second value goes into Green channel
		g = static_cast<double>(img->at(j + 1)) / 255.0f;
		(*greenChannel)(i) = g;

		//third value goes into Blue channel
		b = static_cast<double>(img->at(j + 2)) / 255.0f;
		(*blueChannel)(i) = b;
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