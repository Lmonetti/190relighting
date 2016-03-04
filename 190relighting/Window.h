#ifndef Window_h
#define Window_h

class Window		//OpenGL output window-related routines
{
public:
	static float width, height;

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
