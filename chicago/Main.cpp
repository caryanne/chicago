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

	btBroadphaseInterface* broadphase = new btDbvtBroadphase();
	btDefaultCollisionConfiguration* collisionConfig = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfig);
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
	
	btDiscreteDynamicsWorld* world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
	world->setGravity(btVector3(0, -5.0, 0));

	Mesh modulemesh = Mesh("scene.obj");
	SceneNode module = SceneNode(&modulemesh, true);
	mgr.getRootNode()->addChild(&module);

	Mesh cubemesh = Mesh("cube.obj");
	SceneNode cube = SceneNode(&cubemesh, false);
	mgr.getRootNode()->addChild(&cube);
	world->addRigidBody(cube.getRigidBody());

	Mesh monkeysmesh = Mesh("monkeys.obj");
	SceneNode monkeys = SceneNode(&monkeysmesh);
	mgr.getRootNode()->addChild(&monkeys);
	world->addRigidBody(monkeys.getRigidBody());
	monkeys.setPosition(glm::vec3(3, 3, 0));
	

	Mesh skymesh = Mesh("skysphere.obj");
	SceneNode sky = SceneNode(&skymesh, true);
	mgr.getRootNode()->addChild(&sky);

	/*Mesh helmetmesh = Mesh("helmetframe.obj");
	Entity helmetent = Entity(&helmetmesh);
	SceneNode helmet = SceneNode(&helmetent);
	mgr.getRootNode()->addChild(&helmet);

	Mesh shieldmesh = Mesh("helmetshield.obj");
	Entity shieldent = Entity(&shieldmesh);
	SceneNode shield = SceneNode(&shieldent);
	helmet.addChild(&shield);*/
	
	mgr.getCamera()->setPosition(glm::vec3(0,2,0));
	mgr.getCamera()->setFOV(90.f);
	mgr.setScreenRatio(width / (float) height);
	

	//double mouseX = 0.0, mouseY = 0.0;
	//glfwSetCursorPos(window, width / 2, height / 2);
	//glfwGetCursorPos(window, &mouseX, &mouseY);

	btVector3 inertia;
	
	btCollisionShape* moduleShape = new btBoxShape(btVector3(50, 0.5f, 50));
	btDefaultMotionState* moduleMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	moduleShape->calculateLocalInertia(0, inertia);
	btRigidBody::btRigidBodyConstructionInfo
		moduleRigidBodyCI(0, moduleMotionState, moduleShape, inertia);
	btRigidBody* moduleRigidBody = new btRigidBody(moduleRigidBodyCI);
	world->addRigidBody(moduleRigidBody);

	btCollisionShape* camShape = new btCapsuleShape(1, 3);
	btDefaultMotionState* camMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 5, 6)));
	camShape->calculateLocalInertia(5, inertia);
	btRigidBody::btRigidBodyConstructionInfo
		camRigidBodyCI(5, camMotionState, camShape, inertia);
	
	btRigidBody* camRigidBody = new btRigidBody(camRigidBodyCI);

	camRigidBody->setActivationState(DISABLE_DEACTIVATION);
	camRigidBody->setDamping(0.4f, 0.4f);

	world->addRigidBody(camRigidBody);

	cube.getRigidBody()->applyTorqueImpulse(btVector3(3, 1, 2));
	cube.getRigidBody()->applyCentralImpulse(btVector3(0, 0.7, 0.8));

	printf("%.2f:end initializing systems in %.2fs\n", glfwGetTime(), glfwGetTime() - start);
	
	char buffer[20];
	unsigned frames = 0;
	unsigned fps = 0;
	double lastUpdate = 0;
	double last = glfwGetTime();

	//ShowCursor(false);
	while(!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		setup3d(width, height);
		
		double delta = glfwGetTime() - last;
		last = glfwGetTime();

		//double tX, tY = 0.0;
		//glfwGetCursorPos(window, &tX, &tY);
		//double dX = mouseX - tX;
		//double dY = mouseY - tY;
		//glfwSetCursorPos(window, mouseX, mouseY);

		btVector3 camForce = btVector3(0, 0, 0);
		btVector3 camTorque = btVector3(0, 0, 0);
		btMatrix3x3& camRot = camRigidBody->getWorldTransform().getBasis();

		if(!keySpace) {
			if(keyW) camForce += camRot * btVector3(0, 0, -0.05f);
			if(keyS) camForce += camRot * btVector3(0, 0, 0.05f);
			if(keyA) camForce += camRot * btVector3(-0.05f, 0, 0);
			if(keyD) camForce += camRot * btVector3(0.05f, 0, 0);
		} else {
			if(keyA) camTorque += camRot * btVector3(0, 0.005f, 0);
			if(keyD) camTorque += camRot * btVector3(0, -0.005f, 0);
			if(keyW) camTorque += camRot * btVector3(-0.01f, 0, 0);
			if(keyS) camTorque += camRot * btVector3(0.01f, 0, 0);
		}
		if(keyE) camTorque = camRot * btVector3(0, 0, -0.025f);
		if(keyQ) camTorque = camRot * btVector3(0, 0, 0.025f);

		camRigidBody->applyCentralImpulse(camForce);
		camRigidBody->applyTorqueImpulse(camTorque);


		world->stepSimulation(delta, 10); 

		
		btTransform camTrans;
		camRigidBody->getMotionState()->getWorldTransform(camTrans);
		mgr.getCamera()->setPosition(glm::vec3(camTrans.getOrigin().x(), camTrans.getOrigin().y(), camTrans.getOrigin().z()));
		mgr.getCamera()->setRotation(glm::quat(camTrans.getRotation().w(), camTrans.getRotation().x(), camTrans.getRotation().y(), camTrans.getRotation().z()));
		
		glm::vec3 eye = mgr.getCamera()->getPosition();
		sky.setPosition(eye);
		
		//mgr.setLightPos(glm::vec4(cube.getPosition(), 1.0));
		//mgr.setLightPos(glm::vec4(eye, 1.0));
		mgr.setLightPos(glm::vec4(-5 * sin(glfwGetTime()), 7.0 +  sin(glfwGetTime() * 2.0), -6 * cos(glfwGetTime()), 1));
		monkeys.setRotation(glm::vec3(0, glm::radians(glfwGetTime() * 70), 0));
		//helmet.setPosition(eye + glm::vec3(0.2) * mgr.getCamera()->getDirection());
		//helmet.setRotation(glm::toQuat(glm::inverse(mgr.getCamera()->getView())));

		
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
	//ShowCursor(true);
	ShaderManager::getInstance().unload();
	world->removeRigidBody(camRigidBody);
	delete camRigidBody->getMotionState();
	delete camRigidBody;


	world->removeRigidBody(moduleRigidBody);
	delete moduleRigidBody->getMotionState();
	delete moduleRigidBody;

	delete moduleShape;
	delete world;
	delete solver;
	delete dispatcher;
	delete collisionConfig;
	delete broadphase;
	imguiRenderGLDestroy();
	glfwTerminate();
	exit(EXIT_SUCCESS);
}