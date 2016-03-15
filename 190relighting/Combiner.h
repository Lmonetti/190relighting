#ifndef Combiner_h
#define Combiner_h

#include <Eigen/Dense>
#include "PNGImage.h"
#include <vector>
#include <string>

//class that takes a transform matrix, a light vector, and combines them together
class Combiner
{
public:
	Eigen::MatrixXd* transformRed;
	Eigen::MatrixXd* transformGreen;
	Eigen::MatrixXd* transformBlue;

	Combiner(void);
	Combiner(std::vector<PNGImage*>*);

	//function that combines multiplies transform matrix and light intensities to output pixels
	void combine(Eigen::VectorXd, float*);

	void combine(Eigen::VectorXd, Eigen::VectorXd, Eigen::VectorXd, float*);

	//function for computing 1d haar wavelet for each row
	void haar1drow(Eigen::VectorXd, int, int);

	//function for computing 1d haar wavelet for each column
	void haar1dcol(Eigen::VectorXd, int, int, int);

	//function for computing 2d haar wavelet for each cube face
	void haar2d(Eigen::VectorXd);
};

#endif
