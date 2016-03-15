#ifndef ENVIRONMENT_MAP_H
#define ENVIRONMENT_MAP_H

#include <Eigen/Dense>
#include <vector>

class EnvironmentMap
{
private:
	enum MODE {UNWEIGHTED_SELECT = 0, TRANSPORT_WEIGHTED = 1, AREA_WEIGHTED = 2};
	int cubemap_width;
	int SORT_MODE;

public:
	/* wavelet size */
	int wavelet_size;
	/* environment face vectors consisted of pixel values */
	Eigen::VectorXd* red_env;
	Eigen::VectorXd* green_env;
	Eigen::VectorXd* blue_env;

	/* means of pixel values of each light in light transport matrix,
	   used for calculating transport weighted selection */
	Eigen::VectorXd* red_means;
	Eigen::VectorXd* green_means;
	Eigen::VectorXd* blue_means;

	/* light vectors L */
	std::vector<std::pair<int, float>*>* red_light_vector;
	std::vector<std::pair<int, float>*>* green_light_vector;
	std::vector<std::pair<int, float>*>* blue_light_vector;

	EnvironmentMap(char*, unsigned int, int);
	void buildEnvMap(char*, unsigned int);
	void buildEachCubeFace(std::vector<unsigned char>, int);

	/* Helper functions to sort the light vector */
	/* unweighted selection */
	//bool unweighted_sort(std::pair<int, float>, std::pair<int, float>);
	/* transport weighted selection */
	/*bool redTWS(std::pair<int, float>*, std::pair<int, float>*);
	bool greenTWS(std::pair<int, float>*, std::pair<int, float>*);
	bool blueTWS(std::pair<int, float>*, std::pair<int, float>*);*/
};

#endif