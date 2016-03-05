#ifndef Combiner_h
#define Combiner_h

#include <Eigen/Dense>
#include "PNGImage.h"
#include <vector>

//class that takes a transform matrix, a light vector, and combines them together
class Combiner
{
public:
	Eigen::MatrixXd* transformRed;
	Eigen::MatrixXd* transformGreen;
	Eigen::MatrixXd* transformBlue;

	Combiner(std::vector<PNGImage*>*);

	//function that combines multiplies transform matrix and light intensities to output pixels
	void combine(Eigen::VectorXd, float*);
};

#endif
