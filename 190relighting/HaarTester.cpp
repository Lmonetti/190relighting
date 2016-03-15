#include "HaarTester.h"

HaarTester::HaarTester()
{
	combiner = new Combiner();
}

void HaarTester::HaarTest()
{
	Eigen::MatrixXd test1(1, 4);
	test1 << 1, 3, 5, 7;

	combiner->Haar1D(&test1, 0);

	std::cerr << "Results of 1D Haar simplification with vector of " <<
		"length that is a power of 2. Should see 8 " <<
		"-4 -1.4142 -1.4142 " << std::endl << std::endl;
	std::cerr << test1 << std::endl;

	combiner->Haar1DInverse(&test1, 0);

	std::cerr << "Results of 1D Haar expansion with vector of " <<
		"length that is a power of 2. Should see 1 3 5 7" << std::endl << std::endl;
	std::cerr << test1 << std::endl;

	//Now with vectors that aren't a power of 2
	Eigen::MatrixXd test2(1, 5);
	test2 << 1, 3, 5, 7, 9;

	combiner->Haar1D(&test2, 0);

	std::cerr << "Results of 1D Haar simplification with vector of " <<
		"length that is NOT a power of 2. Should see 8 " <<
		"-4 -1.4142 -1.4142 9" << std::endl << std::endl;

	std::cerr << test2 << std::endl;

	combiner->Haar1DInverse(&test2, 0);

	std::cerr << "Results of 1D Haar expansion with vector of " <<
		"length that is NOT a power of 2. Should see 1 3 5 7 9" << std::endl << std::endl;
	std::cerr << test2 << std::endl;

	Eigen::MatrixXd test3(2, 4);
	test3.row(0) << test1;
	test3.row(1) << 2, 4, 6, 8;

	combiner->Haar2D(&test3);

	std::cerr << "Results of 2D Haar expansion with matrix with rows of length " <<
		"power of 2. Should see" << std::endl << "8 -4 -1.4142 -1.4142" <<
		std::endl << "10 -4 -1.4142 -1.4142" << std::endl << std::endl;
	std::cerr << test3 << std::endl;

	combiner->Haar2DInverse(&test3);

	std::cerr << "Results of expansion with matrix with rows of length " <<
		"power of 2. Should see" << std::endl << "1 3 5 7" << std::endl <<
		"2 4 6 8" << std::endl << std::endl;
	std::cerr << test3 << std::endl;

	Eigen::MatrixXd test4(2, 5);
	test4.row(0) << test2;
	test4.row(1) << 2, 4, 6, 8, 10;

	combiner->Haar2D(&test4);

	std::cerr << "Results of 2D Haar expansion with matrix with rows of length " <<
		"NOT a power of 2. Should see" << std::endl << "8 -4 -1.4142 -1.4142 9" <<
		std::endl << "10 -4 -1.4142 -1.4142 10" << std::endl << std::endl;
	std::cerr << test4 << std::endl;

	combiner->Haar2DInverse(&test4);

	std::cerr << "Results of expansion with matrix with rows of length " <<
		"NOT a power of 2. Should see" << std::endl << "1 3 5 7 9" << std::endl <<
		"2 4 6 8 10" << std::endl << std::endl;
	std::cerr << test4 << std::endl;
}