#ifndef _SHADERMANAGER_H_
#define _SHADERMANAGER_H_

#include <gl\glew.h>
#include "glfw\glfw3.h"
#include "Shader.h"
#include <string>
#include <map>


using namespace std;

class ShaderManager {
private:
	ShaderManager() {};
	ShaderManager(ShaderManager const&);
	void operator=(ShaderManager const&);

	map<string, Shader*> mShaders;

	Shader* loadShader(const string& name);

public:
	static ShaderManager& getInstance() {
		static ShaderManager instance;
		return instance;
	}
	
	Shader* getShader(const string& name);

	void unload();
};

#endif