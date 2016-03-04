#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include "lodepng.h"    //Load PNG files

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
	//glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutCreateWindow("Viewer");
	init();
	display();
	//glutDisplayFunc(display);
	//glutKeyboardFunc(keyboard);
	//glutSpecialFunc(specialKey);
	//glutReshapeFunc(reshape);
	//glutReshapeWindow(width, height);
	glutMainLoop();
	return 0;
}