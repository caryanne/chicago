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

#include "SceneManager.h"
#include "SceneNode.h"
#include "Camera.h"
#include "Mesh.h"

#include "Entity.h"

GLFWwindow *window;

SceneManager mgr;


void setup3d(double w, double h) {

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, w, h);
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

void reloadResources() {
	
	mgr.reloadScene();

}

static void keyPress(GLFWwindow* window, int key, int scanCode, int action, int mods) {
	if(key == GLFW_KEY_F1 && action == GLFW_PRESS)
		reloadResources();
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main() {

	
	if(!glfwInit())
		exit(EXIT_FAILURE);
	printf("%.2f:started. begin initializing systems\n", glfwGetTime());
	double start = glfwGetTime();

	window = glfwCreateWindow(1728, 972, "chicago", NULL, NULL);
	//window = glfwCreateWindow(1024, 768, "chicago", NULL, NULL);
	if(!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	printf("%.2f:window created\n", glfwGetTime());

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyPress);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	printf("%.2f:OpenGL context created\n", glfwGetTime());
	if(glewInit() != GLEW_OK)
		exit(EXIT_FAILURE);
	printf("%.2f:checked OpenGL extensions\n", glfwGetTime());
	
	//load and set up shit
	

	//Model helmetframe = Model("helmetframe.obj");
	//Model helmetshield = Model("helmetshield.obj");
	

	//helmetframe.setScale(glm::vec3(0.9f));
	//helmetshield.setScale(glm::vec3(0.9f));

	glClearColor(0.0f, 0.0f, 0.05f, 1.f);
	unsigned frames = 0;
	double lastUpdate = 0;
	
	printf("%.2f:end initializing systems in %.2fs\n", glfwGetTime(), glfwGetTime() - start);

	Mesh basemesh = Mesh("station.obj");
	Entity baseent = Entity(&basemesh);
	SceneNode base = SceneNode(&baseent);
	base.setScale(glm::vec3(0.25f));
	mgr.getRootNode()->addChild(&base);

	Mesh gndmesh = Mesh("plane.obj");
	Entity gndent = Entity(&gndmesh);
	SceneNode ground = SceneNode(&gndent);
	ground.setScale(glm::vec3(5.0f));
	mgr.getRootNode()->addChild(&ground);

	Mesh helmetmesh = Mesh("helmetframe.obj");
	Entity helmetent = Entity(&helmetmesh);
	SceneNode helmet = SceneNode(&helmetent);
	helmet.setScale(glm::vec3(3.f));
	helmet.setPosition(glm::vec3(3,3,3));
	base.addChild(&helmet);

/*	Mesh basemesh = Mesh("station.obj");
	Entity stnent = Entity(&basemesh);
	SceneNode base = SceneNode(&stnent);
	base.setScale(glm::vec3(0.25f));
	mgr.getRootNode()->addChild(&base);*/


	mgr.setScreenRatio(width / (float) height);
	

	while(!glfwWindowShouldClose(window)) {

	
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		setup3d(width, height);

		
		

		glm::vec3 eye = glm::vec3(10.f * (float)sin(glfwGetTime()), 5.f, 10.f * (float)cos(glfwGetTime()));
		//glm::vec3 eye = glm::vec3(-5.f, 3.f, -5.f);

		//helmetframe.setPosition(eye);
		//helmetshield.setPosition(eye);

		//helmetframe.setRotation(glm::toQuat(glm::inverse(view)));
		//helmetshield.setRotation(glm::toQuat(glm::inverse(view)));

		
		//base.render(eye, view, viewProjection);
		//plane.render(eye, view, viewProjection);
		//helmetframe.render(eye, view, viewProjection);
		//helmetshield.render(eye, view, viewProjection);
		base.setRotation(glm::vec3(0.f,2 * (float)glfwGetTime(), 0.f));
		helmet.setRotation(glm::vec3(6 * (float)glfwGetTime(), 0.f, 0.f));

		mgr.getCamera()->lookAt(eye, glm::vec3(0.f, 0.f, 0.f),
								glm::vec3(0.f, 1.f, 0.f));
		mgr.drawScene();
		


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