#include "Combiner.h"
#include <iostream>

Eigen::MatrixXd Combiner::transformRed;
Eigen::MatrixXd Combiner::transformGreen;
Eigen::MatrixXd Combiner::transformBlue;

//constructor
Combiner::Combiner(PNGImage images[])
{
	//number of images in the array
	int numImages = sizeof(images) / sizeof(images[0]);

	//initialize size of transform matrices
	//Rows = number of pixels in each image
	//Cols = number of images lighting was precomputed for
	transformRed(images[0].width * images[0].height, numImages);
	transformGreen(images[0].width * images[0].height, numImages);
	transformBlue(images[0].width * images[0].height, numImages);

	//loop through each image, pull out relevant vectors
	//matrices set column-by-column
	for (int i = 0; i < numImages; i++)
	{
		transformRed.col(i) = images[i].redChannel;
		transformGreen.col(i) = images[i].greenChannel;
		transformBlue.col(i) = images[i].blueChannel;
	}
}

//multiply the transform matrix and the light vector to get resulting image
void Combiner::combine(Eigen::VectorXd lights, float* out)
{
	//make sure number of rows in transform matrix equals the number of lights
	if (lights.size() != transformRed.rows())
	{
		std::cerr << "Invalid length of light vector!" << std::endl;
		exit(-1);
	}

	//place multiplication results for each channel in temporary vectors
	Eigen::VectorXd redChannel(transformRed.size());
	Eigen::VectorXd greenChannel(transformGreen.size());
	Eigen::VectorXd blueChannel(transformBlue.size());

	redChannel = transformRed * lights;
	greenChannel = transformGreen * lights;
	blueChannel = transformBlue * lights;

	//loop through data, add each pixel to out array
	for (int i = 0, j = 0; i < 3 * redChannel.size(); i += 3, j++)
	{
		out[i] = redChannel(j);
		out[i + 1] = greenChannel(j);
		out[i + 2] = blueChannel(j);
	}
}