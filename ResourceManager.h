#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_

#include <map>
#include <string>
#include "Resource.h"

using namespace std;

class ResourceManager {

private:
	ResourceManager() {}
	ResourceManager(ResourceManager const*);
	void operator =(ResourceManager const*);
	map<string, Resource> mResources;

public:
	static ResourceManager& getInstance() {
		static ResourceManager instance;
		return instance;
	}


};

#endif