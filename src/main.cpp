#include "common.hpp"

#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glutSwapBuffers();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(0);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL");
		
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		cerr << "error: " << glewGetErrorString(err) << endl;
	}

	glutDisplayFunc(display);
	glutIdleFunc(display);

	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glutMainLoop();
}