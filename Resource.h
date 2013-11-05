#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include <string>
using namespace std;

class Resource {

private:
	string mName;

public:
	Resource(const string& name) { mName = name; }
	virtual const string& getName() { return mName; }
	virtual void setName(const string& name) { mName = name; }

};

#endif