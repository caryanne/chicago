#ifndef _Entity_H_
#define _Entity_H_

#include "MoveableObject.h"
#include "Mesh.h"

class Entity: public MoveableObject {

private:
	Mesh *mMesh;

public:
	Entity();
	Entity(Mesh* mesh) { setMesh(mesh); }
	Mesh* getMesh() { return mMesh; }
	void setMesh(Mesh* mesh) { mMesh = mesh; }
	void bind();


};

#endif