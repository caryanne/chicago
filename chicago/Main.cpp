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
	//window = glfwCreateWindow(1920, 1080, "chicago", glfwGetPrimaryMonitor(), NULL);
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
	
	Mesh modulemesh = Mesh("module.obj");
	Entity moduleent = Entity(&modulemesh);
	SceneNode module = SceneNode(&moduleent);
	mgr.getRootNode()->addChild(&module);

	Mesh cubemesh = Mesh("cube.obj");
	Entity cubeent = Entity(&cubemesh);
	SceneNode cube = SceneNode(&cubeent);
	mgr.getRootNode()->addChild(&cube);
	cube.setPosition(glm::vec3(0, 4, 4));

	Mesh skymesh = Mesh("skysphere.obj");
	Entity skyent = Entity(&skymesh);
	SceneNode sky = SceneNode(&skyent);
	mgr.getRootNode()->addChild(&sky);

	Mesh helmetmesh = Mesh("helmetframe.obj");
	Entity helmetent = Entity(&helmetmesh);
	SceneNode helmet = SceneNode(&helmetent);
	mgr.getRootNode()->addChild(&helmet);

	Mesh shieldmesh = Mesh("helmetshield.obj");
	Entity shieldent = Entity(&shieldmesh);
	SceneNode shield = SceneNode(&shieldent);
	helmet.addChild(&shield);

	mgr.getCamera()->setPosition(glm::vec3(0,2,0));
	mgr.getCamera()->setFOV(120.f);
	mgr.setScreenRatio(width / (float) height);
	
	glm::vec3 movSpeed = glm::vec3(0.f);
	glm::vec3 strafeSpeed = glm::vec3(0.f);

	double xRotSpeed = 0.0, yRotSpeed = 0.0, pitch = 0.0, yaw = 0.0;
	double mouseX = 0.0, mouseY = 0.0;
	glfwSetCursorPos(window, width / 2, height / 2);
	glfwGetCursorPos(window, &mouseX, &mouseY);

	btBroadphaseInterface* broadphase = new btDbvtBroadphase();
	btDefaultCollisionConfiguration* collisionConfig = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfig);
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
	
	btDiscreteDynamicsWorld* world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
	world->setGravity(btVector3(0, -9.8, 0));

	btCollisionShape* moduleShape = new btBoxShape(btVector3(10, 1, 10));
	btCollisionShape* cubeShape = new btBoxShape(btVector3(1, 1, 1));

	btDefaultMotionState* moduleMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		moduleRigidBodyCI(0, moduleMotionState, moduleShape, btVector3(0, 0, 0));
	btRigidBody* moduleRigidBody = new btRigidBody(moduleRigidBodyCI);

	btDefaultMotionState* cubeMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 4, 0)));
	cubeShape->calculateLocalInertia(1, btVector3(0, 0, 0));
	btRigidBody::btRigidBodyConstructionInfo
		cubeRigidBodyCI(1, cubeMotionState, cubeShape, btVector3(0, 0, 0));
	btRigidBody* cubeRigidBody = new btRigidBody(cubeRigidBodyCI);


	printf("%.2f:end initializing systems in %.2fs\n", glfwGetTime(), glfwGetTime() - start);
	
	unsigned frames = 0;
	double lastUpdate = 0;
	double last = glfwGetTime();
	
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

		if(keyW) movSpeed = glm::vec3(delta * 7.f);
		if(keyS) movSpeed = glm::vec3(delta * -7.f);
		if(keyA) strafeSpeed = glm::vec3(delta * 7.f);
		if(keyD) strafeSpeed = glm::vec3(delta * -7.f);

		if(abs(dY) > 0.0 && abs(dY)) xRotSpeed += dY * 0.001;
		if(abs(dX) > 0.0 && abs(dX)) yRotSpeed += dX * 0.001;

		pitch += xRotSpeed;
		if(pitch > 80.0) pitch = 80.0;
		if(pitch < -80.0) pitch = -80.0;
		yaw += yRotSpeed;
		xRotSpeed *= 0.995f;
		yRotSpeed *= 0.995f;

		mgr.getCamera()->setPosition(mgr.getCamera()->getPosition() - movSpeed * mgr.getCamera()->getDirection());
		mgr.getCamera()->setPosition(mgr.getCamera()->getPosition() - strafeSpeed * glm::cross(glm::vec3(0,1,0), mgr.getCamera()->getDirection()));
		mgr.getCamera()->setRotation(glm::vec3(glm::radians(pitch), glm::radians(yaw), 0.f));

		sky.setPosition(mgr.getCamera()->getPosition());

		movSpeed *= 0.999f;
		strafeSpeed *= 0.999f;

		glm::vec3 eye = mgr.getCamera()->getPosition();
		
		helmet.setPosition(eye + glm::vec3(0.17) * mgr.getCamera()->getDirection());
		helmet.setRotation(glm::toQuat(glm::inverse(mgr.getCamera()->getView())));

		world->stepSimulation(delta, 10); 
		btTransform cubeTrans;
		cubeRigidBody->getMotionState()->getWorldTransform(cubeTrans);
		cube.setPosition(glm::vec3(cubeTrans.getOrigin().getX(),
									cubeTrans.getOrigin().getY(),
									cubeTrans.getOrigin().getZ()));
	

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
	delete cubeRigidBody;
	delete cubeMotionState;
	delete moduleRigidBody;
	delete moduleMotionState;
	delete cubeShape;
	delete moduleShape;
	delete world;
	delete solver;
	delete dispatcher;
	delete collisionConfig;
	delete broadphase;
	glfwTerminate();
	exit(EXIT_SUCCESS);
}