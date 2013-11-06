#ifndef _Entity_H_
#define _Entity_H_


#include "Mesh.h"

class Entity {

private:
	Mesh *mMesh;

public:
	Entity();
	Entity(Mesh* mesh) { setMesh(mesh); }
	Mesh* getMesh() { return mMesh; }
	void setMesh(Mesh* mesh) { mMesh = mesh; }
	void bind() {
		getMesh()->draw();
		
	};


};

#endif