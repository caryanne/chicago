#include "ShaderManager.h"


Shader* ShaderManager::getShader(const string& name) {
	if(mShaders.find(name) != mShaders.end())
		return mShaders.find(name)->second;
	else
		return loadShader(name);
}

Shader* ShaderManager::loadShader(const string& name) {
	Shader* shader = new Shader(("media/shaders/" + name).c_str());
	mShaders[name] = shader;
	return shader;
}

void ShaderManager::unload() {
	for(map<string, Shader*>::iterator it = mShaders.begin(); it != mShaders.end(); ++it) {
		(*it).second->unload();
		delete (*it).second;
	}
}