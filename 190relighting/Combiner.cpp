#include "Combiner.h"
#include <iostream>

//constructor
Combiner::Combiner(std::vector<PNGImage*>* images)
{
	//number of images in the array
	int numImages = images->size();

	//initialize size of transform matrices
	//Rows = number of pixels in each image
	//Cols = number of images lighting was precomputed for
	transformRed = new Eigen::MatrixXd(images->at(0)->width * images->at(0)->height, numImages);
	transformGreen = new Eigen::MatrixXd(images->at(0)->width * images->at(0)->height, numImages);
	transformBlue = new Eigen::MatrixXd(images->at(0)->width * images->at(0)->height, numImages);

	//loop through each image, pull out relevant vectors
	//matrices set column-by-column
	for (int i = 0; i < numImages; i++)
	{
		transformRed->col(i) = *images->at(i)->redChannel;
		transformGreen->col(i) = *images->at(i)->greenChannel;
		transformBlue->col(i) = *images->at(i)->blueChannel;
	}
}

//multiply the transform matrix and the light vector to get resulting image
void Combiner::combine(Eigen::VectorXd lights, float* out)
{
	//make sure number of rows in transform matrix equals the number of lights
	if (lights.size() != transformRed->cols())
	{
		std::cerr << "Invalid length of light vector!" << std::endl;
		exit(-1);
	}

	//place multiplication results for each channel in temporary vectors
	Eigen::VectorXd redChannel(transformRed->size());
	Eigen::VectorXd greenChannel(transformGreen->size());
	Eigen::VectorXd blueChannel(transformBlue->size());

	redChannel = *transformRed * lights;
	greenChannel = *transformGreen * lights;
	blueChannel = *transformBlue * lights;

	//loop through data, add each pixel to out array
	for (int i = 3 * redChannel.size()-1, j = 0; i >= 0; i -= 3, j++)
	{
		out[i - 2] = redChannel(j);
		out[i - 1] = greenChannel(j);
		out[i] = blueChannel(j);
	}
}