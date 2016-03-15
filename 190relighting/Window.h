#ifndef Window_h
#define Window_h

#include <Eigen/Dense>
#include <vector>
#include "PNGImage.h"
#include "Combiner.h"
#include "EnvironmentMap.h"

class Window		//OpenGL output window-related routines
{
public:
	//parameters
	static unsigned int width;
	static unsigned int height;
	static unsigned int resolution;
	static unsigned int cubemap_width;

	static int default_width;
	static int default_height;
	static int default_bitDepth;
	static int num_wavelets;
	static float* pixels;					//data to draw
	static Eigen::VectorXd lightWeights[2];	//different lighting weights to change lighting scheme
	static std::vector<PNGImage*>* images;
	static Combiner* combiner;
	static EnvironmentMap* envMap;

	static void initialize(void);
	static void populateLightVectors(void);
	static void idleCallback(void);
	static void reshapeCallback(int, int);
	static void displayCallback(void);
	static void keyboardCallback(unsigned char key, int x, int y);
	static void specialKeyCallback(int key, int x, int y);
	static void mouseCallback(int button, int state, int x, int y);
	static void motionCallback(int x, int y);
	static void menuItemCallback(int);
};

#endif
