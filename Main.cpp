#include <gl\glew.h>
#include <windows.h>
#include "glfw\glfw3.h"
#include <gl\GLU.h>
#include "bullet\btBulletDynamicsCommon.h"

#include "soil\SOIL.h"
#include "Shader.h"
#include "Model.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"


GLFWwindow *window;

void setup3d(double w, double h) {

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, w, h);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(45.f, w / h, 0.1f, 100.f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	/*glEnable(GL_LIGHTING);
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

	Shader basic = Shader("basic.vert", "basic.frag");
	Shader simple = Shader("simple.vert", "simple.frag");
	Model monkey = Model("monkey.obj", &basic);
	//Model plane = Model("cloth.obj", &simple);

	
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	unsigned frames = 0;
	double lastUpdate = 0;
	
	while(!glfwWindowShouldClose(window)) {

	
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		setup3d(width, height);

		monkey.setPosition(glm::vec3(0.f,abs(0.5*sin(glfwGetTime()*3)),0.f));
		//monkey.setScale(glm::vec3(0.5+abs(0.5*tan(glfwGetTime()*.4))));
		

		glm::mat4 projection = glm::perspective( 45.f, width / (float)height, 0.1f, 100.f);

		//glm::vec3 eye = glm::vec3(6.5f * (float)sin(glfwGetTime()), 5.f, 6.5f * (float)cos(glfwGetTime()));
		glm::vec3 eye = glm::vec3(-5.f, 3.f, -5.f);
		glm::mat4 view = glm::lookAt(eye,
										glm::vec3(2.5*1.5f, 0.f, 2.5*1.5f),
										glm::vec3(0.f, 1.f, 0.f));

		glm::mat4 viewProjection = projection * view;

		for(int x = 0; x < 3; x++)
			for(int y = 0; y < 3; y++) {
				monkey.setPosition(glm::vec3(x * 2.5,(1*sin((glfwGetTime() + y / 5.f +  x / 3.f )*3)), y * 2.5));
				monkey.setRotation(glm::quat(glm::vec3(0,glm::radians(180+glfwGetTime()*10 + x*y*15),0.0)));
				monkey.render(eye, view, viewProjection);
			}

		monkey.render(eye, view, viewProjection);
		//plane.render(eye, view, viewProjection);

		glfwSwapBuffers(window);

		frames++;
		if(glfwGetTime() - lastUpdate > 1.0) {
			char buffer[32];
			sprintf(buffer, "%ifps", frames);
			lastUpdate = glfwGetTime();
			glfwSetWindowTitle(window,buffer);
			frames = 0;
		}

		glfwPollEvents();
	}

	glfwTerminate();
	exit(EXIT_SUCCESS);
}