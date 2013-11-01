#include <gl/glew.h>
#include <windows.h>
#include "glfw/glfw3.h"
#include <gl/GLU.h>
#include "bullet/btBulletDynamicsCommon.h"
#include "soil/SOIL.h"

GLFWwindow *window;

static char *readFile(const char *filename) {
	FILE *fp = fopen(filename, "r");
	fseek(fp, 0, SEEK_END);
	long length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char *contents = new char[length + 1];
	for(int i = 0; i < length + 1; i++)
		contents[i] = 0;
	fread(contents, 1, length, fp);
	contents[length + 1] = '\0';
	fclose(fp);
	return contents;
}

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
bool CompileSuccessful(int obj) {
  int status;
  glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
  return status == GL_TRUE;
}
bool LinkSuccessful(int obj) {
  int status;
  glGetProgramiv(obj, GL_LINK_STATUS, &status);
  return status == GL_TRUE;
}
GLuint makeVertexShader(const char *filename) {
	GLuint id = glCreateShader(GL_VERTEX_SHADER);
	char *source = readFile(filename);
	glShaderSource(id, 1, (const GLchar**)&source, NULL);
	glCompileShader(id);
	printf("vertex shader %i compiled %i\n", id, CompileSuccessful(id));
	return id;
}
GLuint makePixelShader(const char *filename) {
	GLuint id = glCreateShader(GL_FRAGMENT_SHADER);
	char *source = readFile(filename);
	glShaderSource(id, 1, (const GLchar**)&source, NULL);
	glCompileShader(id);
	printf("pixel shader %i compiled %i\n", id, CompileSuccessful(id));
	return id;
}
GLuint makeShaderProgram(GLuint vertexShader, GLuint pixelShader) {
	GLuint id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, pixelShader);
	glLinkProgram(id);
	printf("shader program %i compiled %i\n", id, LinkSuccessful(id));
	return id;
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
	GLuint vao;
	GLuint vbo;
	GLuint pos;
	GLuint col;
	GLuint shader = makeShaderProgram(makeVertexShader("vertex.glsl"), makePixelShader("pixel.glsl"));

	GLfloat verts[] = {-0.5f, 0.f, 0.f,
						0.f, 0.5f, 0.f,
						0.5f,0.f,0.f};

	GLfloat color[] = { 1.f, 0.f, 0.f, 1.f,
						0.f, 1.f, 0.f, 1.f,
						0.f, 0.f, 1.f, 1.f};
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 7 * 3, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 9 * sizeof(GLfloat), verts);
	glBufferSubData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), 12 * sizeof(GLfloat), color);

	pos = glGetAttribLocation(shader, "s_vpos");
	col = glGetAttribLocation(shader, "s_vcolor");

	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(col, 4, GL_FLOAT, GL_FALSE, 0, ((char*)NULL + (9 * sizeof(GLfloat))));
	glUseProgram(shader);
	glEnableVertexAttribArray(pos);
	glEnableVertexAttribArray(col);

	glClearColor(0.f, 0.5f, 1.f, 1.f);
	
	while(!glfwWindowShouldClose(window)) {

	
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		setup3d(width, height);

		gluLookAt(10.f, 10.0f, 10.f,
					0.f, 0.0f, 0.f,
					0.f, 1.f, 0.f);

		/*glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f); glVertex3f(-1.f, 0.f, 0.f);
		glColor3f(0.f, 1.f, 0.f); glVertex3f(0.f, 1.f, 0.f);
		glColor3f(0.f, 0.f, 1.f); glVertex3f(1.f, 0.f, 0.f);
		glEnd();*/
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	exit(EXIT_SUCCESS);
}