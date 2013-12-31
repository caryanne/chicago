#include <gl\glew.h>
#include <windows.h>
#include "glfw\glfw3.h"
#include <gl\GLU.h>

#include "soil\SOIL.h"
#include "Shader.h"


#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\matrix_inverse.hpp"

#include "ShaderManager.h"
#include "SceneManager.h"
#include "SceneNode.h"
#include "Camera.h"
#include "Mesh.h"

#include "imgui\imgui.h"
#include "imgui\imguiRenderGL3.h"

GLFWwindow *window;

SceneManager mgr;

bool keyW = false, keyS = false, keyA = false, keyD = false, keyQ = false, keyE = false;
bool keyUp = false, keyDown = false, keyRight = false, keyLeft = false;
bool keySpace = false;

void setup3d(double w, double h) {

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, w, h);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_MULTISAMPLE);
}

void prepareGUI(double w, double h) {
	glBindVertexArray(0);
	glUseProgram(0);

	glViewport(0, 0, w, h);
	glDisable(GL_DEPTH_TEST);

}

void reloadResources() {
	
	mgr.reloadScene();

}

static void keyPress(GLFWwindow* window, int key, int scanCode, int action, int mods) {
	if(key == GLFW_KEY_F1 && action == GLFW_PRESS) reloadResources();
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
	
	if(key == GLFW_KEY_W && action == GLFW_PRESS) keyW = true;
	if(key == GLFW_KEY_W && action == GLFW_RELEASE)	keyW = false;
	if(key == GLFW_KEY_S && action == GLFW_PRESS) keyS = true;
	if(key == GLFW_KEY_S && action == GLFW_RELEASE)	keyS = false;
	if(key == GLFW_KEY_A && action == GLFW_PRESS) keyA = true;
	if(key == GLFW_KEY_A && action == GLFW_RELEASE)	keyA = false;
	if(key == GLFW_KEY_D && action == GLFW_PRESS) keyD = true;
	if(key == GLFW_KEY_D && action == GLFW_RELEASE)	keyD = false;
	if(key == GLFW_KEY_Q && action == GLFW_PRESS) keyQ = true;
	if(key == GLFW_KEY_Q && action == GLFW_RELEASE)	keyQ = false;
	if(key == GLFW_KEY_E && action == GLFW_PRESS) keyE = true;
	if(key == GLFW_KEY_E && action == GLFW_RELEASE)	keyE = false;

	if(key == GLFW_KEY_UP && action == GLFW_PRESS) keyUp = true;
	if(key == GLFW_KEY_UP && action == GLFW_RELEASE) keyUp = false;
	if(key == GLFW_KEY_DOWN && action == GLFW_PRESS) keyDown = true;
	if(key == GLFW_KEY_DOWN && action == GLFW_RELEASE) keyDown = false;
	if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS) keyRight = true;
	if(key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)	keyRight = false;
	if(key == GLFW_KEY_LEFT && action == GLFW_PRESS) keyLeft = true;
	if(key == GLFW_KEY_LEFT && action == GLFW_RELEASE) keyLeft = false;
	if(key == GLFW_KEY_SPACE && action == GLFW_PRESS) keySpace = true;
	if(key == GLFW_KEY_SPACE && action == GLFW_RELEASE) keySpace = false;		

}

int main() {

	if(!glfwInit())
		exit(EXIT_FAILURE);
	printf("%.2f:started. begin initializing systems\n", glfwGetTime());
	double start = glfwGetTime();
	//window = glfwCreateWindow(1920, 1080, "chicago", glfwGetPrimaryMonitor(), NULL);
	window = glfwCreateWindow(1728, 972, "chicago", NULL, NULL);
	//window = glfwCreateWindow(1024, 768, "chicago", NULL, NULL);
	
	if(!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	printf("%.2f:window created\n", glfwGetTime());
	glfwSetWindowPos(window, 20, 20);
	
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyPress);
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	printf("%.2f:OpenGL context created\n", glfwGetTime());
	glfwSwapInterval(1);
	if(glewInit() != GLEW_OK)
		exit(EXIT_FAILURE);
	printf("%.2f:checked OpenGL extensions\n", glfwGetTime());
	
	if(!imguiRenderGLInit("media/fonts/DroidSans.ttf"))
		exit(EXIT_FAILURE);
	
	//load and set up shit

	glClearColor(0.0f, 0.5f, 1.f, 1.f);

	Mesh modulemesh = Mesh("scene.obj"); SceneNode module = SceneNode(&modulemesh);
	mgr.getRootNode()->addChild(&module);

	Mesh cubemesh = Mesh("cube.obj"); SceneNode cube = SceneNode(&cubemesh);
	mgr.getRootNode()->addChild(&cube);


	Mesh monkeysmesh = Mesh("monkeys.obj");	SceneNode monkeys = SceneNode(&monkeysmesh);
	mgr.getRootNode()->addChild(&monkeys);
	monkeys.setPosition(glm::vec3(3, 3, 0));
	

	Mesh skymesh = Mesh("skysphere.obj");
	SceneNode sky = SceneNode(&skymesh);
	mgr.getRootNode()->addChild(&sky);

	mgr.getCamera()->setPosition(glm::vec3(0,5,5));
	mgr.getCamera()->setFOV(90.f);
	mgr.setScreenRatio(width / (float) height);
	


	printf("%.2f:end initializing systems in %.2fs\n", glfwGetTime(), glfwGetTime() - start);
	
	char buffer[20];
	unsigned frames = 0;
	unsigned fps = 0;
	double lastUpdate = 0;
	double last = glfwGetTime();


	while(!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		setup3d(width, height);
		
		double delta = glfwGetTime() - last;
		last = glfwGetTime();
		
		glm::vec3 eye = mgr.getCamera()->getPosition();
		sky.setPosition(eye);
	
		mgr.setLightPos(glm::vec4(-5 * sin(glfwGetTime()), 7.0 +  sin(glfwGetTime() * 2.0), -6 * cos(glfwGetTime()), 1));
		monkeys.setRotation(glm::vec3(0, glm::radians(glfwGetTime() * 70), 0));
		
		mgr.drawScene();
		
		prepareGUI(width, height);
		imguiBeginFrame(100, 100, 0, 0);
        imguiEndFrame();	
        imguiDrawText(20, height - 20, IMGUI_ALIGN_LEFT, buffer, imguiRGBA(255, 255, 255, 255));
        imguiRenderGLDraw(width, height); 

		glfwSwapBuffers(window);

		frames++;
		if(glfwGetTime() - lastUpdate > 1.0) {
			fps = frames;
			sprintf(buffer, "%i fps", fps);
			lastUpdate = glfwGetTime();
			frames = 0;
		}

		glfwPollEvents();
	}

	ShaderManager::getInstance().unload();
	
	imguiRenderGLDestroy();
	glfwTerminate();
	exit(EXIT_SUCCESS);
}