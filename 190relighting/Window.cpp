#include <iostream>
#include <cstring>
#include <time.h>
#include <math.h>
#include <cassert>
#include <dirent.h>   //Read all files from directory
#include <Eigen/Dense> //Matrix and vector calculations

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#include <GL/glut.h>
#endif

#include "Window.h"
#include "lodepng.h"    //Load PNG files
#include "Combiner.h"

int Window::default_width = 256;
int Window::default_height = 256;
int Window::default_bitDepth = 24;
float* Window::pixels;
Eigen::VectorXd Window::lightWeights[2];

unsigned int Window::width;
unsigned int Window::height;
unsigned int Window::resolution;
unsigned int Window::cubemap_width = 16;

std::vector<PNGImage*>* Window::images;
Combiner* Window::combiner;
EnvironmentMap* Window::envMap;

void Window::initialize(void)
{
	//set window width and height
	width = default_width;
	height = default_height;

	images = new std::vector<PNGImage*>();

	DIR *dir;
	struct dirent *ent;
	char* directoryName = "povray/sphere_complete";

	if ((dir = opendir(directoryName)) != NULL) {
		/* print all the files and directories within directory*/
		while ((ent = readdir(dir)) != NULL) {
			//std::printf("\nReading in file name %s\n", ent->d_name);
			std::vector<unsigned char> out;
			width = default_width;
			height = default_height;
			std::string temp = std::string(directoryName) + "/" + std::string(ent->d_name);

			const char* filename = temp.c_str();

			//unsigned error = lodepng::decode(out, width, height, filename);

			/*if (error != NULL) {
				if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
			}
			else {
				//std::cerr << "Image width: " << width << std::endl;
				//std::cerr << "Image height: " << height << std::endl;
				//make image object, add to vector of images
				PNGImage* img = new PNGImage(&out, width, height);

				//img->print();

				images->push_back(img);
			}*/
		}
		closedir(dir);
	}
	else {
		/* cannot open directory*/
		std::perror("Directory_Error");
	}

	int full_res = cubemap_width * cubemap_width * 6;
	lightWeights[0] = Eigen::VectorXd(full_res);
	for (int i = 0; i < full_res; ++i) {
		lightWeights[0](i) = 1.0f / (float) full_res;
	}

	//lightWeights[0] = Eigen::VectorXd(20);
	//lightWeights[0] << 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05,
		//0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05;
	
	lightWeights[1] = Eigen::VectorXd(20);
	lightWeights[1] << 0, 0, 0, 0, .5, .5, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0;
	
	Window::pixels = new float[3 * (int)width * (int)height];

	//combiner = new Combiner(images);

	envMap = new EnvironmentMap("povray/PRTCubemap1", cubemap_width, 0);
	
	Eigen::VectorXd* red_light_final = new Eigen::VectorXd(envMap->red_light_vector->size());
	Eigen::VectorXd* green_light_final = new Eigen::VectorXd(envMap->green_light_vector->size());
	Eigen::VectorXd* blue_light_final = new Eigen::VectorXd(envMap->blue_light_vector->size());


	for (int i = 0; i < 100; ++i) {
		int index = envMap->red_light_vector->at(i)->first;
		int val = envMap->red_light_vector->at(i)->second;
		(*red_light_final) (index) = val;
		std::cout << "Red at " << index << " where value is " << val << std::endl;

		index = envMap->green_light_vector->at(i)->first;
		val = envMap->green_light_vector->at(i)->second;
		(*green_light_final) (index) = val;
		std::cout << "Green at " << index << " where value is " << val << std::endl;

		index = envMap->blue_light_vector->at(i)->first;
		val = envMap->blue_light_vector->at(i)->second;
		(*blue_light_final) (index) = val;
		std::cout << "Blue at " << index << " where value is " << val << std::endl;
	}

	//combiner->combine(lightWeights[0], pixels);
	combiner->combine(*red_light_final, *green_light_final, *blue_light_final, pixels);
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