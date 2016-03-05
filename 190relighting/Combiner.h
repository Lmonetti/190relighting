#ifndef Combiner_h
#define Combiner_h

#include <Eigen/Dense>
#include "PNGImage.h"

//class that takes a transform matrix, a light vector, and combines them together
class Combiner
{
public:
	static Eigen::MatrixXd transformRed;
	static Eigen::MatrixXd transformGreen;
	static Eigen::MatrixXd transformBlue;

	Combiner(PNGImage[]);

	//function that combines multiplies transform matrix and light intensities to output pixels
	static void combine(Eigen::VectorXd, float*);
};

#endif
