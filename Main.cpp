#include <windows.h>
#include "glfw/glfw3.h"
#include <gl/GLU.h>
#include "bullet/btBulletDynamicsCommon.h"
#include "soil/SOIL.h"

GLFWwindow *window;

void setup3d(double w, double h) {

	glClear(GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.f, w / h, 0.1f, 100.f);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	float pos[] = {20,3,20,0};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);*/

}

int main() {

	if(!glfwInit())
		exit(EXIT_FAILURE);


	window = glfwCreateWindow(800, 600, "chicago", NULL, NULL);
	if(!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetWindowPos(window, 50, 50);
	glfwMakeContextCurrent(window);
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	

	glClearColor(0.f, 0.5f, 1.f, 1.f);
	
	while(!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		setup3d(width, height);

		gluLookAt(0.f, 0.0f, 3.f,
					0.f, 0.0f, 0.f,
					0.f, 1.f, 0.f);

		glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f); glVertex3f(-1.f, 0.f, 0.f);
		glColor3f(0.f, 1.f, 0.f); glVertex3f(0.f, 1.f, 0.f);
		glColor3f(0.f, 0.f, 1.f); glVertex3f(1.f, 0.f, 0.f);
		glEnd();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	exit(EXIT_SUCCESS);
}