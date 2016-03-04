#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>
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

void display(void);

void init() {
	width = default_width;
	height = default_height;

	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir("povray/sphere")) != NULL) {
		/* print all the files and directories within directory*/
		while ((ent = readdir(dir)) != NULL) {
			std::printf("%s\n", ent->d_name);
			unsigned char** out;
			width = default_width;
			height = default_height;
			unsigned error = lodepng_decode_file(out, &width, &height, ent->d_name, LCT_RGB, 24);

			if (error != NULL) {
				if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
			}
			std::cout << "size of array is " << sizeof(out);
		}
		closedir(dir);
	}
	else {
		/* cannot open directory*/
		std::perror("Directory_Error");
	}


	glutInitWindowSize(default_width, default_height);
	glutDisplayFunc(display);
}

void display() {}

int main(int argc, char* argv[]) {
	//parse_command_line(argc, argv);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(Window::width, Window::height);          //Set initial window size
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

	glutMainLoop();
	return 0;
}