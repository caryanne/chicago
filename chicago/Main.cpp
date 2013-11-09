#include <gl\glew.h>
#include <windows.h>
#include "glfw\glfw3.h"
#include <gl\GLU.h>
#include "bullet\btBulletDynamicsCommon.h"

#include "soil\SOIL.h"
#include "Shader.h"


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

bool keyW = false, keyS = false, keyA = false, keyD = false;

void setup3d(double w, double h) {

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, w, h);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);


}

void reloadResources() {
	
	mgr.reloadScene();

}

static void keyPress(GLFWwindow* window, int key, int scanCode, int action, int mods) {
	if(key == GLFW_KEY_F1 && action == GLFW_PRESS)
		reloadResources();
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	if(key == GLFW_KEY_W && action == GLFW_PRESS)
		keyW = true;
	if(key == GLFW_KEY_W && action == GLFW_RELEASE)
		keyW = false;
	if(key == GLFW_KEY_S && action == GLFW_PRESS)
		keyS = true;
	if(key == GLFW_KEY_S && action == GLFW_RELEASE)
		keyS = false;
	if(key == GLFW_KEY_A && action == GLFW_PRESS)
		keyA = true;
	if(key == GLFW_KEY_A && action == GLFW_RELEASE)
		keyA = false;
	if(key == GLFW_KEY_D && action == GLFW_PRESS)
		keyD = true;
	if(key == GLFW_KEY_D && action == GLFW_RELEASE)
		keyD = false;


		

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

	glClearColor(0.0f, 0.5f, 1.f, 1.f);
	unsigned frames = 0;
	double lastUpdate = 0;
	
	printf("%.2f:end initializing systems in %.2fs\n", glfwGetTime(), glfwGetTime() - start);

	Mesh basemesh = Mesh("station2.obj");
	Entity baseent = Entity(&basemesh);
	SceneNode base = SceneNode(&baseent);

	mgr.getRootNode()->addChild(&base);

	Mesh gndmesh = Mesh("ground.obj");
	Entity gndent = Entity(&gndmesh);
	SceneNode ground = SceneNode(&gndent);

	mgr.getRootNode()->addChild(&ground);

	Mesh helmetmesh = Mesh("helmetframe.obj");
	Entity helmetent = Entity(&helmetmesh);
	SceneNode helmet = SceneNode(&helmetent);
	//helmet.setScale(glm::vec3(.95,.95,.95));
	mgr.getRootNode()->addChild(&helmet);

	Mesh shieldmesh = Mesh("helmetshield.obj");
	Entity shieldent = Entity(&shieldmesh);
	SceneNode shield = SceneNode(&shieldent);
	helmet.addChild(&shield);

	mgr.getCamera()->setPosition(glm::vec3(0,1,10));
	

	mgr.setScreenRatio(width / (float) height);
	
	double last = glfwGetTime();

	glm::vec3 movSpeed = glm::vec3(0.f);


	double xRotSpeed = 0.0, yRotSpeed = 0.0, pitch = 0.0, yaw = 0.0;

	double mouseX = 0.0, mouseY = 0.0;
	glfwSetCursorPos(window, width / 2, height / 2);
	glfwGetCursorPos(window, &mouseX, &mouseY);


	while(!glfwWindowShouldClose(window)) {

	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		setup3d(width, height);

		
		double delta = glfwGetTime() - last;
		last = glfwGetTime();

		double tX, tY = 0.0;
		glfwGetCursorPos(window, &tX, &tY);
		double dX = mouseX - tX;
		double dY = mouseY - tY;
		glfwSetCursorPos(window, mouseX, mouseY);


		if(keyW)
			movSpeed = glm::vec3(delta * 5.f);
		if(keyS)
			movSpeed = glm::vec3(delta * -5.f);
		if(abs(dY) > 0.0 && abs(dY))
			xRotSpeed += dY * 0.001;
		if(abs(dX) > 0.0 && abs(dX))
			yRotSpeed += dX * 0.001;

		pitch += xRotSpeed;
		yaw += yRotSpeed;
		xRotSpeed *= 0.995f;
		yRotSpeed *= 0.995f;

		mgr.getCamera()->setPosition(
				mgr.getCamera()->getPosition() - movSpeed * mgr.getCamera()->getDirection());
		

		mgr.getCamera()->setRotation(glm::vec3(glm::radians(pitch), glm::radians(yaw), 0.f));


		movSpeed *= 0.999f;

		glm::vec3 eye = mgr.getCamera()->getPosition();
		
		helmet.setPosition(eye - glm::vec3(0.2) * mgr.getCamera()->getDirection());

		helmet.setRotation(glm::toQuat(glm::inverse(mgr.getCamera()->getView())));


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