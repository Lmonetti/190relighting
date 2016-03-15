#ifndef HaarTester_h
#define HaarTester_h

#include "Combiner.h"
#include <Eigen/Dense>
#include <iostream>

class HaarTester
{
private:
	Combiner* combiner;

public:
	HaarTester();
	void HaarTest();


};

#endif