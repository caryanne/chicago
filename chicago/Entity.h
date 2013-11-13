#ifndef _Entity_H_
#define _Entity_H_


#include "Mesh.h"

class Entity {

private:
	Mesh *mMesh;

public:
	Entity() {}
	Entity(Mesh* mesh) { setMesh(mesh); }
	Mesh* getMesh() { return mMesh; }
	//Shader* getShader() { return mMesh->getShader(0); }
	void setMesh(Mesh* mesh) { mMesh = mesh; }
};

#endif