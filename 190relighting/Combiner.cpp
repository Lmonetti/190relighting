#include "Combiner.h"
#include <iostream>

//constructor
Combiner::Combiner() {};

Combiner::Combiner(std::vector<PNGImage*>* images)
{
	//number of images in the array
	int numImages = images->size();

	//initialize size of transform matrices 
	//Rows = number of pixels in each image
	//Cols = number of images lighting was precomputed for
	int res = images->at(0)->width * images->at(0)->height;
	transformRed = new Eigen::MatrixXd(res, numImages);
	transformGreen = new Eigen::MatrixXd(res, numImages);
	transformBlue = new Eigen::MatrixXd(res, numImages);

	//loop through each image, pull out relevant vectors
	//matrices set column-by-column
	for (int i = 0; i < numImages; i++)
	{
		transformRed->col(i) = *images->at(i)->redChannel;
		transformGreen->col(i) = *images->at(i)->greenChannel;
		transformBlue->col(i) = *images->at(i)->blueChannel;
	}

	/* wavelet transform each row of the matrix */
	/*for (int i = 0; i < res; i++) {
		std::cout << "waveleting row " << i << std::endl;
		haar2d(transformRed->row(i));
		haar2d(transformGreen->row(i));
		haar2d(transformBlue->row(i));
	}*/
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
	for (int i = 3 * redChannel.size() - 1, j = 0; i >= 0; i -= 3, j++)
	{
		out[i - 2] = redChannel(j);
		out[i - 1] = greenChannel(j);
		out[i] = blueChannel(j);
	}
}

//multiply the transform matrix and the light vector to get resulting image
void Combiner::combine(Eigen::VectorXd red_light, Eigen::VectorXd green_light, Eigen::VectorXd blue_light, float* out)
{
	//make sure number of rows in transform matrix equals the number of lights
	if (red_light.size() != transformRed->cols())
	{
		std::cerr << "Invalid length of light vector!" << std::endl;
		exit(-1);
	}

	//place multiplication results for each channel in temporary vectors
	Eigen::VectorXd redChannel(transformRed->size());
	Eigen::VectorXd greenChannel(transformGreen->size());
	Eigen::VectorXd blueChannel(transformBlue->size());

	redChannel = *transformRed * red_light;
	greenChannel = *transformGreen * green_light;
	blueChannel = *transformBlue * blue_light;

	//loop through data, add each pixel to out array
	for (int i = 3 * redChannel.size() - 1, j = 0; i >= 0; i -= 3, j++)
	{
		out[i - 2] = redChannel(j);
		out[i - 1] = greenChannel(j);
		out[i] = blueChannel(j);
	}
}

/* Multiply transport matrix and light vector according to number of wavelets*/
/*void Combiner::combine(std::vector<std::pair<int, float>*>* light_vector, int numWavelets, float* out)
{
	//make sure number of rows in transform matrix equals the number of lights
	if (light_vector->size() != transformRed->cols())
	{
		std::cerr << "Invalid length of light vector!" << std::endl;
		exit(-1);
	}
	else if (numWavelets > light_vector->size()) {
		std::cerr << "Number of wavelets exceed total amount of lights!" << std::endl;
		exit(-1);
	}

	//place multiplication results for each channel in temporary vectors
	Eigen::VectorXd redChannel(transformRed->size());
	Eigen::VectorXd greenChannel(transformGreen->size());
	Eigen::VectorXd blueChannel(transformBlue->size());

	for (int i = 0; i < numWavelets; ++i) {
		int index = light_vector->at(i)->first;
		transformRed->row(index) * light_vector
	}
	redChannel = *transformRed * lights;
	greenChannel = *transformGreen * lights;
	blueChannel = *transformBlue * lights;

	//loop through data, add each pixel to out array
	for (int i = 3 * redChannel.size() - 1, j = 0; i >= 0; i -= 3, j++)
	{
		out[i - 2] = redChannel(j);
		out[i - 1] = greenChannel(j);
		out[i] = blueChannel(j);
	}
}*/


/* Perform haar wavelet 1d transform on a single row */
void Combiner::haar1drow(Eigen::VectorXd vec, int index, int width) {
	Eigen::VectorXd * row = new Eigen::VectorXd(width);
	int half = floor((float) width / 2.0f);
	//std::cout << "half is " << half << std::endl;

	for (int i = 0; i < half; i += 2) {
		//std::cout << "At " << i << ", trying to replace index " << i + index << std::endl;
		//fill first half with average of each pair
		(*row)(i) = (vec(i + index) + vec(i + 1 + index)) / sqrt(2.0);
		//fill second half with coefficients
		(*row)(i + half) = (vec(i + index) - vec(i + 1 + index)) / sqrt(2.0);
	}

	for (int i = 0; i < width; ++i) {
		vec(i + index) = (*row)(i);
	}
}

/* Perform haar wavelet 1d transform on a single col */
void Combiner::haar1dcol(Eigen::VectorXd vec, int index, int width, int cubemap_width) {
	Eigen::VectorXd *col = new Eigen::VectorXd(width);
	int half = floor((float) width / 2.0f);

	for (int i = 0; i < half; i += 2) {
		(*col)(i) = (vec(i * cubemap_width + index) + vec((i + 1) * cubemap_width + index)) / sqrt(2.0);
		(*col)(i + half) = (vec[i * cubemap_width + index] - vec((i + 1) * cubemap_width + index)) / sqrt(2.0);
	}

	for (int i = 0; i < width; ++i) {
		//std::cout << "At " << i << ", replacing vector[" << (i + index) * cubemap_width << "] with " << col[i] << std::endl;
		vec(i * cubemap_width + index) = (*col)(i);
	}
}

/* 2d haar wavelet trasnform */
void Combiner::haar2d(Eigen::VectorXd vec) {
	/* size of cube map */
	int cubemap_width = floor(sqrt((float) vec.size()));
	/* initial width of filter */
	int filter_width = cubemap_width;

	/* wavelet transform */
	while (filter_width > 1) {
		int index = 0;
		/* loop through all rows */
		for (int i = 0; i < cubemap_width; i += 1) {
			haar1drow(vec, index, filter_width);
			index += cubemap_width;
		}
		/* loop through all columns */
		for (int i = 0; i < cubemap_width; ++i) {
			haar1dcol(vec, i, filter_width, cubemap_width);
		}

		/* reduce filter width */
		filter_width /= 2;
	}
	
	std::cout << "Done with Haar..." << std::endl;
}

