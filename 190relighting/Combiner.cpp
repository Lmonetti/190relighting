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
	int cubemap_width = transformRed->cols() / 6;
	/*for (int rowN = 0; rowN < res; rowN++) {
		for (int img = 0; img < transformRed->cols(); img += cubemap_width) {
			Eigen::VectorXd * img_red = new Eigen::VectorXd(cubemap_width);
			Eigen::VectorXd * img_green = new Eigen::VectorXd(cubemap_width);
			Eigen::VectorXd * img_blue = new Eigen::VectorXd(cubemap_width);

			std::cout << "waveleting row " << rowN << std::endl;
			haar2d(transformRed->row(i));
			haar2d(transformGreen->row(i));
			haar2d(transformBlue->row(i));
		}
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

/* Perform haar wavelet 1d transform on a single row */
void Combiner::haar1drow(Eigen::MatrixXd* mat, int row, int index, int width) {
	Eigen::MatrixXd temp(1, width);
	int half = floor((float) width / 2.0f);
	std::cout << "half is " << half << std::endl;

	for (int i = 0, j = 0; i < width; i += 2, ++j) {
		std::cout << "At " << i + index << ", trying to replace index " << j << " and " << j + half << std::endl;
		//fill first half with average of each pair
		temp(j) = ((*mat)(row, i + index) + (*mat)(row, i + 1 + index)) / sqrt(2.0);
		//fill second half with coefficients
		temp(j + half) = ((*mat)(row, i + index) - (*mat)(row, i + 1 + index)) / sqrt(2.0);
	}

	mat->block(row, index, 1, width) = temp.block(0, 0, 1, width);
}

/* Perform haar wavelet 1d transform on a single col */
void Combiner::haar1dcol(Eigen::MatrixXd* mat, int row, int index, int width, int cubemap_width) {
	Eigen::MatrixXd temp(1, width);
	int half = floor((float) width / 2.0f);

	for (int i = 0, j = 0; i < width; i += 2, ++j) {
		std::cout << "At " << i * cubemap_width + index << ", trying to replace index " << j << " and " << j + half << std::endl;
		temp(j) = ((*mat)(row, i * cubemap_width + index) + (*mat)(row, (i + 1) * cubemap_width + index)) / sqrt(2.0);
		temp(j + half) = ((*mat)(row, i * cubemap_width + index) - (*mat)(row, (i + 1) * cubemap_width + index)) / sqrt(2.0);
	}

	for (int i = 0; i < width; ++i) {
		//std::cout << "At " << i << ", replacing vector[" << (i + index) * cubemap_width << "] with " << col[i] << std::endl;
		(*mat)(row, i * cubemap_width + index) = temp(i);
	}
}

/* 2d haar wavelet trasnform
 * 'row' is the pixel we are on, face is the cubemap face we are on
 */
void Combiner::haar2d(Eigen::MatrixXd* mat, int row, int face, int cubemap_width) {
	/* initial width of filter */
	int filter_width = cubemap_width;

	/* wavelet transform */
	while (filter_width > 1) {
		int index = 0;
		/* loop through all rows */
		for (int i = 0; i < cubemap_width; ++i) {
			haar1drow(mat, row, index, filter_width);
			index += cubemap_width;
		}
		/* loop through all columns */
		for (int i = 0; i < cubemap_width; ++i) {
			haar1dcol(mat, i, row, filter_width, cubemap_width);
		}

		/* reduce filter width */
		filter_width /= 2;
	}
	
	std::cout << "Done with Haar..." << std::endl;
}

