#include <gl/glew.h>
#include <windows.h>
#include "glfw/glfw3.h"
#include <gl/GLU.h>
#include "bullet/btBulletDynamicsCommon.h"
#include "soil/SOIL.h"
#include "Shader.h"
#include "Model.h"


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

	glfwMakeContextCurrent(window);
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	
	if(glewInit() != GLEW_OK)
		exit(EXIT_FAILURE);

	//load and set up shit

	Shader test = Shader("vertex.glsl", "frag.glsl");
	Model model = Model("pib2.obj", &test);

	glClearColor(0.f, 0.5f, 1.f, 1.f);
	
	while(!glfwWindowShouldClose(window)) {

	
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		setup3d(width, height);

		gluLookAt(0.f, 0.0f, 10.f,
					0.f, 0.0f, 0.f,
					0.f, 1.f, 0.f);

		model.render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	exit(EXIT_SUCCESS);
}