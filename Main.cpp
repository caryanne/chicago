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
#include "glm\gtc\matrix_inverse.hpp"


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
	printf("%.2f:started. begin initializing systems\n", glfwGetTime());
	double start = glfwGetTime();

	window = glfwCreateWindow(1360, 768, "chicago", glfwGetPrimaryMonitor(), NULL);
	//window = glfwCreateWindow(800, 600, "chicago", NULL, NULL);
	if(!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	printf("%.2f:window created\n", glfwGetTime());

	glfwMakeContextCurrent(window);
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	printf("%.2f:OpenGL context created\n", glfwGetTime());
	if(glewInit() != GLEW_OK)
		exit(EXIT_FAILURE);
	printf("%.2f:checked OpenGL extensions\n", glfwGetTime());
	//load and set up shit

	Model base = Model("station.obj");
	Model plane = Model("plane.obj");

	Model helmetframe = Model("helmetframe.obj");
	Model helmetshield = Model("helmetshield.obj");
	
	plane.setScale(glm::vec3(5.0f));
	base.setScale(glm::vec3(0.25f));
	helmetframe.setScale(glm::vec3(0.9f));
	helmetshield.setScale(glm::vec3(0.9f));

	glClearColor(0.0f, 0.0f, 0.05f, 1.f);
	unsigned frames = 0;
	double lastUpdate = 0;
	
	printf("%.2f:end initializing systems in %.2fs\n", glfwGetTime(), glfwGetTime() - start);

	while(!glfwWindowShouldClose(window)) {

	
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		setup3d(width, height);

		
		

		glm::mat4 projection = glm::perspective( 45.f, width / (float)height, 0.1f, 100.f);

		glm::vec3 eye = glm::vec3(5.f * (float)sin(glfwGetTime()), 1.5f, 5.f * (float)cos(glfwGetTime()));
		//glm::vec3 eye = glm::vec3(-5.f, 3.f, -5.f);
		glm::mat4 view = glm::lookAt(eye,
										glm::vec3(1.f, 1.f, 1.5f),
										glm::vec3(0.5f, 1.f, -0.25f));

		glm::mat4 viewProjection = projection * view;

		helmetframe.setPosition(eye);
		helmetshield.setPosition(eye);

		helmetframe.setRotation(glm::toQuat(glm::inverse(view)));
		helmetshield.setRotation(glm::toQuat(glm::inverse(view)));

		
		base.render(eye, view, viewProjection);
		plane.render(eye, view, viewProjection);
		helmetframe.render(eye, view, viewProjection);
		helmetshield.render(eye, view, viewProjection);
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