#ifndef Window_h
#define Window_h

#define _SCL_SECURE_NO_WARNINGS

class Window		//OpenGL output window-related routines
{
public:
	//parameters
	static unsigned int width;
	static unsigned int height;
	static unsigned int resolution;
	static int default_width;
	static int default_height;
	static int default_bitDepth;

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
