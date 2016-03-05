#ifndef Window_h
#define Window_h

#include <Eigen/Dense>

class Window		//OpenGL output window-related routines
{
public:
	static float width, height;
	static float* pixels;					//data to draw
	static Eigen::VectorXd* lightWeights;	//different lighting weights to change lighting scheme

	static void initialize(void);
	static void idleCallback(void);
	static void reshapeCallback(int, int);
	static void displayCallback(void);
	static void keyboardCallback(unsigned char key, int x, int y);
	static void mouseCallback(int button, int state, int x, int y);
	static void motionCallback(int x, int y);
	static void menuItemCallback(int);
};

#endif
