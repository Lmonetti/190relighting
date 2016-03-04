#ifndef PNG_IMAGE
#define PNG_IMAGE

#include <Eigen/Dense>

class PNGImage
{
public:
	int width, height;

	//vectors of color channel values for each pixel P_0 through P_n
	Eigen::VectorXd transformRed;
	Eigen::VectorXd transformGreen;
	Eigen::VectorXd transformBlue;

	PNGImage(unsigned char**, int, int);
};

#endif
