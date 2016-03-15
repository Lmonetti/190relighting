#include "EnvironmentMap.h"
#include "lodepng.h"    //Load PNG files
#include "PNGImage.h"   //Store cube map images
#include "Combiner.h"   //Haar wavelet transform

#include <dirent.h>   //Read all files from directory
#include <algorithm>  //Sort vector
#include <iostream>
#include <string>

EnvironmentMap::EnvironmentMap(char* directoryName, unsigned int cubemap_width, int sort_mode) {
	red_light_vector = new std::vector<std::pair<int, float>*> ();
	green_light_vector = new std::vector<std::pair<int, float>*> ();
	blue_light_vector = new std::vector<std::pair<int, float>*> ();
	SORT_MODE = sort_mode;
	wavelet_size = 100; /* default */
	
	buildEnvMap(directoryName, cubemap_width);
}

/* Sort by magnitube of wavelet coefficients of lights */
bool unweighted_sort(std::pair<int, float>* left, std::pair<int, float>* right)
{
	return left->second > right->second;
}

/*bool redTWS(std::pair<int, float>* left, std::pair<int, float>* right)
{
float leftComp = left->second * (*red_means)(left->first);
float rightComp = right->second * (*red_means)(right->first);
return leftComp > rightComp;
}

bool greenTWS(std::pair<int, float>* left, std::pair<int, float>* right)
{
float leftComp = left->second * (*green_means)(left->first);
float rightComp = right->second * (*green_means)(right->first);
return leftComp > rightComp;
}

bool blueTWS(std::pair<int, float>* left, std::pair<int, float>* right)
{
float leftComp = left->second * (*blue_means)(left->first);
float rightComp = right->second * (*blue_means)(right->first);
return leftComp > rightComp;
}*/

void EnvironmentMap::buildEnvMap(char* directoryName, unsigned int cubemap_width) {
	DIR *dir;             //dirent
	struct dirent *ent;
	this->cubemap_width = cubemap_width;

	if ((dir = opendir(directoryName)) != NULL) {
		/* print all the files and directories within directory*/
		while ((ent = readdir(dir)) != NULL) {
			std::printf("\nReading in file name %s\n", ent->d_name);
			std::vector<unsigned char> out;
			std::string temp = std::string(directoryName) + "/" + std::string(ent->d_name);
			std::cout << temp << std::endl;

			const char* filename = temp.c_str();

			unsigned error = lodepng::decode(out, cubemap_width, cubemap_width, filename);

			if (error != NULL) {
				if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
			}
			else {
				std::cerr << "Cubemap width: " << cubemap_width << std::endl;

				std::string filename_raw = std::string(ent->d_name);
				int curr_face;
				int face_res = (int)(cubemap_width * cubemap_width);

				if (filename_raw.compare("cubemap_top.png") == 0)
					curr_face = 0;
				else if (filename_raw.compare("cubemap_bottom.png") == 0)
					curr_face = face_res;
				else if (filename_raw.compare("cubemap_left.png") == 0)
					curr_face = face_res * 2;
				else if (filename_raw.compare("cubemap_right.png") == 0)
					curr_face = face_res * 3;
				else if (filename_raw.compare("cubemap_front.png") == 0)
					curr_face = face_res * 4;
				else if (filename_raw.compare("cubemap_back.png") == 0)
					curr_face = face_res * 5;

				//Build cube face
				buildEachCubeFace(out, curr_face);
			}
		}
		//light vector map
		closedir(dir);
	}
	else {
		/* cannot open directory*/
		std::perror("Directory_Error");
	}

	/*std::cout << "size of red light vector is " + red_light_vector->size() << '\n';
	for (int i = 0; i < red_light_vector->size(); ++i) {
		std::cout << "at " << i << ", value is " << red_light_vector->at(i)->second << std::endl;
	}*/

	/* Sort vectors according to non-linear approximation */
	switch(SORT_MODE) {
	case UNWEIGHTED_SELECT:
		std::cout << "Default to unweighted selection mode." << std::endl;
		std::sort(red_light_vector->begin(), red_light_vector->end(), unweighted_sort);
		std::sort(green_light_vector->begin(), green_light_vector->end(), unweighted_sort);
		std::sort(blue_light_vector->begin(), blue_light_vector->end(), unweighted_sort);
		break;
	case TRANSPORT_WEIGHTED:
		std::cout << "Switched to transport weighted mode." << std::endl;
		/*std::sort(red_light_vector->begin(), red_light_vector->end(), redTWS);
		std::sort(green_light_vector->begin(), green_light_vector->end(), greenTWS);
		std::sort(blue_light_vector->begin(), blue_light_vector->end(), blueTWS);*/
		break;
	case AREA_WEIGHTED:
		std::cout << "Area weighted mode have not been implemented." << std::endl;
		break;
	}
}

void EnvironmentMap::buildEachCubeFace(std::vector<unsigned char> out, int curr_face) {
	PNGImage* image = new PNGImage(&out, (int)cubemap_width, (int)cubemap_width);
	int full_res = (int)(cubemap_width * cubemap_width);
	Combiner* combiner = new Combiner();

	Eigen::MatrixXd red_holder(1, image->redChannel->size());
	red_holder.row(0) = *image->redChannel;
	Eigen::MatrixXd green_holder(1, image->greenChannel->size());
	green_holder.row(0) = *image->greenChannel;	
	Eigen::MatrixXd blue_holder(1, image->blueChannel->size());
	blue_holder.row(0) = *image->blueChannel;

	std::cout << "Red haar transform." << std::endl;
	combiner->haar2d(&red_holder, 0, 0, cubemap_width);
	std::cout << "Green haar transform." << std::endl;
	combiner->haar2d(&green_holder, 0, 0, cubemap_width);
	std::cout << "Blue haar transform." << std::endl;
	combiner->haar2d(&blue_holder, 0, 0, cubemap_width);

	for (int i = 0; i < full_res; ++i) {
		std::pair<int, float> * red_pair = new std::pair<int, float>(i + curr_face, red_holder(0, i));
		std::pair<int, float> * green_pair = new std::pair<int, float>(i + curr_face, green_holder(0, i));
		std::pair<int, float> * blue_pair = new std::pair<int, float>(i + curr_face, blue_holder(0, i));

		red_light_vector->push_back(red_pair);
		green_light_vector->push_back(green_pair);
		blue_light_vector->push_back(blue_pair);
	}
}