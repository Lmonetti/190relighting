#ifndef PNG_IMAGE_h
#define PNG_IMAGE_h

#include <Eigen/Dense>

class PNGImage
{
public:
	int width, height;

	//vectors of color channel values for each pixel P_0 through P_n
	Eigen::VectorXd redChannel;
	Eigen::VectorXd greenChannel;
	Eigen::VectorXd blueChannel;

	PNGImage(unsigned char**, int, int);
};

#endif
