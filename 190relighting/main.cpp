#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include "lodepng.h"    //Load PNG files
#include "Window.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//parameters
unsigned int width;
unsigned int height;
unsigned int resolution;
int default_width = 512;
int default_height = 384;
int default_bitDepth = 24;

void init() {
	width = default_width;
	height = default_height;

}
void display() {}

int main(int argc, char* argv[]) {
	//parse_command_line(argc, argv);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(width, height);          //Set initial window size
	glutCreateWindow("Precomputed Radiance Transfer");			//Open window and set window title

	glutDisplayFunc(Window::displayCallback);
	glutKeyboardFunc(Window::keyboardCallback);
	glutReshapeFunc(Window::reshapeCallback);

	//create menu
	glutCreateMenu(Window::menuItemCallback);

	//add entry for collapsing edge. Pass 0 to callback function
	glutAddMenuEntry("Spherical Harmonics", 0);
	glutAddMenuEntry("Wavelets", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	Window::initialize();

	glutMainLoop();
	return 0;
}