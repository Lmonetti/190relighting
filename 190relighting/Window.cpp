#include <iostream>
#include <cstring>
#include <time.h>
#include <math.h>
#include <cassert>
#include <dirent.h>   //Read all files from directory

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Window.h"
#include "lodepng.h"    //Load PNG files
#include <cstring>
#include <time.h>
#include <math.h>
#include <cassert>
#include "Combiner.h"
#include <Eigen/Dense>

int Window::default_width = 512;
int Window::default_height = 384;
int Window::default_bitDepth = 24;
float* Window::pixels;
Eigen::VectorXd Window::lightWeights[2];

unsigned int Window::width;
unsigned int Window::height;
unsigned int Window::resolution;

std::vector<PNGImage*>* Window::images;
Combiner* Window::combiner;

void Window::initialize(void)
{
	//Setup the light
	//GLfloat light_position[] = { 1.0, 40.0, 200.0, 0.0 };
	//gluLookAt(15, 0, 15, 0, 0, 0, 0, 1, 0);
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	//set window width and height
	width = default_width;
	height = default_height;

	images = new std::vector<PNGImage*>();

	DIR *dir;
	struct dirent *ent;
	char* directoryName = "povray/sphere";
	char* directoryNameWithSlash = "povray/sphere/";

	if ((dir = opendir(directoryName)) != NULL) {
		/* print all the files and directories within directory*/
		while ((ent = readdir(dir)) != NULL) {
			std::printf("\nReading in file name %s\n", ent->d_name);
			std::vector<unsigned char>* out = new std::vector<unsigned char>();
			width = default_width;
			height = default_height;
			std::string temp = std::string(directoryNameWithSlash) + std::string(ent->d_name);

			const char* filename = temp.c_str();

			unsigned error = lodepng::decode(*out, width, height, filename);
			std::cerr << "New image" << std::endl;
			for (int i = 0; i < out->size(); i++)
			{
				std::cerr << "channel val: " << static_cast<unsigned>(out->at(i)) << std::endl;
			}

			if (error != NULL) {
				if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
			}
			else {
				std::cerr << "Image width: " << width << std::endl;
				std::cerr << "Image height: " << height << std::endl;
				//make image object, add to vector of images
				PNGImage* img = new PNGImage(out, width, height);

				//img->print();

				images->push_back(img);
			}
		}
		closedir(dir);
	}
	else {
		/* cannot open directory*/
		std::perror("Directory_Error");
	}

	lightWeights[0] = Eigen::VectorXd(20);
	lightWeights[0] << 0, 0, 0, 0, 0, 0, 0, 0, .25, 0, .25,
		0, .25, 0, .25, 0, 0, 0, 0, 0;
	
	lightWeights[1] = Eigen::VectorXd(20);
	lightWeights[1] << 0, 0, 0, 0, .5, .5, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0;
	
	Window::pixels = new float[3 * (int)width * (int)height];

	combiner = new Combiner(images);

	combiner->combine(lightWeights[0], pixels);
}

//----------------------------------------------------------------------------
// Callback method called when system is idle.
// This is called at the start of every new "frame" (qualitatively)
void Window::idleCallback()
{
	//Set up a static time delta for update calls
	//Globals::camera.update();

	//std::cout << "FPS: " << fps << '\n';

	displayCallback();
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when graphics window is resized by the user
void Window::reshapeCallback(int w, int h)
{
	width = w;                                                       //Set the window width
	height = h;                                                      //Set the window height
	glViewport(0, 0, w, h);                                          //Set new viewport size
	glMatrixMode(GL_PROJECTION);                                     //Set the OpenGL matrix mode to Projection
	glLoadIdentity();                                                //Clear the projection matrix by loading the identity
	gluPerspective(60.0, double(width) / (double)height, 1.0, 1000.0); //Set perspective projection viewing frustum
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when window readraw is necessary or when glutPostRedisplay() was called.
void Window::displayCallback()
{
	std::cerr << "Displaying!" << std::endl;

	//Clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set the OpenGL matrix mode to ModelView
	glMatrixMode(GL_MODELVIEW);

	//DRAW IMAGE HERE
	glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixels);

	//Pop off the changes we made to the matrix stack this frame
	glPopMatrix();

	//Tell OpenGL to clear any outstanding commands in its command buffer
	//This will make sure that all of our commands are fully executed before
	//we swap buffers and show the user the freshly drawn frame
	glFlush();

	//Swap the off-screen buffer (the one we just drew to) with the on-screen buffer
	glutSwapBuffers();
}

void Window::keyboardCallback(unsigned char key, int x, int y) 
{
	switch (key)
	{
	case 'l':
		combiner->combine(lightWeights[0], pixels);
		break;
	case 'k':
		combiner->combine(lightWeights[1], pixels);
		break;
	}
}

//TODO: Mouse callbacks!
void Window::mouseCallback(int button, int state, int x, int y) 
{

}

//TODO: Mouse Motion callbacks!
void Window::motionCallback(int x, int y) 
{

}

//Menu callback!
//Potentially use to switch between wavelets and spherical harmonics for compression
void Window::menuItemCallback(int id)
{
	switch (id)
	{
	case 0:
		std::cerr << "Compression with Spherical Harmonics!" << std::endl;
		break;
	case 1:
		std::cerr << "Compression with wavelets!" << std::endl;
		break;
	}
}